#include <string.h>
#include <stdint.h>
#include "util/http/http.h"

int8_t http_body_parser(Http *parsedHttp, char *jsonProperty, char *rawBody, Header *headers, uint8_t headersLength)
{
    Header contentTypeHeader;
    if (find_header(&contentTypeHeader, headers, headersLength, "content-type") != 0)
        return -1;
    char *contentTypeHeaderValues[1];
    char contentTypeHeaderValueOne[32];
    contentTypeHeaderValues[0] = contentTypeHeaderValueOne;
    if (split_header_values(contentTypeHeaderValues, contentTypeHeader.value, 1, 32, ';') != 0)
        return -1;
    if (strcmp(contentTypeHeaderValues[0], "application/json") == 0)
    {
        parsedHttp->responseBody.content_type = HEADER_CONTENT_TYPE_APPLICATION_JSON;
        if (parse_json(&(parsedHttp->responseBody.data.data_json), jsonProperty, rawBody) != 0)
            return -1;
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