#ifndef SYSTICK_H
#define SYSTICK_H
#include <stdint.h>
#include <time.h>

void SysTick_Handler(void);

extern volatile uint64_t ticks;
extern volatile time_t unix;

void init_systick(void);

#endif