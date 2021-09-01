#ifndef FIND_HEADER_C
#define FIND_HEADER_C

#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include "util/http/http.h"
#include "../util/to_lower_case.h"

Header *find_header(Header **headers, uint8_t headersLength, char *key)
{
    for (; headersLength--; headers++)
        if (strcmp((*headers)->key, key) == 0)
            return *headers;
    return NULL;
}

#endif