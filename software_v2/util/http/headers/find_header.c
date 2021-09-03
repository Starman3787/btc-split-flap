#ifndef FIND_HEADER_C
#define FIND_HEADER_C

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "util/http/http.h"
#include "../util/to_lower_case.h"

Header *find_header(Header *headers, uint8_t headersLength, char *key)
{
    printf("FINDING %s\n", key);
    for (uint8_t i = 0; headersLength--; i++)
    {
        puts(headers[i].key);
        if (strcmp(headers[i].key, key) == 0)
            return &(headers[i]);
    }
    return NULL;
}

#endif