#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "../../http.h"

void skip_whitespace(char **cursor)
{
    while (**cursor == '\t' || **cursor == '\r' || **cursor == '\n' || **cursor == ' ')
        (*cursor)++;
}

char *get_property_name(char **cursor)
{
    skip_whitespace(cursor);
    if (**cursor == 't' || **cursor == 'f' || **cursor == 'n' || **cursor == '[' || **cursor == '{')
        return NULL;
    (*cursor)++;
    char *propertyName = malloc(sizeof(char) * 1);
    *propertyName = '\0';
    for (uint8_t i = 1; **cursor != '"'; i++, (*cursor)++)
    {
        propertyName = realloc(propertyName, sizeof(char) * (i + 1));
        *(propertyName + i - 1) = **cursor;
        *(propertyName + i) = '\0';
    }
    (*cursor)++;
    return propertyName;
}

char *get_string(char **cursor, size_t *size)
{
    char *value = malloc(sizeof(char) * 1);
    *value = '\0';
    uint8_t i;
    for (i = 1; **cursor != '"'; i++, (*cursor)++)
    {
        value = realloc(value, sizeof(char) * (i + 1));
        *(value + i - 1) = **cursor;
        *(value + i) = '\0';
    }
    *size = sizeof(char) * i;
    (*cursor)++;
    return value;
}

bool get_boolean(char **cursor, size_t *size)
{
    char *value = malloc(sizeof(char) * 5);
    *value = *((*cursor)++);
    *(value + 1) = *((*cursor)++);
    *(value + 2) = *((*cursor)++);
    *(value + 3) = *((*cursor)++);
    *(value + 4) = '\0';
    *size = sizeof(bool) * 1;
    bool result = strcmp(value, "true") == 0 ? true : false;
    free(value);
    return result;
}

Json **get_array(char **cursor, size_t *size)
{
    Json **elements = malloc(sizeof(Json *) * 0);
    uint8_t i;
    (*cursor)++;
    for (i = 0; **cursor != ']'; (*cursor)++)
    {
        skip_whitespace(cursor);
        if (**cursor == ']')
            break;
        elements = realloc(elements, sizeof(Json *) * (i + 1));
        Json *element = parse_element(cursor);
        *(elements + i) = element;
        if (**cursor == ']')
            break;
        i++;
    }
    (*cursor)++;
    *size = sizeof(Json *) * (i + 1);
    return elements;
}

Json **get_object(char **cursor, size_t *size)
{
    Json **elements = malloc(sizeof(Json *) * 0);
    uint8_t i;
    (*cursor)++;
    for (i = 0; **cursor != '}'; i++, (*cursor)++)
    {
        skip_whitespace(cursor);
        if (**cursor == '}')
            break;
        elements = realloc(elements, sizeof(Json *) * (i + 1));
        Json *element = parse_element(cursor);
        *(elements + i) = element;
        if (**cursor == '}')
            break;
    }
    (*cursor)++;
    *size = sizeof(Json *) * (i + 1);
    return elements;
}

int64_t get_number(char **cursor, size_t *size)
{
    char *value = malloc(sizeof(char) * 1);
    *value = '\0';
    uint8_t i;
    for (i = 1; **cursor == '.' || isdigit(**cursor); i++, (*cursor)++)
    {
        value = realloc(value, sizeof(char) * (i + 1));
        *(value + i - 1) = **cursor;
        *(value + i) = '\0';
    }
    char *endOfValuePointer = value + strlen(value);
    int64_t number = strtoll(value, &endOfValuePointer, 10);
    *size = sizeof(int64_t);
    free(value);
    return number;
}

Json *parse_element(char **cursor)
{
    Json *element = malloc(sizeof(Json) * 1);
    element->key = get_property_name(cursor);
    if (element->key != NULL)
    {
        (*cursor)++;
        skip_whitespace(cursor);
    }
    size_t size;
    switch (**cursor)
    {
    case '"':
        // string
        (*cursor)++;
        element->type = JSON_STRING;
        element->data.json_string = get_string(cursor, &size);
        element->child_size = size;
        break;
    case 't':
    case 'f':
    case 'n':
        // boolean
        element->type = JSON_BOOLEAN;
        element->data.json_boolean = get_boolean(cursor, &size);
        element->child_size = size;
        break;
    case '[':
        // array
        element->type = JSON_ARRAY;
        element->data.json_array = get_array(cursor, &size);
        element->child_size = size;
        break;
    case '{':
        // object
        element->type = JSON_OBJECT;
        element->data.json_object = get_object(cursor, &size);
        element->child_size = size;
        break;
    default:
        // number
        element->type = JSON_NUMBER;
        element->data.json_number = get_number(cursor, &size);
        element->child_size = size;
        break;
    }
    skip_whitespace(cursor);
    return element;
}

Json **parse_json(char *body, size_t *size)
{
    if (*body != '{')
        return NULL;
    Json **allJsonElements = malloc(sizeof(Json *) * 1);
    uint8_t count = 1;
    while (*(++body) != '\0')
    {
        skip_whitespace(&body);
        if (*body == '\0')
            break;
        allJsonElements = realloc(allJsonElements, sizeof(Json *) * count);
        Json *element = parse_element(&body);
        *(allJsonElements + count - 1) = element;
        count++;
    }
    *size = sizeof(Json *) * (count - 1);
    return allJsonElements;
}