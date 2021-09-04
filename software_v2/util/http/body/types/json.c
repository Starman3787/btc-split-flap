#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "drivers/uart/uart.h"
#include "util/http/http.h"

int8_t skip_whitespace(char **cursor)
{
    while (**cursor == '\t' || **cursor == '\r' || **cursor == '\n' || **cursor == ' ')
    {
        INCREMENT_POINTER((*cursor))
    }
    return 0;
}

int8_t get_property_name(char elementKey[], char **cursor)
{
    if (skip_whitespace(cursor) != 0)
        return -1;
    if (**cursor == 't' || **cursor == 'f' || **cursor == 'n' || **cursor == '[' || **cursor == '{')
        return -1;
    INCREMENT_POINTER((*cursor))
    elementKey[0] = '\0';
    for (uint8_t i = 0; **cursor != '"' && i < 62; i++)
    {
        elementKey[i] = **cursor;
        elementKey[i + 1] = '\0';
        INCREMENT_POINTER((*cursor))
    }
    INCREMENT_POINTER((*cursor))
    return 0;
}

int8_t get_string(char elementString[], char **cursor)
{
    elementString[0] = '\0';
    for (uint8_t i = 0; **cursor != '"' && i < 126; i++)
    {
        elementString[i] = **cursor;
        elementString[i + 1] = '\0';
        INCREMENT_POINTER((*cursor))
    }
    INCREMENT_POINTER((*cursor))
    return 0;
}

int8_t get_boolean(bool *elementBoolean, char **cursor)
{
    char value[5];
    value[0] = **cursor;
    INCREMENT_POINTER(*cursor)
    value[1] = **cursor;
    INCREMENT_POINTER(*cursor)
    value[2] = **cursor;
    INCREMENT_POINTER(*cursor)
    value[3] = **cursor;
    INCREMENT_POINTER(*cursor)
    value[4] = '\0';
    *elementBoolean = strcmp(value, "true") == 0 ? true : false;
    return 0;
}

int8_t get_number(int64_t *elementNumber, char **cursor)
{
    char value[64];
    value[0] = '\0';
    for (uint8_t i = 0; (**cursor == '.' || isdigit((unsigned char)**cursor)) && i < 62; i++)
    {
        value[i] = **cursor;
        value[i + 1] = '\0';
        INCREMENT_POINTER((*cursor))
    }
    *elementNumber = strtoll(value, NULL, 10);
    return 0;
}

int8_t parse_element(Json *jsonElement, const char *jsonProperty, char **cursor)
{
    char elementKey[64];
    if (get_property_name(elementKey, cursor) != 0)
        return -1;
    bool wantedElement = false;
    if (strcmp(elementKey, jsonProperty) == 0)
    {
        wantedElement = true;
        strncpy(jsonElement->key, elementKey, 63);
    }
    if (elementKey[0] != '\0')
    {
        INCREMENT_POINTER((*cursor))
        if (skip_whitespace(cursor) != 0)
            return -1;
    }
    char elementString[128];
    bool elementBoolean;
    int64_t elementNumber;
    switch (**cursor)
    {
    case '"':
        // string
        INCREMENT_POINTER((*cursor))
        if (get_string(elementString, cursor) != 0)
            return -1;
        if (wantedElement == true)
        {
            jsonElement->type = JSON_STRING;
            strncpy(jsonElement->data.json_string, elementString, 127);
            return 0;
        }
        break;
    case 't':
    case 'f':
    case 'n':
        // boolean
        if (get_boolean(&elementBoolean, cursor) != 0)
            return -1;
        if (wantedElement == true)
        {
            jsonElement->type = JSON_BOOLEAN;
            jsonElement->data.json_boolean = elementBoolean;
            return 0;
        }
        break;
    default:
        // number
        if (get_number(&elementNumber, cursor) != 0)
            return -1;
        if (wantedElement == true)
        {
            jsonElement->type = JSON_NUMBER;
            jsonElement->data.json_number = elementNumber;
            return 0;
        }
        break;
    }
    if (skip_whitespace(cursor) != 0)
        return -1;
    return 1;
}

int8_t parse_json(Json *jsonElement, const char *jsonProperty, char *body)
{
    if (*body != '{')
        return -1;
    INCREMENT_POINTER(body)
    while (*body != '\0')
    {
        if (skip_whitespace(&body) != 0)
            return -1;
        if (*body == '\0')
            break;
        if (parse_element(jsonElement, jsonProperty, &body) == 0)
            return 0;
        INCREMENT_POINTER(body)
    }
    return -1;
}