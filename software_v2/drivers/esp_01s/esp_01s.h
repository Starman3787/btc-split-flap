#ifndef DRIVERS_ESP_01S_H_
#define DRIVERS_ESP_01S_H_

#include <stdint.h>
#include "util/http/http.h"

int8_t init_esp_01s(void);
int8_t make_http_request(Http *response, char *jsonProperty);
int8_t response_parser(Http *response, char *jsonProperty);

#endif