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
#include <stdint.h>

void SysTick_Handler(void);

extern volatile uint64_t ticks;