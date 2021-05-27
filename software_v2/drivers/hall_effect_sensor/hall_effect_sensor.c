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

void init_hall_effect_sensor(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
    GPIOE->MODER &= (0b11 << (hall_effect_sensor_pins[0] * 2));
    GPIOE->MODER &= ~(0b11 << (hall_effect_sensor_pins[0] * 2));
}

bool read_hall_effect_sensor(uint16_t sensor)
{
    return (GPIOE->IDR & (0b1 << hall_effect_sensor_pins[sensor])) == 0;
}