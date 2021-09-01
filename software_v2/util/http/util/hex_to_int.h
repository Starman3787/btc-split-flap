#ifndef HEX_TO_INT_H
#define HEX_TO_INT_H

#include <ctype.h>
#include <stdint.h>

uint8_t hex_char_to_int(char hex);
uint32_t hex_to_int(char *hex);

#endif