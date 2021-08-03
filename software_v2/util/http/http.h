#ifndef HTTP_H
#define HTTP_H

#include <stdint.h>
#include <stdbool.h>

typedef enum contenttype {
    HEADER_CONTENT_TYPE_TEXT_PLAIN,
    HEADER_CONTENT_TYPE_APPLICATION_JSON
} ContentType;

typedef enum jsontype {
    JSON_NUMBER,
    JSON_STRING,
    JSON_BOOLEAN,
    JSON_ARRAY,
    JSON_OBJECT
} JsonType;

typedef struct json Json;

struct json {
    char *key;
    JsonType type;
    union {
        int64_t json_number;
        char *json_string;
        bool json_boolean;
        struct Json **json_array;
        struct Json **json_object;
    } data;
    size_t child_size;
};

typedef struct body {
    ContentType content_type;
    union {
        Json **data_json;
        char *data_text;
    } data;
    size_t data_size;
} Body;

typedef struct header {
    char *key;
    char *value;
} Header;

typedef struct http {
    uint16_t statusCode;
    Header **headers;
    uint8_t headersLength;
    Body *responseBody;
} Http;

void skip_whitespace(char **cursor);
char *get_property_name(char **cursor);
char *get_string(char **cursor, size_t *size);
bool get_boolean(char **cursor, size_t *size);
Json **get_array(char **cursor, size_t *size);
Json **get_object(char **cursor, size_t *size);
Json *parse_element(char **cursor);

#endif