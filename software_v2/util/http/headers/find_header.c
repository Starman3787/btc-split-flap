#include <stdint.h>
#include <string.h>
#include "util/http/http.h"
#include "../util/to_lower_case.h"

int8_t find_header(Header *headerFound, Header *headers, uint8_t headersLength, char *key)
{
    while (headersLength--)
    {
        if (strcmp(headers[headersLength].key, key) == 0)
        {
            *headerFound = headers[headersLength];
            return 0;
        }
    }
    return -1;
}