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
#include <stdint.h>

/**
 * Pin  - Led
 * 
 * PB0  - Green
 * PB7  - Blue
 * PB14 - Red
 * 
 */
uint8_t led_pins[] = {
    0,
    7,
    14};