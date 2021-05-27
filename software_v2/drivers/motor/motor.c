#include "../../headers/stm32f767xx.h"
#include <stdint.h>

/**
 * Pin  - Sensor
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

uint8_t motor_enable_pins[] = {
    };

void init_motor(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
    GPIOF->MODER |= (0b01 << (motor_pins[0] * 2));
    GPIOF->MODER |= (0b01 << (motor_pins[1] * 2));
    GPIOF->MODER |= (0b01 << (motor_pins[2] * 2));
    GPIOF->MODER |= (0b01 << (motor_pins[3] * 2));
    GPIOF->MODER |= (0b01 << (motor_pins[4] * 2));
}

void write_motor(uint16_t motor)
{
    GPIOF->ODR ^= (0b1 << motor_pins[motor]);
}

void toggle_motor(uint16_t motor)
{
    
}