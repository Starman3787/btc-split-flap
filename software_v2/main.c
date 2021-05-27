#include "./timers/systick/systick.h"
volatile uint64_t ticks = 0;

void main(void)
{
    init_hall_effect_sensor();
    init_motor();
    init_systick();
    while (1)
    {
        delay_ms(25);
        write_motor(0);
        write_motor(1);
        write_motor(2);
        write_motor(3);
        write_motor(4);
    }
}