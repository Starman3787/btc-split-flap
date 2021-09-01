#include <stdlib.h>
#include "util/http/http.h"

void free_json(Json ***jsonBody, size_t child_size)
{
    uint16_t child_elements = child_size / sizeof(Json *);
    for (uint16_t i = 0; i < child_elements; i++)
    {
        free((*(*jsonBody + i))->key);
        switch ((*(*jsonBody + i))->type)
        {
        case JSON_NUMBER:
            // number data is not a pointer, nothing to do here
            break;
        case JSON_STRING:
            free((*(*jsonBody + i))->data.json_string);
            break;
        case JSON_BOOLEAN:
            // boolean data is not a pointer, nothing to do here
            break;
        case JSON_ARRAY:
            free_json(&((*(*jsonBody + i))->data.json_array), (*(*jsonBody + i))->child_size);
            break;
        case JSON_OBJECT:
            free_json(&((*(*jsonBody + i))->data.json_object), (*(*jsonBody + i))->child_size);
            break;
        }
        free((*(*jsonBody + i)));
    }
    free(*jsonBody);
}