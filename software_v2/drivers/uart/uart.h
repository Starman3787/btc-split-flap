/**
 * @file uart.h
 * @author Starman
 * @brief UART header file
 * @version 0.1
 * @date 2021-06-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef _DRIVERS_UART_H
#define _DRIVERS_UART_H

#include <stdint.h>
#include <stdbool.h>

extern uint8_t uart_pins[4];

#define CORE_CLOCK      (16000000U)
#define BAUD_RATE       (9600U)

void init_uart(void);
void print(char byte);
void print_full(char *message);
void write_uart(char byte);
char read_uart(uint16_t timeout);
void write_full_uart(char *message);
bool read_full_uart_and_expect(char *message, uint16_t timeout);
void find_pattern(char *pattern, uint16_t patternLength);

#endif