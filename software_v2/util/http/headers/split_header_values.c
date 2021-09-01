#include <stdint.h>
#include <stdlib.h>

char **split_header_values(char *headerValue, uint8_t *values, const char splitBy)
{
    char **headerValues;
    if ((headerValues = malloc(sizeof(char *) * 1)) == NULL)
        return NULL;
    *values = 0;
    while (1)
    {
        if ((headerValues = realloc(headerValues, sizeof(char *) * ((*values) + 1))) == NULL)
        {
            while ((*values)--)
                free((headerValues + *values));
            free(headerValues);
            return NULL;
        }
        char *value;
        if ((value = malloc(sizeof(char) * 1)) == NULL)
            return NULL;
        *value = '\0';
        uint8_t valueLength;
        for (valueLength = 1; *headerValue != splitBy && *headerValue != '\0'; valueLength++, headerValue++)
        {
            if ((value = realloc(value, sizeof(char) * (valueLength + 1))) == NULL)
            {
                free(value);
                while ((*values)--)
                    free((headerValues + *values));
                return NULL;
            }
            *(value + valueLength - 1) = *headerValue;
            *(value + valueLength) = '\0';
        }
        *(headerValues + *values) = value;
        (*values)++;
        if (*headerValue == '\0')
            break;
        headerValue += 2;
    }
    return headerValues;
}