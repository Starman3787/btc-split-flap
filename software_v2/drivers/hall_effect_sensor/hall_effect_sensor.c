#include "../../headers/stm32f767xx.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * Pin  - Sensor
 * 
 * PD7  - 0
 *
 */
uint8_t hall_effect_sensor_pins[] = {
    7};

void init_hall_effect_sensor(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    for (uint8_t i = 0; i < sizeof hall_effect_sensor_pins / hall_effect_sensor_pins[0]; i++)
        GPIOD->MODER &= (0b11 << (hall_effect_sensor_pins[i] * 2));
}

bool read_hall_effect_sensor(uint16_t sensor)
{
    return (GPIOD->IDR & (0b1 << hall_effect_sensor_pins[sensor])) < 0;
}