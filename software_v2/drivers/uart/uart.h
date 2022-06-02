#ifndef DRIVERS_UART_H_
#define DRIVERS_UART_H_

#include <stdint.h>
#include <stdbool.h>

extern uint8_t uart_pins[4];
extern char irq_uart_char;
extern bool irq_uart_char_read;

#define CORE_CLOCK      (16000000U)
#define BAUD_RATE       (9600U)
#define BAUD_RATE_DEBUG (115200U)

int8_t init_uart(void);
void print(char byte);
void print_full(char *message);
void write_uart(char byte);
char read_uart(uint16_t timeout);
void write_full_uart(char *message);
int8_t read_full_uart_and_expect(char *message, uint16_t timeout);
int8_t find_pattern(char *pattern, uint16_t patternLength, uint16_t timeout);
void UART4_IRQHandler(void);
void UART4_Interrupts_Enable(void);
void UART4_Interrupts_Disable(void);
void check_incoming_config(void);
void check_incoming_mode(void);

#endif