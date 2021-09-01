/**
 * @file systick.h
 * @author Starman
 * @brief SysTick header
 * @version 0.1
 * @date 2021-05-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef SYSTICK_H
#define SYSTICK_H
#include <stdint.h>
#include <time.h>

void SysTick_Handler(void);

extern volatile uint64_t ticks;
extern volatile time_t unix;

void init_systick(void);

#endif