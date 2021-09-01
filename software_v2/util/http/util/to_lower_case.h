#ifndef UTIL_HTTP_UTIL_TO_LOWER_CASE_H
#define UTIL_HTTP_UTIL_TO_LOWER_CASE_H
#include <ctype.h>

void to_lower_case(char *characters)
{
    for (; *characters; ++characters) 
        *characters = tolower(*characters);
}

#endif