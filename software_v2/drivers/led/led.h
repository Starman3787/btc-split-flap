/**
 * @file led.h
 * @author Starman
 * @brief LED header file
 * @version 0.1
 * @date 2021-07-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef _DRIVERS_LED_H
#define _DRIVERS_LED_H

#include <stdint.h>

extern uint8_t led_pins[3];

void init_led(void);
void write_led(uint8_t pin, bool state);
void toggle_led(uint8_t pin);

#endif