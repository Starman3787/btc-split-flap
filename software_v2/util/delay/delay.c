#include <stdint.h>
#include "timers/systick/systick.h"
#include "util/delay/delay.h"

// blocking delay
void delay_ms(uint32_t milliseconds)
{
    // wraparound is not a problem here
    // the counter should run for 2^64 milliseconds
    // or 584,554,531 years before wrapping around
    uint64_t waitUntilTicks = ticks + milliseconds;
    uint64_t currentTicks = ticks;
    do
    {
        // keep setting the currentTicks to the current number of ticks
        // prevents the compiler from optimising this loop out
        currentTicks = ticks;
    } while (currentTicks < waitUntilTicks);
}