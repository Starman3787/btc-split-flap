/**
 * @file led.c
 * @author Starman
 * @brief Functions for interacting with LEDs
 * @version 0.1
 * @date 2021-07-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdbool.h>
#include "../../headers/stm32f767xx.h"
#include "./led.h"

/**
 * @brief Initialises the LEDs
 * 
 */
void init_led(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    GPIOB->MODER |= (0b01 << (led_pins[0] * 2)) |
                    (0b01 << (led_pins[1] * 2)) |
                    (0b01 << (led_pins[2] * 2));
}

/**
 * @brief Sets a specified LED to a certain state
 * 
 * @param pin 
 * @param state 
 */
void write_led(uint8_t pin, bool state)
{
    if (state == true)
        GPIOB->ODR |= (0b1 << led_pins[pin]);
    else
        GPIOB->ODR &= ~(0b1 << led_pins[pin]);
}

/**
 * @brief Toggles a specified LED on or off
 * 
 * @param pin 
 */
void toggle_led(uint8_t pin)
{
    GPIOB->ODR ^= (0b1 << led_pins[pin]);
}