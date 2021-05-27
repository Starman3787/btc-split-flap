#include "../../headers/stm32f767xx.h"
#include "../../headers/CMSIS/core_cm7.h"

void init_systick(void)
{
    SysTick->CTRL |= (0b1 << 1);
    SysTick->CTRL |= (0b1 << 2);
    SysTick->LOAD = 18749;
    SysTick->VAL = 0;
    SysTick->CTRL |= (0b1 << 0);
}