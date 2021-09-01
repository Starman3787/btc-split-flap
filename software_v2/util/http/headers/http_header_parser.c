#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "util/http/http.h"

/**
 * @brief Extracts the headers from a raw HTTP response, and returns a pointer to an array of pointers to Header structures.
 * 
 * @param *rawHttp A pointer to the zeroth index of the raw HTTP response.
 * @param *headerIndex A pointer which will contain the value of the number of headers extracted from this response.
 * @return struct Header** 
 */
Header **http_header_parser(char *rawHttp, uint8_t *headerIndex, char **headersEnd)
{
    Header **allHeaders;
    puts("17");
    if ((allHeaders = malloc(sizeof(Header *) * 1)) == NULL)
        return NULL;
    puts("20");
    while (!(*rawHttp == '\r' && *(rawHttp + 1) == '\n'))
        rawHttp++;
    rawHttp += 2;
    *headerIndex = 0;
    puts("25");
    while (1)
    {
        puts("28");
        if (*rawHttp == '\r' && *(rawHttp + 1) == '\n')
            break;
        puts("31");
        if ((allHeaders = realloc(allHeaders, sizeof(Header *) * (*headerIndex + 1))) == NULL)
        {
            free_headers(&allHeaders, *headerIndex);
            return NULL;
        }
        puts("37");
        char *key;
        if ((key = malloc(sizeof(char))) == NULL)
        {
            free_headers(&allHeaders, *headerIndex);
            return NULL;
        }
        *key = '\0';
        puts("45");
        uint8_t keyLength;
        rawHttp++;
        puts("48");
        for (keyLength = 1; *(rawHttp - 1) != ':'; keyLength++)
        {
            puts("51");
            if ((key = realloc(key, sizeof(char) * (keyLength + 1))) == NULL)
            {
                free(key);
                free_headers(&allHeaders, *headerIndex);
                return NULL;
            }
            puts("58");
            *(key + keyLength - 1) = (char)tolower(*(rawHttp - 1));
            *(key + keyLength) = '\0';
            rawHttp++;
        }
        char *value;
        puts("64");
        if ((value = malloc(sizeof(char))) == NULL)
        {
            free(key);
            free_headers(&allHeaders, *headerIndex);
            return NULL;
        }
        puts("71");
        *value = '\0';
        uint8_t valueLength;
        rawHttp += 2;
        puts("75");
        for (valueLength = 1; !(*(rawHttp - 1) == '\r' && *rawHttp == '\n'); valueLength++)
        {
            puts("78");
            if ((value = realloc(value, sizeof(char) * (valueLength + 1))) == NULL)
            {
                free(value);
                free(key);
                free_headers(&allHeaders, *headerIndex);
                return NULL;
            }
            puts("86");
            *(value + valueLength - 1) = *(rawHttp - 1);
            *(value + valueLength) = '\0';
            rawHttp++;
        }
        puts("91");
        if ((*(allHeaders + *headerIndex) = malloc(sizeof(Header))) == NULL)
        {
            free(value);
            free(key);
            free_headers(&allHeaders, *headerIndex);
            return NULL;
        }
        puts("99");
        (*(allHeaders + *headerIndex))->key = key;
        (*(allHeaders + *headerIndex))->value = value;
        rawHttp++;
        (*headerIndex)++;
    }
    puts("105");
    *headersEnd = ++rawHttp;
    return allHeaders;
}