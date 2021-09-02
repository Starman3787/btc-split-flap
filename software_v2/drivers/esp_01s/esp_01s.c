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
    char command[128] = "AT+CIPSTART=";
    strcat(strcat(strcat(strcat(strcat(strcat(command, "\""), protocol), "\",\""), host), "\","), port);
    printf("%d\n", strlen(command));
    puts(command);
    write_full_uart(command);
    if (read_full_uart_and_expect(strcat(command, "\r\r\n"), 1000) == false)
    {
        puts("nope");
        return false;
    }
    if (read_full_uart_and_expect("CONNECT", 10000) == false)
        return false;
    if (read_full_uart_and_expect("\r\n\r\n", 1000) == false)
        return false;
    if (read_full_uart_and_expect("OK\r\n", 1000) == false)
        return false;
    return true;
}

bool send_cip_send_command(char *size, char *httpRequest)
{
    char command[128] = "AT+CIPSEND=";
    strcat(command, size);
    puts(command);
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
    return true;
}

int8_t response_parser(Http *response)
{
    uint8_t totalChunks = 0;
    size_t currentResponseSize = 1;
    find_pattern("+IPD,", 5);
    while (1)
    {
        totalChunks++;
        if (totalChunks > 10)
            return -1;
        uint16_t currentChunkLength;
        char currentCharacter = read_uart(1000U);
        print(currentCharacter);
        char rawChunkLength[5];
        uint8_t counter;
        for (counter = 0; currentCharacter != ':'; counter++)
        {
            if (counter > 4)
                return -1;
            rawChunkLength[counter] = currentCharacter;
            rawChunkLength[counter + 1] = '\0';
            currentCharacter = read_uart(1500U);
            print(currentCharacter);
        }
        currentChunkLength = strtoul(rawChunkLength, NULL, 10);
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
    *(fullRawResponse + currentResponseSize) = '\0';
    puts("added null characters");
    parse_http(response, fullRawResponse);
    puts("parsed http");
    free(fullRawResponse);
    puts("freed 3");
    Header *currentTimeString = find_header(response->headers, response->headersLength, "date");
    puts("got time");
    unix = parse_date(currentTimeString->value);
    puts(currentTimeString->value);
    return 0;
}

int8_t make_http_request(Http *response, char *protocol, char *host, char *port, char *size, char *httpRequest)
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