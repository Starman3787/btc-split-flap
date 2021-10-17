#ifndef DRIVERS_MOTOR_H_
#define DRIVERS_MOTOR_H_

#include <stdint.h>

extern uint8_t motor_pins[5];
extern uint8_t motor_enable_pins[5];

int8_t init_motor(void);
void write_motor(uint16_t motor);
void toggle_motor(uint16_t motor);

#endif