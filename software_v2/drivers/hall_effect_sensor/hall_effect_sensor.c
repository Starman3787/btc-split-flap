#include <stdbool.h>
#include <stdint.h>
#include "headers/stm32f767xx.h"
#include "drivers/hall_effect_sensor/hall_effect_sensor.h"

/**
 * Pin  - Sensor
 * 
 * PE2  - 0
 * PE3  - 1
 * PE4  - 2
 * PE5  - 3
 * PE6  - 4
 *
 */
uint8_t hall_effect_sensor_pins[5] = {
    2,
    3,
    4,
    5,
    6};

int8_t init_hall_effect_sensor(void)
{
    // enable the GPIO clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;

    // set the mode for the digital input pins
    GPIOE->MODER &= ~(0b11 << (hall_effect_sensor_pins[0] * 2));
    GPIOE->MODER &= ~(0b11 << (hall_effect_sensor_pins[1] * 2));
    GPIOE->MODER &= ~(0b11 << (hall_effect_sensor_pins[2] * 2));
    GPIOE->MODER &= ~(0b11 << (hall_effect_sensor_pins[3] * 2));
    GPIOE->MODER &= ~(0b11 << (hall_effect_sensor_pins[4] * 2));

    return 0;
}

// returns true if a magnet is detected, or false if not
bool read_hall_effect_sensor(uint16_t sensor)
{
    return (GPIOE->IDR & (0b1 << hall_effect_sensor_pins[sensor])) == 0U;
}