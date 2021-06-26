#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "./wifi.h"

void read_full_uart_until_json_property_match(char *property, size_t propertyLength, char *value, size_t valueLength, uint8_t *counter)
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
                {
                    endOfValue = true;
                }
                else
                {
                    value[*counter] = currentValue;
                    *counter += 1;
                }
            }
        } while (!endOfValue && (*counter < valueLength));
    }
    else
    {
        print('5');
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

void init_wifi(void)
{
    write_led(1, true);
    delay_ms(5000);
    if (send_test_command(TEST) == false)
        write_led(2, true);
    else
        write_led(0, true);
    if (send_cip_start_command("TCP", "rest.coinapi.io", "80") == false)
        write_led(2, true);
    if (send_cip_send_command("143", HTTP_REQUEST) == false)
        write_led(2, true);
}