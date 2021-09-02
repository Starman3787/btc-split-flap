#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
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
        size_t json_size;
        parsedHttp->responseBody.content_type = HEADER_CONTENT_TYPE_APPLICATION_JSON;
        parsedHttp->responseBody.data.data_json = parse_json(rawBody, &json_size);
        parsedHttp->responseBody.data_size = json_size;
    }
    else if (strcmp(contentTypeHeaderValues[0], "text/plain") == 0)
    {
        parsedHttp->responseBody.content_type = HEADER_CONTENT_TYPE_TEXT_PLAIN;
        parsedHttp->responseBody.data.data_text = rawBody;
        parsedHttp->responseBody.data_size = strlen(rawBody) + 1;
    }
    else
    {
        return -1;
    }
    return 0;
}