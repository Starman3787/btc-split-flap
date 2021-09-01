#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "util/http/http.h"
#include "util/delay/delay.h"
#include "util/time/parse_date.h"
#include "util/status/status.h"
#include "timers/systick/systick.h"
#include "drivers/uart/uart.h"
#include "drivers/esp_01s/esp_01s.h"

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

void response_parser(Http *response)
{
    uint8_t totalChunks = 0;
    size_t currentResponseSize = 1;
    find_pattern("+IPD,", 5);
    while (1)
    {
        totalChunks++;
        if (totalChunks > 10)
            break;
        uint16_t currentChunkLength;
        char currentCharacter = read_uart(1000U);
        print(currentCharacter);
        char *rawChunkLength = malloc(sizeof(char) * 2);
        uint8_t counter;
        for (counter = 0; currentCharacter != ':'; counter++)
        {
            rawChunkLength = realloc(rawChunkLength, sizeof(char) * (counter + 2));
            *(rawChunkLength + counter) = currentCharacter;
            *(rawChunkLength + counter + 1) = '\0';
            currentCharacter = read_uart(1500U);
            print(currentCharacter);
        }
        char *endOfRawChunkLengthString = rawChunkLength + counter + 1;
        currentChunkLength = strtoul(rawChunkLength, &endOfRawChunkLengthString, 10);
        free(rawChunkLength);
        uint16_t bodyCounter;
        for (bodyCounter = 0; bodyCounter < currentChunkLength; bodyCounter++)
        {   
            rawResponseChunks[totalChunks - 1].chunkData[bodyCounter] = read_uart(1500U);
            print(rawResponseChunks[totalChunks - 1].chunkData[bodyCounter]);
        }
        rawResponseChunks[totalChunks - 1].chunkData[currentChunkLength] = '\0';
        rawResponseChunks[totalChunks - 1].chunkLength = currentChunkLength;
        currentResponseSize += currentChunkLength;
        char firstChar = read_uart(1000U);
        if (firstChar == '\r')
        {
            if (read_full_uart_and_expect("\n+IPD,", 10000UL) != true)
            {
                puts("NOT TRUE");
                break;
            }
        }
        else if (firstChar == 'C')
        {
            find_pattern("LOSED\r\n", 7);
            break;
        }
        else
        {
            print('C');
        }
    }
    puts("END OF RESPONSE");
    char *fullRawResponse = malloc(sizeof(char) * currentResponseSize);
    *fullRawResponse = '\0';
    puts("143");
    for (uint8_t i = 0; i < totalChunks; i++)
    {
        printf("%d\n", i);
        strncat(fullRawResponse, rawResponseChunks[i].chunkData, rawResponseChunks[i].chunkLength);
        puts("concat'd");
    }
    puts("freed 2");
    *(fullRawResponse + currentResponseSize) = '\0';
    puts("added null characters");
    parse_http(fullRawResponse, response);
    puts("parsed http");
    free(fullRawResponse);
    puts("freed 3");
    Header *currentTimeString = find_header(response->headers, response->headersLength, "date");
    puts("got time");
    unix = parse_date(currentTimeString->value);
    puts(currentTimeString->value);
}

int8_t make_http_request(char *protocol, char *host, char *port, char *size, char *httpRequest, Http *response)
{
    if (send_cip_start_command(protocol, host, port) != true)
    {
        status_error(true);
        return -1;
    }
    if (send_cip_send_command(size, httpRequest) != true)
    {
        status_error(true);
        return -1;
    }
    response_parser(response);
    return 0;
}