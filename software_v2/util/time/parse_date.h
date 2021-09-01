#ifndef UTIL_PARSE_DATE_H_
#define UTIL_PARSE_DATE_H_

#include <stdint.h>
#include <string.h>
#include <stdio.h>

extern const char months[12][4];

int8_t convertMonth(const char *month);
time_t parse_date(char *date);

#endif