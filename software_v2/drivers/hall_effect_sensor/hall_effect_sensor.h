#ifndef DRIVERS_HALL_EFFECT_SENSOR_H_
#define DRIVERS_HALL_EFFECT_SENSOR_H_

#include <stdint.h>
#include <stdbool.h>

extern uint8_t hall_effect_sensor_pins[5];

int8_t init_hall_effect_sensor(void);
bool read_hall_effect_sensor(uint16_t sensor);

#endif