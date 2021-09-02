#include <stdint.h>
#include <ctype.h>
#include <stdio.h>
#include "util/http/http.h"
#include "drivers/uart/uart.h"

int8_t http_header_parser(Header *allHeaders, char *rawHttp, uint8_t *headerIndex, char **headersEnd)
{
    while (!(*rawHttp == '\r' && *(rawHttp + 1) == '\n'))
        INCREMENT_POINTER(rawHttp)
    INCREMENT_POINTER(rawHttp)
    INCREMENT_POINTER(rawHttp)
    for (*headerIndex = 0; *headerIndex < 16; (*headerIndex)++)
    {
        if (*rawHttp == '\r' && *(rawHttp + 1) == '\n')
            break;
        else if (*rawHttp == '\0')
            return -1;
        allHeaders[*headerIndex].key[0] = '\0';
        INCREMENT_POINTER(rawHttp)
        for (uint8_t keyLength = 0; *(rawHttp - 1) != ':' && keyLength < 62; keyLength++)
        {
            allHeaders[*headerIndex].key[keyLength] = (char)tolower(*(rawHttp - 1));
            allHeaders[*headerIndex].key[keyLength + 1] = '\0';
            INCREMENT_POINTER(rawHttp)
        }
        puts(allHeaders[*headerIndex].key);
        allHeaders[*headerIndex].value[0] = '\0';
        INCREMENT_POINTER(rawHttp)
        INCREMENT_POINTER(rawHttp)
        for (uint8_t valueLength = 0; !(*(rawHttp - 1) == '\r' && *rawHttp == '\n' && valueLength < 126); valueLength++)
        {
            allHeaders[*headerIndex].value[valueLength] = *(rawHttp - 1);
            allHeaders[*headerIndex].value[valueLength + 1] = '\0';
            INCREMENT_POINTER(rawHttp)
        }
        INCREMENT_POINTER(rawHttp)
        puts(allHeaders[*headerIndex].value);
    }
    *headersEnd = ++rawHttp;
    return 0;
}