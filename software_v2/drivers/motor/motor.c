#include "../../headers/stm32f767xx.h"
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

void init_motor(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
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
    GPIOD->ODR ^= (0b1 << motor_enable_pins[motor]);
}