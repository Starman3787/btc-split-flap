#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "util/http/http.h"

int8_t parse_http(Http *parsedHttp, char *jsonProperty, char *rawHttp)
{
    if (http_response_status(&(parsedHttp->statusCode), rawHttp) != 0)
    {
#ifdef SYSTEM_DEBUG__
        printf("FAILED AT LINE %d IN FILE %s\n", __LINE__, __FILE__);
#endif
        return -1;
    }
    uint8_t headersLength;
    char *headersEnd;
    if (http_header_parser(parsedHttp->headers, rawHttp, &headersLength, &headersEnd) != 0)
    {
#ifdef SYSTEM_DEBUG__
        printf("FAILED AT LINE %d IN FILE %s\n", __LINE__, __FILE__);
#endif
        return -1;
    }
    parsedHttp->headersLength = headersLength;
    headersEnd++;
    if (http_body_parser(parsedHttp, jsonProperty, headersEnd, parsedHttp->headers, headersLength) != 0)
    {
#ifdef SYSTEM_DEBUG__
        printf("FAILED AT LINE %d IN FILE %s\n", __LINE__, __FILE__);
#endif
        return -1;
    }
    return 0;
}