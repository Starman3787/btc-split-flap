#include <stdint.h>
#include "headers/stm32f767xx.h"
#include "drivers/stepper_motor/stepper_motor.h"

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
uint8_t motor_pins[5] = {
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
uint8_t motor_enable_pins[5] = {
    0,
    1,
    2,
    3,
    4};

int8_t init_motor(void)
{
    // enable the GPIO clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN |
                    RCC_AHB1ENR_GPIODEN;

    // set the mode for all the enable pins
    GPIOD->MODER |= (0b01 << (motor_enable_pins[0] * 2)) |
                    (0b01 << (motor_enable_pins[1] * 2)) |
                    (0b01 << (motor_enable_pins[2] * 2)) |
                    (0b01 << (motor_enable_pins[3] * 2)) |
                    (0b01 << (motor_enable_pins[4] * 2));

    // set the mode for all the step pins
    GPIOF->MODER |= (0b01 << (motor_pins[0] * 2)) |
                    (0b01 << (motor_pins[1] * 2)) |
                    (0b01 << (motor_pins[2] * 2)) |
                    (0b01 << (motor_pins[3] * 2)) |
                    (0b01 << (motor_pins[4] * 2));

    return 0;
}

// toggles the step pin, steps the stepper motor
void write_motor(uint16_t motor)
{
    GPIOF->ODR ^= (0b1 << motor_pins[motor]);
}

// toggle enables/disables the motor
void toggle_motor(uint16_t motor)
{
    // logic high will disable the driver
    // logic low will enable the driver
    GPIOD->ODR ^= (0b1 << motor_enable_pins[motor]);
}