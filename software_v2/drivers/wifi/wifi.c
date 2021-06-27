#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "./wifi.h"

bool read_full_uart_until_json_property_match(char *property, size_t propertyLength, char *value, size_t valueLength, uint8_t *counter)
{
    uint8_t matchingChars = 0;
    while (matchingChars != propertyLength)
    {
        char currentValue = read_uart();
        if (currentValue == property[matchingChars])
            matchingChars++;
        else
            matchingChars = 0U;
    }
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

uint32_t fetch_price(void)
{
    if (send_cip_start_command(PROTOCOL, HOST, PORT) == false)
        write_led(2, true);
    if (send_cip_send_command(REQUEST_SIZE, REQUEST) == false)
        write_led(2, true);
    char value[32];
    uint8_t length;
    if (read_full_uart_until_json_property_match("\"rate\"", 6, value, 32, &length) == true)
    {
        char *finalValue = malloc(length + 1);
        strncpy(finalValue, value, length);
        finalValue[length] = '\0';
        char *pointer = finalValue + length + 1;
        uint32_t price = strtoul(finalValue, &pointer, 10);
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

void init_wifi(void)
{
    status_loading(true);
    delay_ms(5000);
    if (send_test_command(TEST) == false)
        status_error(true);
    else
        status_ok(true);
    uint32_t price = fetch_price();
}