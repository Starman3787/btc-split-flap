#include <stdlib.h>
#include "../http.h"

void free_http(Http **parsedHttp)
{
    free_headers(&((*parsedHttp)->headers), (*parsedHttp)->headersLength);
    free_body(&((*parsedHttp)->responseBody));
    free(*parsedHttp);
}