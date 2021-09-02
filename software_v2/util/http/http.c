#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "util/http/http.h"

void parse_http(Http *parsedHttp, char *rawHttp)
{
    puts("parse_http init alloc");
    puts("parse_http init alloc'd");
    parsedHttp->statusCode = http_response_status(rawHttp);
    printf("found http status of %d\n", parsedHttp->statusCode);
    uint8_t headersLength;
    char *headersEnd;
    puts("parsing headers");
    http_header_parser(parsedHttp->headers, rawHttp, &headersLength, &headersEnd);
    printf("found and parsed %d headers\n", headersLength);
    printf("HEADER 0 == %s\n", parsedHttp->headers[0].key);
    parsedHttp->headersLength = headersLength;
    headersEnd++;
    puts("parsing body now");
    http_body_parser(parsedHttp, headersEnd, parsedHttp->headers, headersLength);
    puts("body parsed");
}