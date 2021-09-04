#include <stdbool.h>
#include "headers/stm32f767xx.h"
#include "drivers/led/led.h"

/**
 * Pin  - Led
 * 
 * PB0  - Green
 * PB7  - Blue
 * PB14 - Red
 * 
 */
uint8_t led_pins[3] = {
    0,
    7,
    14};

int8_t init_led(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    GPIOB->MODER |= (0b01 << (led_pins[0] * 2)) |
                    (0b01 << (led_pins[1] * 2)) |
                    (0b01 << (led_pins[2] * 2));

    return 0;
}

// switches an led on/off
void write_led(uint8_t pin, bool state)
{
    if (state == true)
        GPIOB->ODR |= (0b1 << led_pins[pin]);
    else
        GPIOB->ODR &= ~(0b1 << led_pins[pin]);
}

// toggles an led
void toggle_led(uint8_t pin)
{
    GPIOB->ODR ^= (0b1 << led_pins[pin]);
}