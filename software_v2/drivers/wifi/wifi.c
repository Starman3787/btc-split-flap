#include <stdbool.h>
#include <stdint.h>
#include <string.h>

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

bool send_cip_start_command(char *protocol, char *host, uint8_t port)
{
    
}

void init_wifi(void)
{
    write_led(1, true);
    delay_ms(5000);
    if (send_test_command("AT") == false)
        write_led(2, true);
    else
        write_led(0, true);
}