#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../../util/hex_to_int.h"

char *chunked(char *httpNoHeaders, size_t *outputLength)
{
    char *output;
    if ((output = malloc(sizeof(char) * 1)) == NULL)
        return NULL;
    *output = '\0';
    *outputLength = 1;
    while (1)
    {
        char *rawChunkLength;
        if ((rawChunkLength = malloc(sizeof(char) * 1)) == NULL)
        {
            free(output);
            return NULL;
        }
        *rawChunkLength = '\0';
        for (uint8_t i = 1; !(*httpNoHeaders == '\r' && *(httpNoHeaders + 1) == '\n'); i++, httpNoHeaders++)
        {
            if ((rawChunkLength = realloc(rawChunkLength, sizeof(char) * (i + 1))) == NULL)
            {
                free(rawChunkLength);
                free(output);
                return NULL;
            }
            *(rawChunkLength + i - 1) = *httpNoHeaders;
            *(rawChunkLength + i) = '\0';
        }
        uint32_t chunkLength = hex_to_int(rawChunkLength);
        free(rawChunkLength);
        if (chunkLength == 0)
            break;
        httpNoHeaders += 2;
        (*outputLength) += chunkLength;
        if ((output = realloc(output, sizeof(char) * (*outputLength))) == NULL)
        {
            free(output);
            return NULL;
        }
        strncat(output, httpNoHeaders, chunkLength);
        httpNoHeaders += chunkLength + 2;
    }
    return output;
}