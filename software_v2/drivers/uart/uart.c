#include <stdint.h>
#include "../../headers/stm32f767xx.h"

void init_uart(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_UART4EN;
}