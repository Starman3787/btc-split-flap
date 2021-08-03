#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../../util/http/http.h"

bool esp_01s_test(void)
{
    write_full_uart("AT");
    if (read_full_uart_and_expect("AT\r\r\n\r\nOK\r\n", 1000) != true)
        return false;
    return true;
}

void init_esp_01s(void)
{
    status_loading(true);
    find_pattern("WIFI GOT IP\r\n", 13);
    delay_ms(5000);
    if (esp_01s_test() == false)
        status_error(true);
    else
        status_ok(true);
}

bool send_cip_start_command(char *protocol, char *host, char *port)
{
    char *command = malloc(strlen("AT+CIPSTART=\"") + strlen(protocol) + strlen("\",\"") + strlen(host) + strlen("\",") + strlen(port) + 1);
    command[0] = '\0';
    strcat(command, "AT+CIPSTART=\"");
    strcat(command, protocol);
    strcat(command, "\",\"");
    strcat(command, host);
    strcat(command, "\",");
    strcat(command, port);
    print_full(command);
    write_full_uart(command);
    if (read_full_uart_and_expect(strcat(command, "\r\r\n"), 1000) == false)
    {
        print_full("nope");
        return false;
    }
    if (read_full_uart_and_expect("CONNECT", 10000) == false)
        return false;
    if (read_full_uart_and_expect("\r\n\r\n", 1000) == false)
        return false;
    if (read_full_uart_and_expect("OK\r\n", 1000) == false)
        return false;
    free(command);
    return true;
}

bool send_cip_send_command(char *size, char *httpRequest)
{
    char *command = malloc(strlen("AT+CIPSEND=") + strlen(size) + 1);
    command[0] = '\0';
    strcat(command, "AT+CIPSEND=");
    strcat(command, size);
    print_full(command);
    write_full_uart(command);
    if (read_full_uart_and_expect(strcat(command, "\r\r\n"), 1000) == false)
        return false;
    if (read_full_uart_and_expect("\r\n", 1000) == false)
        return false;
    if (read_full_uart_and_expect("OK\r\n", 1000) == false)
        return false;
    if (read_full_uart_and_expect(">", 1000) == false)
        return false;
    write_full_uart(httpRequest);
    free(command);
    return true;
}

char *response_parser(void)
{
    char *fullRawResponse = malloc(sizeof(char) * 1);
    *fullRawResponse = '\0';
    size_t currentResponseSize = 1;
    uint16_t responseStart = find_pattern("+IPD,", 5);
    while (1)
    {
        uint16_t currentChunkLength;
        char currentCharacter = read_uart(1000);
        print(currentCharacter);
        char *rawChunkLength = malloc(sizeof(char) * 2);
        uint8_t counter;
        for (counter = 0; currentCharacter != ':'; counter++)
        {
            rawChunkLength = realloc(rawChunkLength, sizeof(char) * (counter + 2));
            *(rawChunkLength + counter) = currentCharacter;
            *(rawChunkLength + counter + 1) = '\0';
            currentCharacter = read_uart(10000);
            print(currentCharacter);
        }
        char *endOfRawChunkLengthString = rawChunkLength + counter + 1;
        currentChunkLength = strtoul(rawChunkLength, &endOfRawChunkLengthString, 10) + 1;
        free(rawChunkLength);
        char *chunk = malloc(sizeof(char) * currentChunkLength);
        uint16_t bodyCounter;
        for (bodyCounter = 0; bodyCounter < currentChunkLength - 1; bodyCounter++)
        {   
            *(chunk + bodyCounter) = read_uart(10000);
            print(*(chunk + bodyCounter));
        }
        *(chunk + currentChunkLength - 1) = '\0';
        currentResponseSize += currentChunkLength - 1;
        fullRawResponse = realloc(fullRawResponse, sizeof(char) * currentResponseSize);
        strcat(fullRawResponse, chunk);
        free(chunk);
        *(fullRawResponse + currentResponseSize + currentChunkLength - 1) = '\0';
        if (read_full_uart_and_expect("\r\n+IPD,", 10000) != true)
            break;
        responseStart = responseStart + counter + 1 + bodyCounter + 7;
    }
    return fullRawResponse;
}

Http *make_http_request(char *protocol, char *host, char *port, char *size, char *httpRequest)
{
    if (send_cip_start_command(protocol, host, port) != true)
    {
        write_led(2, true);
        return 0U;
    }
    if (send_cip_send_command(size, httpRequest) != true)
    {
        write_led(2, true);
        return 0U;
    }
    char *response = response_parser();
    print_full(response);
    return parse_http(response);
}