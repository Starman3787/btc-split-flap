#include <stdint.h>
#include <math.h>
#include <string.h>
#include "./hex_to_int.h"

/**
 * @brief Converts hexidecimal to its denary equivalent.
 * 
 * @param hex 
 * @return uint32_t 
 */
uint32_t hex_to_int(char *hex)
{
    uint32_t total = 0;
    while (*hex != '\0')
        total += hex_char_to_int(*hex) * pow(16, (strlen(hex++) - 1));
    return total;
}