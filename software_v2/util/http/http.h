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
    char *key;
    JsonType type;
    union {
        int64_t json_number;
        char *json_string;
        bool json_boolean;
        Json **json_array;
        Json **json_object;
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
char *get_property_name(char **cursor);
char *get_string(char **cursor, size_t *size);
bool get_boolean(char **cursor, size_t *size);
Json **get_array(char **cursor, size_t *size);
Json **get_object(char **cursor, size_t *size);
Json *parse_element(char **cursor);
Json **parse_json(char *body, size_t *size);

/* Other internals */
int8_t split_header_values(char *contentTypeHeaderValues[], char *headerValue, uint8_t maxValues, uint8_t maxValueLength, const char splitBy);

/* Front-facing API */
void parse_http(Http *parsedHttp, char *rawHttp);
Header *find_header(Header *headers, uint8_t headersLength, char *key);
uint16_t http_response_status(char *rawHttp);
int8_t http_header_parser(Header *allHeaders, char *rawHttp, uint8_t *headerIndex, char **headersEnd);
int8_t http_body_parser(Http *parsedHttp, char *rawBody, Header *headers, uint8_t headersLength);

void free_body(Body *parsedBody);
void free_http(Http *parsedHttp);
void free_json(Json ***jsonBody, size_t child_size);

#endif