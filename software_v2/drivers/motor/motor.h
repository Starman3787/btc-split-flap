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
#ifndef _DRIVERS_MOTOR_H
#define _DRIVERS_MOTOR_H

#include <stdint.h>

extern uint8_t motor_pins[5];
extern uint8_t motor_enable_pins[5];

void init_motor(void);
void write_motor(uint16_t motor);
void toggle_motor(uint16_t motor);

#endif