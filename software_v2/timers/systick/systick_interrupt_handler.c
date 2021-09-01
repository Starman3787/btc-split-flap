/**
 * @file systick_interrupt_handler.c
 * @author Starman
 * @brief Handles the SysTick interrupt
 * @version 0.1
 * @date 2021-05-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "timers/systick/systick.h"

/**
 * @brief SysTick interrupt handler, is called every millisecond
 * 
 */
void SysTick_Handler(void)
{
    ticks++;
    if (ticks % 1000 == 0)
        unix++;
}