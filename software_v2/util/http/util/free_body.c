#include <stdlib.h>
#include "../http.h"

void free_body(Body **parsedBody)
{
    switch ((*parsedBody)->content_type)
    {
    case HEADER_CONTENT_TYPE_TEXT_PLAIN:
    free((*parsedBody)->data.data_text);
    break;
    case HEADER_CONTENT_TYPE_APPLICATION_JSON:
    free_json(&((*parsedBody)->data.data_json), (*parsedBody)->data_size);
    break;
    }
    free(*parsedBody);
}