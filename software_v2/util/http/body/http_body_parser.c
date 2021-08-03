#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "../http.h"

Body *http_body_parser(char *rawBody, Header **headers, uint8_t headersLength)
{
    Body *parsedBody = malloc(sizeof(Body) * 1);
    Header *transferEncodingHeader = find_header(headers, headersLength, "transfer-encoding");
    char *transferEncodingValue = transferEncodingHeader->value;
    if (*transferEncodingValue == NULL)
    {
        // transfer-encoding header not present
    }
    else
    {
        uint8_t transferEncodingValuesCount;
        char **transferEncodingHeaderValues = split_header_values(transferEncodingValue, &transferEncodingValuesCount, ',');
        size_t bodySize = 0;
        while (transferEncodingValuesCount--)
        {
            if (strcmp(*(transferEncodingHeaderValues + transferEncodingValuesCount), "chunked") == 0)
                rawBody = chunked(rawBody, &bodySize);
        }
    }
    Header *contentTypeHeader = find_header(headers, headersLength, "content-type");
    char *contentTypeValue = contentTypeHeader->value;
    uint8_t contentTypeValuesCount;
    char **contentTypeHeaderValues = split_header_values(contentTypeValue, &contentTypeValuesCount, ';');
    if (strcmp(*contentTypeHeaderValues, "application/json") == 0)
    {
        size_t json_size;
        parsedBody->content_type = HEADER_CONTENT_TYPE_APPLICATION_JSON;
        parsedBody->data.data_json = parse_json(rawBody, &json_size);
        parsedBody->data_size = json_size;
    } else if (strcmp(*contentTypeHeaderValues, "text/plain") == 0)
    {
        parsedBody->content_type = HEADER_CONTENT_TYPE_TEXT_PLAIN;
        parsedBody->data.data_text = rawBody;
        parsedBody->data_size = strlen(rawBody) + 1;
    }
    return parsedBody;
}