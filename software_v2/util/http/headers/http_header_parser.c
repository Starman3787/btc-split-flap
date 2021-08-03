#include <stdint.h>
#include <stdlib.h>
#include "../http.h"

/**
 * @brief Extracts the headers from a raw HTTP response, and returns a pointer to an array of pointers to Header structures.
 * 
 * @param *rawHttp A pointer to the zeroth index of the raw HTTP response.
 * @param *headerIndex A pointer which will contain the value of the number of headers extracted from this response.
 * @return struct Header** 
 */
Header **http_header_parser(char *rawHttp, uint8_t *headerIndex, char **headersEnd)
{
    Header **allHeaders = malloc(sizeof(Header *) * 1);
    while (!(*rawHttp == '\r' && *(rawHttp + 1) == '\n'))
        rawHttp++;
    rawHttp += 2;
    *headerIndex = 0;
    while (1)
    {
        if (*rawHttp == '\r' && *(rawHttp + 1) == '\n')
            break;
        allHeaders = realloc(allHeaders, sizeof(Header *) * (*headerIndex + 1));
        char *key = malloc(sizeof(char));
        *key = '\0';
        uint8_t keyLength;
        rawHttp++;
        for (keyLength = 1; *(rawHttp - 1) != ':'; keyLength++)
        {
            key = realloc(key, sizeof(char) * (keyLength + 1));
            *(key + keyLength - 1) = *(rawHttp - 1);
            *(key + keyLength) = '\0';
            rawHttp++;
        }
        char *value = malloc(sizeof(char));
        *value = '\0';
        uint8_t valueLength;
        rawHttp += 2;
        for (valueLength = 1; !(*(rawHttp - 1) == '\r' && *rawHttp == '\n'); valueLength++)
        {
            value = realloc(value, sizeof(char) * (valueLength + 1));
            *(value + valueLength - 1) = *(rawHttp - 1);
            *(value + valueLength) = '\0';
            rawHttp++;
        }
        allHeaders[*headerIndex] = malloc(sizeof(Header));
        allHeaders[*headerIndex]->key = key;
        allHeaders[*headerIndex]->value = value;
        rawHttp++;
        (*headerIndex)++;
    }
    *headersEnd = ++rawHttp;
    return allHeaders;
}