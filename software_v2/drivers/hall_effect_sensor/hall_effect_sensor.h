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
#ifndef DRIVERS_HALL_EFFECT_SENSOR_H_
#define DRIVERS_HALL_EFFECT_SENSOR_H_

#include <stdint.h>

extern uint8_t hall_effect_sensor_pins[5];

void init_hall_effect_sensor(void);
bool read_hall_effect_sensor(uint16_t sensor);

#endif