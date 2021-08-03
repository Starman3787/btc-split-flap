#include <stdint.h>
#include <stdlib.h>
#include <string.h>

char *chunked(char *httpNoHeaders, size_t *outputLength)
{
    char *output = malloc(sizeof(char) * 1);
    *output = '\0';
    *outputLength = 1;
    while (1)
    {
        char *rawChunkLength = malloc(sizeof(char) * 1);
        *rawChunkLength = '\0';
        for (uint8_t i = 1; !(*httpNoHeaders == '\r' && *(httpNoHeaders + 1) == '\n'); i++, httpNoHeaders++)
        {
            rawChunkLength = realloc(rawChunkLength, sizeof(char) * (i + 1));
            *(rawChunkLength + i - 1) = *httpNoHeaders;
            *(rawChunkLength + i) = '\0';
        }
        uint32_t chunkLength = hex_to_int(rawChunkLength);
        free(rawChunkLength);
        if (chunkLength == 0)
            break;
        httpNoHeaders += 2;
        (*outputLength) += chunkLength;
        output = realloc(output, sizeof(char) * (*outputLength));
        strncat(output, httpNoHeaders, chunkLength);
        httpNoHeaders += chunkLength + 2;
    }
    return output;
}