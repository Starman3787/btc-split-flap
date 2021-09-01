#include <stdint.h>
#include <stdlib.h>
#include "util/http/http.h"

int8_t split_header_values(char **contentTypeHeaderValues, char *headerValue, uint8_t maxValues, const char splitBy)
{
    for (uint8_t i = 0; i < maxValues; i++)
    {
        char *value;
        if ((value = malloc(sizeof(char) * 1)) == NULL)
            return -1;
        *value = '\0';
        for (uint8_t valueLength = 1; *headerValue != splitBy && *headerValue != '\0'; valueLength++, headerValue++)
        {
            if ((value = realloc(value, sizeof(char) * (valueLength + 1))) == NULL)
            {
                free(value);
                return -1;
            }
            *(value + valueLength - 1) = *headerValue;
            *(value + valueLength) = '\0';
        }
        contentTypeHeaderValues[i] = value;
        if (*headerValue == '\0')
            break;
        headerValue += 2;
    }
    return 0;
}