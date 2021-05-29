/**
 * @file motor.c
 * @author Starman
 * @brief Standard motor functions for interacting with stepper motors in combination with an A4988 driver
 * @version 0.2
 * @date 2021-05-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdint.h>
#include "../../headers/stm32f767xx.h"
#include "./motor.h"

/**
 * @brief Initialises the motor by setting the mode for both the enable and step pins
 * 
 */
void init_motor(void)
{
    // enable the GPIO clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    
    // set the mode for all the enable pins
    GPIOD->MODER |= (0b01 << (motor_enable_pins[0] * 2));
    GPIOD->MODER |= (0b01 << (motor_enable_pins[1] * 2));
    GPIOD->MODER |= (0b01 << (motor_enable_pins[2] * 2));
    GPIOD->MODER |= (0b01 << (motor_enable_pins[3] * 2));
    GPIOD->MODER |= (0b01 << (motor_enable_pins[4] * 2));

    // set the mode for all the step pins
    GPIOF->MODER |= (0b01 << (motor_pins[0] * 2));
    GPIOF->MODER |= (0b01 << (motor_pins[1] * 2));
    GPIOF->MODER |= (0b01 << (motor_pins[2] * 2));
    GPIOF->MODER |= (0b01 << (motor_pins[3] * 2));
    GPIOF->MODER |= (0b01 << (motor_pins[4] * 2));
}

/**
 * @brief Toggles the step pin on or off
 * 
 * @param motor 
 */
void write_motor(uint16_t motor)
{
    GPIOF->ODR ^= (0b1 << motor_pins[motor]);
}

/**
 * @brief Toggles the enable pin on or off
 * 
 * @param motor 
 */
void toggle_motor(uint16_t motor)
{
    // logic high will disable the driver
    // logic low will enable the driver
    GPIOD->ODR ^= (0b1 << motor_enable_pins[motor]);
}