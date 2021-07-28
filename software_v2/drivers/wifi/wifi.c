/**
 * @file wifi.c
 * @author Starman
 * @brief Wifi functions for setting up the module and making requests
 * @version 0.1
 * @date 2021-07-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "./wifi.h"

/**
 * @brief Reads the incoming http json body data from the uart until a property is found, then will read and return the value
 * 
 * @param property The http json body property to find
 * @param propertyLength The length of the http json body property
 * @param value A pointer to a variable which should store the value
 * @param valueLength The maximum length of the value to retrieve
 * @param counter A pointer to a counter, so the actual length of the value can be determined
 * @return true All expected values were read when expected
 * @return false A character was expected, but was not returned
 */
bool read_full_uart_until_json_property_match(char *property, size_t propertyLength, char *value, size_t valueLength, uint8_t *counter)
{
    find_pattern(property, propertyLength);
    if (read_full_uart_and_expect(":"))
    {
        bool endOfValue = false;
        *counter = 0;
        do
        {
            char currentValue = read_uart();
            if (currentValue != ' ')
            {
                // THIS WILL STOP MID-STRING IF THERE IS A COMMA PRESENT
                // MUST BE MODIFIED
                // ONLY WORKS WITH NUMBERS FOR NOW
                if (currentValue == ',')
                    endOfValue = true;
                else
                    value[(*counter)++] = currentValue;
            }
        } while (!endOfValue && (*counter < valueLength));
        return true;
    }
    else
    {
        return false;
    }
}

void find_pattern(char *pattern, size_t patternLength)
{
    uint8_t matchingChars = 0;
    while (matchingChars != patternLength)
    {
        char currentValue = read_uart();
        print(currentValue);
        if (currentValue == *(pattern + matchingChars))
            matchingChars++;
        else
            matchingChars = 0U;
    }
}

/**
 * @brief Send the test command to the ESP8266 wifi module, and check "OK" is returned
 * 
 * @param command The test command to send over uart
 * @return true A response of "OK" was returned
 * @return false An unexpected response was returned
 */
bool send_test_command(char *command)
{
    write_full_uart(command);
    if (read_full_uart_and_expect(strcat(command, "\r\n")) == false)
        return false;
    if (read_full_uart_and_expect("\r\r\n\r\n") == false)
        return false;
    if (read_full_uart_and_expect("OK\r\n") == false)
        return false;
    return true;
}

/**
 * @brief Sends the command to start the http request
 * 
 * @param protocol "TCP" or "UDP"
 * @param host The host to send the request to
 * @param port The port to send the request to
 * @return true The command was sent successfully and the expected values were returned
 * @return false An unexpected response was returned
 */
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
    write_full_uart(command);
    if (read_full_uart_and_expect(strcat(command, "\r\r\n")) == false)
        return false;
    if (read_full_uart_and_expect("CONNECT\r\n") == false)
        return false;
    if (read_full_uart_and_expect("\r\n") == false)
        return false;
    if (read_full_uart_and_expect("OK\r\n") == false)
        return false;
    free(command);
    return true;
}

/**
 * @brief Sends the command to make the http request
 * 
 * @param size The size of the request
 * @param httpRequest The http request
 * @return true The command was sent successfully and the expected values were returned
 * @return false An unexpected response was returned
 */
bool send_cip_send_command(char *size, char *httpRequest)
{
    char *command = malloc(strlen("AT+CIPSEND=") + strlen(size) + 1);
    command[0] = '\0';
    strcat(command, "AT+CIPSEND=");
    strcat(command, size);
    write_full_uart(command);
    if (read_full_uart_and_expect(strcat(command, "\r\r\n")) == false)
        return false;
    if (read_full_uart_and_expect("\r\n") == false)
        return false;
    if (read_full_uart_and_expect("OK\r\n") == false)
        return false;
    if (read_full_uart_and_expect(">") == false)
        return false;
    write_full_uart(httpRequest);
    free(command);
    return true;
}

/**
 * @brief Combines other functions to make a request to the API to fetch the current price of Bitcoin
 * 
 * @return uint32_t The price
 */
uint32_t fetch_price(void)
{
    if (send_cip_start_command(PROTOCOL, HOST, PORT) == false)
    {
        write_led(2, true);
        return 0U;
    }
    if (send_cip_send_command(REQUEST_SIZE, REQUEST) == false)
    {
        write_led(2, true);
        return 0U;
    }
    char value[32];
    uint8_t length;
    if (read_full_uart_until_json_property_match("\"rate\"", 6, value, 32, &length) == true)
    {
        find_pattern("CLOSED\r\n", 8);
        char *finalValue = malloc(length + 1);
        strncpy(finalValue, value, length);
        finalValue[length] = '\0';
        char *endOfFinalValueString = finalValue + length + 1;
        uint32_t price = strtoul(finalValue, &endOfFinalValueString, 10);
        print_full(finalValue);
        free(finalValue);
        status_ok(true);
        return price;
    }
    else
    {
        status_ok(false);
        status_error(true);
        return 0U;
    }
}

/**
 * @brief Initialises the wifi module
 * 
 */
void init_wifi(void)
{
    status_loading(true);
    find_pattern("WIFI GOT IP\r\n", 13);
    delay_ms(10000);
    if (send_test_command(TEST) == false)
        status_error(true);
    else
        status_ok(true);
}