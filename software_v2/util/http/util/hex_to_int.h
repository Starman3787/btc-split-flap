#ifndef HEX_TO_INT_H
#define HEX_TO_INT_H

#include <ctype.h>
#include <stdint.h>

uint8_t hex_char_to_int(char hex)
{
    if (hex >= '0' && hex <= '9')
        return hex - '0';
    if (hex >= 'A' && hex <= 'F')
        return hex - 'A' + 10;
    return 0;
}

#endif