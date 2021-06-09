#include <stdbool.h>
#include "../../headers/stm32f767xx.h"
#include "./led.h"

void init_led(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    GPIOB->MODER |= (0b01 << (led_pins[0] * 2)) |
                    (0b01 << (led_pins[1] * 2)) |
                    (0b01 << (led_pins[2] * 2));
}

void write_led(uint8_t pin, bool state)
{
    if (state == true)
        GPIOB->ODR |= (0b1 << led_pins[pin]);
    else
        GPIOB->ODR &= ~(0b1 << led_pins[pin]);
}