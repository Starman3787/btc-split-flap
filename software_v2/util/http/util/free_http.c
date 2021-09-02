#include <stdlib.h>
#include "util/http/http.h"

void free_http(Http *parsedHttp)
{
    free_body(&(parsedHttp->responseBody));
}