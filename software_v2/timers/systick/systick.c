/**
 * @file systick.c
 * @author Starman
 * @brief SysTick functions
 * @version 0.1
 * @date 2021-05-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "../../headers/stm32f767xx.h"
#include "../../headers/CMSIS/core_cm7.h"

/**
 * @brief Initialises and starts the SysTick
 * 
 */
void init_systick(void)
{
    SysTick->CTRL |= (0b1 << 1); /* enables the SysTick interrupt */
    SysTick->CTRL |= (0b1 << 2); /* sets the clock source to the processor clock */
    SysTick->LOAD = 18749; /* sets the reload value to 18749, since the clock runs at 18.75 MHz */
    SysTick->VAL = 0; /* set the current value to 0 */
    SysTick->CTRL |= (0b1 << 0); /* enables the counter */
}