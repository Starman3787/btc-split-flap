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
#include <stdint.h>

/**
 * Pin  - Motor
 * 
 * PC10 - UART4_TX
 * PC11 - UART4_RX
 * PB9  - UART5_TX
 * PB8  - UART5_RX
 *
 */
uint8_t uart_pins[] = {
    10,
    11,
    9,
    8};

#define CORE_CLOCK      (16000000U)
#define BAUD_RATE       (9600U)