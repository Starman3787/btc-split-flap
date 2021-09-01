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

typedef struct chunkData {
    char chunkData[1461];
    uint16_t chunkLength;
} ChunkData;

ChunkData rawResponseChunks[10];

void init_esp_01s(void);
int8_t make_http_request(char *protocol, char *host, char *port, char *size, char *httpRequest, Http *response);

#endif