#include <stdlib.h>
#include "util/http/http.h"

void free_headers(Header ***headers, uint8_t headersLength)
{
    for (uint8_t i = 0; i < headersLength; i++)
    {
        free((*(*headers + i))->key);
        free((*(*headers + i))->value);
        free(*(*headers + i));
    }
    free(*headers);
}