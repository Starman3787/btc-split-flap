#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../http.h"

/**
 * @brief Extracts the HTTP status from the raw response.
 * 
 * @param rawHttp A raw HTTP response.
 * @return uint16_t The 3 digit HTTP code.
 */
uint16_t http_response_status(char *rawHttp)
{
    char *httpSupported = "HTTP/1.1";
    for (uint8_t index = 0; index < strlen(httpSupported); index++, rawHttp++)
        if (*rawHttp != *(httpSupported + index))
            return 0U;
    if (*rawHttp != ' ')
        return 0U;
    rawHttp++;
    char rawHttpStatus[4];
    rawHttpStatus[0] = *(rawHttp++);
    rawHttpStatus[1] = *(rawHttp++);
    rawHttpStatus[2] = *(rawHttp++);
    rawHttpStatus[3] = '\0';
    char *rawHttpStatusEnd = rawHttpStatus + 3;
    return strtoul(rawHttpStatus, &rawHttpStatusEnd, 10);
}