#ifndef UTIL_HTTP_H_
#define UTIL_HTTP_H_

#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>

#define INCREMENT_POINTER(pointer) if (*(++pointer) == '\0') return 1;

extern char httpSupported[2][9];

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
    char key[64];
    JsonType type;
    union {
        int64_t json_number;
        char json_string[128];
        bool json_boolean;
    } data;
};

typedef struct body {
    ContentType content_type;
    union {
        Json data_json;
        char data_text[1024];
    } data;
} Body;

typedef struct header {
    char key[64];
    char value[128];
} Header;

typedef struct http {
    uint16_t statusCode;
    Header headers[16];
    uint8_t headersLength;
    Body responseBody;
} Http;

/* JSON parsing functions */
void skip_whitespace(char **cursor);
int8_t get_property_name(char elementKey[], char **cursor);
int8_t get_string(char elementString[], char **cursor);
int8_t get_boolean(bool *elementBoolean, char **cursor);
int8_t get_number(int64_t *elementNumber, char **cursor);
// Json **get_array(char **cursor, size_t *size);
// Json **get_object(char **cursor, size_t *size);
int8_t parse_element(Json *jsonElement, const char *jsonProperty, char **cursor);
int8_t parse_json(Json *jsonElement, const char *jsonProperty, char *body);

/* Other internals */
int8_t split_header_values(char *contentTypeHeaderValues[], char *headerValue, uint8_t maxValues, uint8_t maxValueLength, const char splitBy);

/* Front-facing API */
void parse_http(Http *parsedHttp, char *rawHttp);
Header *find_header(Header *headers, uint8_t headersLength, char *key);
uint16_t http_response_status(char *rawHttp);
int8_t http_header_parser(Header *allHeaders, char *rawHttp, uint8_t *headerIndex, char **headersEnd);
int8_t http_body_parser(Http *parsedHttp, char *rawBody, Header *headers, uint8_t headersLength);

#endif