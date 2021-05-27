/**
 * @file hall_effect_sensor.c
 * @author Starman
 * @brief Standard functions for interracting with a hall effect sensor
 * @version 0.1
 * @date 2021-05-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "../../headers/stm32f767xx.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * Pin  - Sensor
 * 
 * PE2  - 0
 *
 */
uint8_t hall_effect_sensor_pins[] = {
    2};

/**
 * @brief Initialises the hall effect sensor by setting the mode
 * 
 */
void init_hall_effect_sensor(void)
{
    // enable the GPIO clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;

    // set the mode for the digital input pin
    GPIOE->MODER &= ~(0b11 << (hall_effect_sensor_pins[0] * 2));
}

/**
 * @brief Checks whether the hall effect sensor senses a magnet
 * 
 * @param sensor 
 * @return true The sensor has detected a magnet
 * @return false The sensor has not detected a magnet
 */
bool read_hall_effect_sensor(uint16_t sensor)
{
    return (GPIOE->IDR & (0b1 << hall_effect_sensor_pins[sensor])) == 0;
}