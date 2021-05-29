/**
 * @file motor.h
 * @author Starman
 * @brief Motor header file
 * @version 0.1
 * @date 2021-05-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdint.h>

/**
 * Pin  - Motor
 * 
 * PF7  - 0
 * PF8  - 1
 * PF9  - 2
 * PF10 - 3
 * PF15 - 4
 *
 */
uint8_t motor_pins[] = {
    7,
    8,
    9,
    10,
    15};

/**
 * Pin  - Motor
 * 
 * PD0  - 0
 * PD1  - 1
 * PD2  - 2
 * PD3  - 3
 * PD4  - 4
 * 
 */
uint8_t motor_enable_pins[] = {
    0,
    1,
    2,
    3,
    4};