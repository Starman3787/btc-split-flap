#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "util/http/http.h"

int8_t http_body_parser(Http *parsedHttp, char *rawBody, Header *headers, uint8_t headersLength)
{
    Header *contentTypeHeader = find_header(headers, headersLength, "content-type");
    char *contentTypeHeaderValues[1];
    char contentTypeHeaderValueOne[32];
    contentTypeHeaderValues[0] = contentTypeHeaderValueOne;
    split_header_values(contentTypeHeaderValues, contentTypeHeader->value, 1, 32, ';');
    printf("HEADER VALUES SPLIT: %s\n", contentTypeHeaderValues[0]);
    if (strcmp(contentTypeHeaderValues[0], "application/json") == 0)
    {
        puts("JSON");
        parsedHttp->responseBody.content_type = HEADER_CONTENT_TYPE_APPLICATION_JSON;
        parse_json(&(parsedHttp->responseBody.data.data_json), "rate", rawBody);
    }
    else if (strcmp(contentTypeHeaderValues[0], "text/plain") == 0)
    {
        parsedHttp->responseBody.content_type = HEADER_CONTENT_TYPE_TEXT_PLAIN;
        strncpy(parsedHttp->responseBody.data.data_text, rawBody, 1023);
    }
    else
    {
        return -1;
    }
    return 0;
}