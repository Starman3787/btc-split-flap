#include <stdint.h>
#include <string.h>
#include "headers/stm32f767xx.h"
#include "drivers/uart/uart.h"
#include "timers/systick/systick.h"

/**
 * Pin  - Motor
 * 
 * PC10 - UART4_TX
 * PC11 - UART4_RX
 * PB9  - UART5_TX
 * PB8  - UART5_RX
 *
 */
uint8_t uart_pins[4] = {
    10,
    11,
    9,
    8};

/**
 * @brief Initialises the UART pins
 * 
 */
int8_t init_uart(void)
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

    return 0;
}

// writes a single character to the UART
// this UART is typically used for IO
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

// writes a string to the UART followed by CRLF
void print_full(char *message)
{
    // iterate over the message and output each character one by one
    while (*message != '\0')
        print(*(message++));
    // output the line termination sequence
    print('\r');
    print('\n');
}

// writes a single character to the UART
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

// reads a single character from the UART
char read_uart(uint16_t timeout)
{
    uint64_t readUntil = ticks + (uint64_t)timeout;
    // interrupt and status register
    uint16_t isr;
    do
    {
        // set current value
        // prevents compiler from optimising this loop out
        isr = !(UART4->ISR & USART_ISR_RXNE);
        if (ticks > readUntil)
            return '\0';
    } while (isr);
    // return the register value
    return UART4->RDR;
}

// writes a string to the UART followed by CRLF
void write_full_uart(char *message)
{
    // iterate over the message and output each character one by one
    while (*message != '\0')
        write_uart(*(message++));
    // output the line termination sequence
    write_uart('\r');
    write_uart('\n');
}

// reads the UART and expects a specified message
int8_t read_full_uart_and_expect(char *message, uint16_t timeout)
{
    // iterate over every character of the expected message
    while (*message != '\0')
    {
        // read the uart and check that the character matches
        char currentValue = read_uart(timeout);
        if (currentValue == '\0')
            return -1;
        if (*(message++) != currentValue)
            return -1;
    }
    return 0;
}

// reads the UART until a match is found with the specified pattern of characters
int8_t find_pattern(char *pattern, uint16_t patternLength, uint16_t timeout)
{
    uint8_t matchingChars = 0;
    while (matchingChars != patternLength)
    {
        char currentValue = read_uart(timeout);
        if (currentValue == *(pattern + matchingChars))
            matchingChars++;
        // else if (currentValue == '\0')
        //     return -1;
        else
            matchingChars = 0U;
    }
    return 0;
}