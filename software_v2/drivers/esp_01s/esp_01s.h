/**
 * @file esp_01s.h
 * @author Starman
 * @brief 
 * @version 0.1
 * @date 2021-08-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef DRIVERS_ESP_01S_H_
#define DRIVERS_ESP_01S_H_

#include <stdint.h>
#include "util/http/http.h"

typedef struct chunkReference {
    char *chunkData;
    uint16_t chunkLength;
} ChunkReference;

void init_esp_01s(void);
Http *make_http_request(char *protocol, char *host, char *port, char *size, char *httpRequest);

#endif