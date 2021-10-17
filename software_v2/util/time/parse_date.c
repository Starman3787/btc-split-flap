#ifndef PARSE_DATE_C
#define PARSE_DATE_C

#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "util/http/http.h"
#include "./parse_date.h"

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
    return 0;
}

// parses a HTTP "Date" header, specified here: https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Date
// to UNIX (seconds)
int8_t parse_date(time_t *timeOutput, char *date)
{
    struct tm t;
    t.tm_isdst = 0;
    INCREMENT_POINTER(date)
    INCREMENT_POINTER(date)
    INCREMENT_POINTER(date)
    INCREMENT_POINTER(date)
    char mday[3];
    mday[0] = '\0';
    INCREMENT_POINTER(date)
    for (uint8_t i = 0; *date != ' '; i++)
    {
        if (i >= 2)
            return -1;
        mday[i] = *date;
        mday[i + 1] = '\0';
        INCREMENT_POINTER(date)
    }
    t.tm_mday = atoi(mday);
    char mon[4];
    mon[3] = '\0';
    INCREMENT_POINTER(date)
    for (uint8_t i = 0; *date != ' '; i++)
    {
        mon[i] = *date;
        INCREMENT_POINTER(date)
    }
    t.tm_mon = convertMonth(mon);
    char year[5];
    year[0] = '\0';
    INCREMENT_POINTER(date)
    for (uint8_t i = 0; *date != ' '; i++)
    {
        if (i >= 4)
            return -1;
        year[i] = *date;
        year[i + 1] = '\0';
        INCREMENT_POINTER(date)
    }
    t.tm_year = atoi(year) - 1900;
    INCREMENT_POINTER(date)
    char hours[3];
    hours[0] = *date;
    INCREMENT_POINTER(date)
    hours[1] = *date;
    INCREMENT_POINTER(date)
    hours[2] = '\0';
    t.tm_hour = atoi(hours);
    INCREMENT_POINTER(date)
    char minutes[3];
    minutes[0] = *date;
    INCREMENT_POINTER(date)
    minutes[1] = *date;
    INCREMENT_POINTER(date)
    minutes[2] = '\0';
    t.tm_min = atoi(minutes);
    INCREMENT_POINTER(date)
    char seconds[3];
    seconds[0] = *date;
    INCREMENT_POINTER(date)
    seconds[1] = *date;
    seconds[2] = '\0';
    t.tm_sec = atoi(seconds);
    *timeOutput = mktime(&t);
    return 0;
}

#endif