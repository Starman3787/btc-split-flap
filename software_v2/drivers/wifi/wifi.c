#include <stdint.h>

void init_wifi(void)
{
    char current[] = "AT";
    for (uint16_t i = 0; i < sizeof current; i++)
        write_uart(current[i]);

    char read_value = read_uart();
    // breakpoint here
}