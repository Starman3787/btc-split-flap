#ifndef MAIN_H_
#define MAIN_H_

#include <stdint.h>
#include <stdbool.h>

#define MODULE_COUNT        (5)

enum modeOptions {
    btc,
    custom
};

extern uint8_t currentMode;
extern char modeCustomText[MODULE_COUNT + 1];
extern bool newModeUpdated;

#endif