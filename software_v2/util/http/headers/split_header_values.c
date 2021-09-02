#include <stdint.h>
#include <stdlib.h>
#include "util/http/http.h"

int8_t split_header_values(char *contentTypeHeaderValues[], char *headerValue, uint8_t maxValues, uint8_t maxValueLength, const char splitBy)
{
    for (uint8_t i = 0; i < maxValues; i++)
    {
        contentTypeHeaderValues[i][0] = '\0';
        for (uint8_t valueLength = 0; *headerValue != splitBy && *headerValue != '\0' && valueLength < maxValueLength - 2; valueLength++, headerValue++)
        {
            contentTypeHeaderValues[i][valueLength] = *headerValue;
            contentTypeHeaderValues[i][valueLength + 1] = '\0';
        }
        if (*headerValue == '\0')
            break;
        headerValue += 2;
    }
    return 0;
}