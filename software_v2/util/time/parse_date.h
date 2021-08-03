#ifndef PARSE_DATE_H
#define PARSE_DATE_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>

const char months[12][4] = {
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec"
};

int8_t convertMonth(const char *month)
{
    for (int8_t i = 0; i < sizeof(months) / 4; i++)
        if (strcmp(month, months[i]) == 0)
            return i;
    return NULL;
}

#endif