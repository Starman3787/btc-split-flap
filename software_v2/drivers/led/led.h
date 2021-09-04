#ifndef DRIVERS_LED_H_
#define DRIVERS_LED_H_

#include <stdint.h>

extern uint8_t led_pins[3];

int8_t init_led(void);
void write_led(uint8_t pin, bool state);
void toggle_led(uint8_t pin);

#endif