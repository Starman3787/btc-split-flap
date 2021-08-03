#ifndef PARSE_DATE_C
#define PARSE_DATE_C

#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include "./parse_date.h"

/**
 * @brief Parses a HTTP "Date" header, specified here: https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Date
 * 
 * @param date 
 * @return time_t The UNIX timestamp (seconds)
 */
time_t parse_date(char *date)
{
    struct tm t;
    time_t t_of_day;
    t.tm_isdst = 0;
    date += 4;
    char *mday = malloc(sizeof(char) * 1);
    *mday = '\0';
    for (uint8_t i = 1; *(++date) != ' '; i++)
    {
        mday = realloc(mday, sizeof(char) * (i + 1));
        *(mday + i - 1) = *date;
        *(mday + i) = '\0';
    }
    t.tm_mday = atoi(mday);
    free(mday);
    char mon[4];
    mon[3] = '\0';
    for (uint8_t i = 0; *(++date) != ' '; i++)
         mon[i] = *date;
    t.tm_mon = convertMonth(mon);
    char *year = malloc(sizeof(char) * 1);
    *year = '\0';
    for (uint8_t i = 1; *(++date) != ' '; i++)
    {
        year = realloc(year, sizeof(char) * (i + 1));
        *(year + i - 1) = *date;
        *(year + i) = '\0';
    }
    t.tm_year = atoi(year) - 1900;
    free(year);
    date++;
    char hours[3];
    hours[0] = *(date++);
    hours[1] = *(date++);
    hours[2] = '\0';
    t.tm_hour = atoi(hours);
    date++;
    char minutes[3];
    minutes[0] = *(date++);
    minutes[1] = *(date++);
    minutes[2] = '\0';
    t.tm_min = atoi(minutes);
    date++;
    char seconds[3];
    seconds[0] = *(date++);
    seconds[1] = *(date++);
    seconds[2] = '\0';
    t.tm_sec = atoi(seconds);
    return mktime(&t);
}

#endif