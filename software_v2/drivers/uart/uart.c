/**
 * @file uart.c
 * @author Starman
 * @brief UART functions for setup and interacting with them
 * @version 0.1
 * @date 2021-06-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdint.h>
#include "../../headers/stm32f767xx.h"
#include "../../headers/system_stm32f7xx.h"
#include "./uart.h"

/**
 * @brief Initialises the UART pins
 * 
 */
void init_uart(void)
{
    // enable the UART4 clock as well as the GPIO clock
    RCC->APB1ENR |= RCC_APB1ENR_UART4EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    // set the mode to alternate function
    GPIOC->MODER |= (0b10 << (uart_pins[0] * 2));
    GPIOC->MODER |= (0b10 << (uart_pins[1] * 2));

    // set the alternate function
    GPIOC->AFR[1] |= (0b0001 << ((uart_pins[0] - 8) * 4));
    GPIOC->AFR[1] |= (0b0001 << ((uart_pins[1] - 8) * 4));

    // calculate and set the baud rate
    uint16_t uartdiv = CORE_CLOCK / BAUD_RATE;
    UART4->BRR = (((uartdiv / 16) << USART_BRR_DIV_MANTISSA_Pos) | ((uartdiv % 16) << USART_BRR_DIV_FRACTION_Pos));

    // enable the RX, TX and then enable the UART
    UART4->CR1 |= USART_CR1_RE;
    UART4->CR1 |= USART_CR1_TE;
    UART4->CR1 |= USART_CR1_UE;
}