#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "headers/stm32f767xx.h"
#include "drivers/uart/uart.h"
#include "timers/systick/systick.h"
#include "main.h"
#include "drivers/split_flap/split_flap.h"

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
    GPIOC->MODER |= (0b10 << (uart_pins[0] * 2U)) |
                    (0b10 << (uart_pins[1] * 2U));
    GPIOB->MODER |= (0b10 << (uart_pins[2] * 2U)) |
                    (0b10 << (uart_pins[3] * 2U));

    // calculate and set the baud rate
    UART4->BRR = (CORE_CLOCK / BAUD_RATE);
    UART5->BRR = (CORE_CLOCK / BAUD_RATE_DEBUG);

    NVIC_EnableIRQ(UART4_IRQn);

    // enable the RX, TX and then enable the UART
    UART4->CR1 |= USART_CR1_RE |
                  USART_CR1_TE |
                  USART_CR1_RXNEIE |
                  USART_CR1_UE;
    UART5->CR1 |= USART_CR1_RE |
                  USART_CR1_TE |
                  USART_CR1_UE;

    return 0;
}

void UART4_IRQHandler(void)
{
    irq_uart_char = UART4->RDR;
    irq_uart_char_read = false;
}

void check_incoming_config(void)
{
    UART4_Interrupts_Disable();
    if (find_pattern("ONFIG:", 6, 100) == 0)
    {
        char configSetting = read_uart(100);
        read_uart(100);
        read_uart(100);
        switch (configSetting)
        {
        case '0':
        {
            // off
            break;
        }
        case '1':
        {
            // on
            break;
        }
        case '2':
        {
            // standby
            // char rawTime[5];
            // rawTime[0] = read_uart(100);
            // rawTime[1] = read_uart(100);
            // read_uart(100);
            // rawTime[2] = read_uart(100);
            // rawTime[3] = read_uart(100);
            // rawTime[4] = '\0';
            // read_uart(100);
            // read_uart(100);
            break;
        }
        }
    }
    UART4_Interrupts_Enable();
}

void check_incoming_mode(void)
{
    UART4_Interrupts_Disable();
    if (find_pattern("ODE:", 4, 100) == 0)
    {
        char configSetting = read_uart(100);
        printf("CONFIG: %c\n", configSetting);
        read_uart(100);
        read_uart(100);
        switch (configSetting)
        {
        case '0':
        {
            // btc
            currentMode = btc;
            newModeUpdated = false;
            break;
        }
        case '1':
        {
            // custom
            int16_t customLength = 0;
            char uartChar = read_uart(50);
            while (uartChar != ':')
            {
                customLength *= 10;
                customLength += (uartChar - '0');
                uartChar = read_uart(50);
            }
            for (uint8_t i = 0; i < customLength; i++)
            {
                char currentCustomChar = read_uart(50);
                if (isalnum(currentCustomChar) != 0 && i < MODULE_COUNT)
                {
                    modeCustomText[i] = currentCustomChar;
                    modeCustomText[i + 1] = '\0';
                }
            }
            printf("CUSTOM TEXT: %s\n", modeCustomText);
            currentMode = custom;
            newModeUpdated = false;
            break;
        }
        }
    }
    UART4_Interrupts_Enable();
}

void UART4_Interrupts_Enable(void)
{
    UART4->CR1 |= USART_CR1_RXNEIE;
}

void UART4_Interrupts_Disable(void)
{
    UART4->CR1 &= ~(USART_CR1_RXNEIE);
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
        if (*(message++) != currentValue)
            return -1;
    }
    return 0;
}

// reads the UART until a match is found with the specified pattern of characters
int8_t find_pattern(char *pattern, uint16_t patternLength, uint16_t timeout)
{
    uint8_t matchingChars = 0;
    uint64_t readUntil = ticks + (uint64_t)(timeout * patternLength * 5);
    while (matchingChars != patternLength)
    {
        if (readUntil < ticks)
            return -1;
        char currentValue = read_uart(timeout);
        if (currentValue == *(pattern + matchingChars))
            matchingChars++;
        else
            matchingChars = 0;
    }
    return 0;
}