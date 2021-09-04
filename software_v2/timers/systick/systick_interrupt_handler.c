#include "timers/systick/systick.h"

// SysTick interrupt handler, is called every millisecond
void SysTick_Handler(void)
{
    ticks++;
    if (ticks % 1000 == 0)
        unix++;
}