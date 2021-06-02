/**
 * @file hall_effect_sensor.h
 * @author Starman
 * @brief Hall effect sensor header file 
 * @version 0.1
 * @date 2021-05-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdint.h>

/**
 * Pin  - Sensor
 * 
 * PE2  - 0
 * PE3  - 1
 * PE4  - 2
 * PE5  - 3
 * PE6  - 4
 *
 */
uint8_t hall_effect_sensor_pins[] = {
    2,
    3,
    4,
    5,
    6};