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
#include <string.h>
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
    RCC->APB1ENR |= RCC_APB1ENR_UART4EN |
                    RCC_APB1ENR_UART5EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN |
                    RCC_AHB1ENR_GPIOBEN;

    // set the alternate function
    GPIOC->AFR[1] |= (0b1000 << ((uart_pins[0] - 8) * 4)) |
                     (0b1000 << ((uart_pins[1] - 8) * 4));
    GPIOB->AFR[1] |= (0b0111 << ((uart_pins[2] - 8) * 4)) |
                     (0b0111 << ((uart_pins[3] - 8) * 4));

    // set the mode to alternate function
    GPIOC->MODER |= (0b10 << (uart_pins[0] * 2)) |
                    (0b10 << (uart_pins[1] * 2));
    GPIOB->MODER |= (0b10 << (uart_pins[2] * 2)) |
                    (0b10 << (uart_pins[3] * 2));

    // calculate and set the baud rate
    uint16_t uartdiv = CORE_CLOCK / BAUD_RATE;
    UART4->BRR = uartdiv;
    UART5->BRR = uartdiv;

    // enable the RX, TX and then enable the UART
    UART4->CR1 |= USART_CR1_RE |
                  USART_CR1_TE |
                  USART_CR1_UE;
    UART5->CR1 |= USART_CR1_RE |
                  USART_CR1_TE |
                  USART_CR1_UE;
}

void print(char byte)
{
    // interrupt and status register
    uint16_t isr;
    do
    {
        // set current value
        // to prevent compiler from optimising this bit out
        isr = !(UART5->ISR & USART_ISR_TXE);
    } while (isr);

    // transmit the byte
    UART5->TDR = byte;
}

/**
 * @brief Writes a single character
 * 
 * @param byte 
 */
void write_uart(char byte)
{
    // interrupt and status register
    uint16_t isr;
    do
    {
        // set current value
        // to prevent compiler from optimising this bit out
        isr = !(UART4->ISR & USART_ISR_TXE);
    } while (isr);

    // transmit the byte
    UART4->TDR = byte;
}

/**
 * @brief Reads a single character
 * 
 * @return char 
 */
char read_uart(void)
{
    // interrupt and status register
    uint16_t isr;
    do
    {
        // set current value
        // prevents compiler from optimising this loop out
        isr = !(UART4->ISR & USART_ISR_RXNE);
    } while (isr);

    // return the register value
    return UART4->RDR;
}

void read_full_uart(char *message)
{
    char previousTwo[2] = {'\0', '\0'};
    for (uint16_t i = 0; !(previousTwo[0] == '\r' && previousTwo[1] == '\n'); i++)
    {
        char currentValue = read_uart();
        message[i] = currentValue;

        previousTwo[0] = previousTwo[1];
        previousTwo[1] = currentValue;
    }
}

void write_full_uart(char *message)
{
    for (uint16_t i = 0; i < strlen(message); i++)
    {
        write_uart(message[i]);
    }
    write_uart('\r');
    write_uart('\n');
}

bool read_full_uart_and_expect(char *message)
{
    for (uint16_t i = 0; i < strlen(message); i++)
    {
        char currentValue = read_uart();
        if (message[i] != currentValue)
            return false;
    }
    return true;
}