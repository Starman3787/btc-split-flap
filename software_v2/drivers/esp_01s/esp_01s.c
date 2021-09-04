#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "util/http/http.h"
#include "util/delay/delay.h"
#include "util/time/parse_date.h"
#include "timers/systick/systick.h"
#include "drivers/uart/uart.h"
#include "drivers/esp_01s/esp_01s.h"
#include "main.h"

// sends a test command to the module and expects to receive an OK indicating the module is working fine
int8_t esp_01s_test(void)
{
    write_full_uart("AT");
    if (read_full_uart_and_expect("AT\r\r\n\r\nOK\r\n", 1000) != 0)
        return -1;
    return 0;
}

// waits until the module has connected to wifi
// can fail if the test command is unsuccessful
int8_t init_esp_01s(void)
{
    if (find_pattern("WIFI GOT IP\r\n", 13, 20000U) != 0)
        return -1;
    delay_ms(5000);
    if (esp_01s_test() != 0)
        return -1;
    else
        return 0;
}

int8_t send_cip_start_command(char *protocol, char *host, char *port)
{
    #define CIP_START_COMMAND_MAX_LENGTH    (127)
    char command[CIP_START_COMMAND_MAX_LENGTH + 1] = "AT+CIPSTART=";
    strncat(command, "\"", CIP_START_COMMAND_MAX_LENGTH - strlen(command));
    strncat(command, protocol, CIP_START_COMMAND_MAX_LENGTH - strlen(command));
    strncat(command, "\",\"", CIP_START_COMMAND_MAX_LENGTH - strlen(command));
    strncat(command, host, CIP_START_COMMAND_MAX_LENGTH - strlen(command));
    strncat(command, "\",", CIP_START_COMMAND_MAX_LENGTH - strlen(command));
    strncat(command, port, CIP_START_COMMAND_MAX_LENGTH - strlen(command));
    write_full_uart(command);
    if (read_full_uart_and_expect(strcat(command, "\r\r\n"), 1000) != 0)
        return -1;
    if (read_full_uart_and_expect("CONNECT", 10000) != 0)
        return -1;
    if (read_full_uart_and_expect("\r\n\r\n", 1000) != 0)
        return -1;
    if (read_full_uart_and_expect("OK\r\n", 1000) != 0)
        return -1;
    return 0;
}

int8_t send_cip_send_command(char *size, char *httpRequest)
{
    #define CIP_SEND_COMMAND_MAX_LENGTH     (127)
    char command[CIP_SEND_COMMAND_MAX_LENGTH + 1] = "AT+CIPSEND=";
    strncat(command, size, CIP_SEND_COMMAND_MAX_LENGTH - strlen(command));
    write_full_uart(command);
    if (read_full_uart_and_expect(strcat(command, "\r\r\n"), 1000) != 0)
        return -1;
    if (read_full_uart_and_expect("\r\n", 1000) != 0)
        return -1;
    if (read_full_uart_and_expect("OK\r\n", 1000) != 0)
        return -1;
    if (read_full_uart_and_expect(">", 1000) != 0)
        return -1;
    write_full_uart(httpRequest);
    return 0;
}

// listens to the UART and parses the response
// will also use the time from the "date" header to calibrate the system time
int8_t response_parser(Http *response, char *jsonProperty)
{
    char rawResponse[14601];
    rawResponse[0] = '\0';
    uint16_t index = 0;
    if (find_pattern("+IPD,", 5, 20000U) != 0)
        return -1;;
    while (1)
    {
        uint16_t currentChunkLength;
        char currentCharacter = read_uart(1000U);
        char rawChunkLength[5];
        uint8_t counter;
        for (counter = 0; currentCharacter != ':'; counter++)
        {
            if (counter > 4)
                return -1;
            rawChunkLength[counter] = currentCharacter;
            rawChunkLength[counter + 1] = '\0';
            currentCharacter = read_uart(1500U);
        }
        currentChunkLength = strtoul(rawChunkLength, NULL, 10);
        uint16_t bodyCounter;
        for (bodyCounter = 0; bodyCounter < currentChunkLength; bodyCounter++, index++)
        {   
            if (index > 14599)
                return -1;
            rawResponse[index] = read_uart(1500U);
            rawResponse[index + 1] = '\0';
        }
        char firstChar = read_uart(1000U);
        if (firstChar == '\r')
        {
            if (read_full_uart_and_expect("\n+IPD,", 10000UL) != 0)
                break;
        }
        else if (firstChar == 'C')
        {
            if (find_pattern("LOSED\r\n", 7, 20000U) != 0)
                return -1;
            break;
        }
    }
    if (parse_http(response, jsonProperty, rawResponse) != 0)
        return -1;
    Header currentTimeString;
    if (find_header(&currentTimeString, response->headers, response->headersLength, "date") != 0)
        return -1;
    time_t timestamp;
    parse_date(&timestamp, currentTimeString.value);
    unix = timestamp;
    return 0;
}

// triggers a HTTP request
int8_t make_http_request(Http *response, char *jsonProperty, char *protocol, char *host, char *port, char *size, char *httpRequest)
{
    if (send_cip_start_command(protocol, host, port) != 0)
        return -1;
    if (send_cip_send_command(size, httpRequest) != 0)
        return -1;
    response_parser(response, jsonProperty);
    return 0;
}