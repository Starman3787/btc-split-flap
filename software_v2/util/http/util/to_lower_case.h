#ifndef TO_LOWER_CASE_H
#define TO_LOWER_CASE_H
#include <ctype.h>

void to_lower_case(char *characters)
{
    for (; *characters; ++characters) 
        *characters = tolower(*characters);
}

#endif