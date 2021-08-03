#include <stdint.h>
#include <stdlib.h>

char **split_header_values(char *headerValue, uint8_t *values, char splitBy)
{
    char **headerValues = malloc(sizeof(char *) * 1);
    *values = 0;
    while (1)
    {
        headerValues = realloc(headerValues, sizeof(char *) * ((*values) + 1));
        char *value = malloc(sizeof(char) * 1);
        *value = '\0';
        uint8_t valueLength;
        for (valueLength = 1; *headerValue != splitBy && *headerValue != '\0'; valueLength++, headerValue++)
        {
            value = realloc(value, sizeof(char) * (valueLength + 1));
            *(value + valueLength - 1) = *headerValue;
            *(value + valueLength) = '\0';
        }
        headerValues[*values] = value;
        (*values)++;
        if (*headerValue == '\0')
            break;
        headerValue += 2;
    }
    return headerValues;
}