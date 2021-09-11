#include "headers/stm32f767xx.h"
#include "headers/CMSIS/core_cm7.h"
#include "timers/systick/systick.h"

void init_systick(void)
{
    SysTick->CTRL |= (0b1 << 1); /* enables the SysTick interrupt */
    SysTick->CTRL |= (0b1 << 2); /* sets the clock source to the processor clock */
    SysTick->LOAD = 18749U; /* sets the reload value to 18749, since the clock runs at 18.75 MHz */
    SysTick->VAL = 0U; /* set the current value to 0 */
    SysTick->CTRL |= (0b1 << 0); /* enables the counter */
}