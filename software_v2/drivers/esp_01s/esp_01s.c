#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "headers/stm32f767xx.h"
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
    write_full_uart("g");
    if (read_full_uart_and_expect("g\r\n", 10000U) != 0)
    {
#ifdef SYSTEM_DEBUG__
        printf("FAILED AT LINE %d IN FILE %s\n", __LINE__, __FILE__);
#endif
        return -1;
    }
    return 0;
}

// waits until the module has connected to wifi
// can fail if the test command is unsuccessful
int8_t init_esp_01s(void)
{
    UART4_Interrupts_Disable();
    for (uint8_t attempts = 0; read_uart(1000U) != 'd'; attempts++)
    {
        if (attempts >= 100)
        {
            UART4_Interrupts_Enable();
            return -1;
        }
    }
    read_uart(100U);
    read_uart(100U);
#ifdef SYSTEM_DEBUG__
    puts("WIFI GOT IP");
#endif
    if (esp_01s_test() != 0)
    {
#ifdef SYSTEM_DEBUG__
        printf("FAILED AT LINE %d IN FILE %s\n", __LINE__, __FILE__);
#endif
        UART4_Interrupts_Enable();
        return -1;
    }
    UART4_Interrupts_Enable();
    return 0;
}

// listens to the UART and parses the response
// will also use the time from the "date" header to calibrate the system time
int8_t response_parser(Http *response, char *jsonProperty)
{
    char rawResponse[14601];
    rawResponse[0] = '\0';
    uint16_t index = 0;
    while (1)
    {
        int16_t currentChunkLength = 0;
        char currentCharacter = read_uart(1000U);
        uint8_t counter;
        for (counter = 0; currentCharacter != ':'; counter++)
        {
            if (counter > 6)
            {
#ifdef SYSTEM_DEBUG__
                printf("FAILED AT LINE %d IN FILE %s\n", __LINE__, __FILE__);
#endif
                return -1;
            }
            currentChunkLength *= 10;
            currentChunkLength += (currentCharacter - '0');
            currentCharacter = read_uart(1500U);
        }
        int16_t bodyCounter;
        for (bodyCounter = 0; bodyCounter < currentChunkLength; bodyCounter++, index++)
        {
            if (index > 14599)
            {
#ifdef SYSTEM_DEBUG__
                printf("FAILED AT LINE %d IN FILE %s\n", __LINE__, __FILE__);
#endif
                return -1;
            }
            rawResponse[index] = read_uart(1500U);
            if (rawResponse[index] == '\0')
            {
#ifdef SYSTEM_DEBUG__
                printf("FAILED AT LINE %d IN FILE %s\n", __LINE__, __FILE__);
#endif
                return -1;
            }
            rawResponse[index + 1] = '\0';
        }
        char firstChar = read_uart(1000U);
        if (firstChar == 'C')
        {
            if (find_pattern("LOSED\r\n", 7, 20000U) != 0)
            {
#ifdef SYSTEM_DEBUG__
                printf("FAILED AT LINE %d IN FILE %s\n", __LINE__, __FILE__);
#endif
                return -1;
            }
            break;
        }
    }
    if (parse_http(response, jsonProperty, rawResponse) != 0)
    {
#ifdef SYSTEM_DEBUG__
        printf("FAILED AT LINE %d IN FILE %s\n", __LINE__, __FILE__);
#endif
        return -1;
    }
    Header currentTimeString;
    if (find_header(&currentTimeString, response->headers, response->headersLength, "date") != 0)
    {
#ifdef SYSTEM_DEBUG__
        printf("FAILED AT LINE %d IN FILE %s\n", __LINE__, __FILE__);
#endif
        return -1;
    }
    time_t timestamp;
    parse_date(&timestamp, currentTimeString.value);
    unix = timestamp;
    // clean up UART
    read_uart(100);
    return 0;
}

// triggers a HTTP request
int8_t make_http_request(Http *response, char *jsonProperty)
{
    UART4_Interrupts_Disable();
    write_full_uart("m");
    if (read_full_uart_and_expect("s\r\n", 30000U) != 0)
    {
        UART4_Interrupts_Enable();
#ifdef SYSTEM_DEBUG__
        printf("FAILED AT LINE %d IN FILE %s\n", __LINE__, __FILE__);
#endif
        return -1;
    }
    if (response_parser(response, jsonProperty) != 0)
    {
        UART4_Interrupts_Enable();
#ifdef SYSTEM_DEBUG__
        printf("FAILED AT LINE %d IN FILE %s\n", __LINE__, __FILE__);
#endif
        return -1;
    }
    UART4_Interrupts_Enable();
    return 0;
}