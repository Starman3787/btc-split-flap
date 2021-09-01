#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "util/http/http.h"

void parse_http(char *rawHttp, Http *parsedHttp)
{
    puts("parse_http init alloc");
    puts("parse_http init alloc'd");
    parsedHttp->statusCode = http_response_status(rawHttp);
    printf("found http status of %d\n", parsedHttp->statusCode);
    uint8_t headersLength;
    char *headersEnd;
    puts("parsing headers");
    parsedHttp->headers = http_header_parser(rawHttp, &headersLength, &headersEnd);
    printf("found and parsed %d headers\n", headersLength);
    parsedHttp->headersLength = headersLength;
    headersEnd++;
    puts("parsing body now");
    parsedHttp->responseBody = http_body_parser(headersEnd, parsedHttp->headers, headersLength);
    puts("body parsed");
}