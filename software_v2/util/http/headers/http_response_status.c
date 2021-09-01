#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "util/http/http.h"

char httpSupported[2][9] = {
    "HTTP/1.0",
    "HTTP/1.1"
};

/**
 * @brief Extracts the HTTP status from the raw response.
 * 
 * @param rawHttp A raw HTTP response.
 * @return uint16_t The 3 digit HTTP code.
 */
uint16_t http_response_status(char *rawHttp)
{
    for (uint8_t index = 0; index < strlen(httpSupported[0]); index++, rawHttp++)
        if ((*rawHttp != *(httpSupported[0] + index)) && (*rawHttp != *(httpSupported[1] + index)))
            return 0U;
    if (*rawHttp != ' ')
        return 0U;
    rawHttp++;
    char rawHttpStatus[4];
    strncpy(rawHttpStatus, rawHttp, 3);
    char *rawHttpStatusEnd = rawHttpStatus + 3;
    return strtoul(rawHttpStatus, &rawHttpStatusEnd, 10);
}