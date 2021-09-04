#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "util/http/http.h"

char httpSupported[2][9] = {
    "HTTP/1.0",
    "HTTP/1.1"
};

int8_t http_response_status(uint16_t *responseStatus, char *rawHttp)
{
    for (uint8_t i = 0; i < strlen(httpSupported[0]); i++)
    {
        if ((*rawHttp != *(httpSupported[0] + i)) && (*rawHttp != *(httpSupported[1] + i)))
            return -1;
        INCREMENT_POINTER(rawHttp)
    }
    if (*rawHttp != ' ')
        return -1;
    INCREMENT_POINTER(rawHttp)
    char rawHttpStatus[4];
    strncpy(rawHttpStatus, rawHttp, 3);
    *responseStatus = strtoul(rawHttpStatus, NULL, 10);
    return 0;
}