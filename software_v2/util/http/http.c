#include <stdlib.h>
#include <stdint.h>
#include "./http.h"

Http *parse_http(char *rawHttp)
{
    Http *parsedHttp = malloc(sizeof(Http));
    parsedHttp->statusCode = http_response_status(rawHttp);
    uint8_t headersLength;
    char *headersEnd;
    parsedHttp->headers = http_header_parser(rawHttp, &headersLength, &headersEnd);
    parsedHttp->headersLength = headersLength;
    headersEnd++;
    parsedHttp->responseBody = http_body_parser(headersEnd, parsedHttp->headers, &headersLength);
    return parsedHttp;
}