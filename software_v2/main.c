#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "timers/systick/systick.h"
#include "main.h"
#include "drivers/hall_effect_sensor/hall_effect_sensor.h"
#include "drivers/stepper_motor/stepper_motor.h"
#include "drivers/led/led.h"
#include "drivers/split_flap/split_flap.h"
#include "drivers/uart/uart.h"
#include "drivers/esp_01s/esp_01s.h"
#include "util/delay/delay.h"
#include "util/http/http.h"
#include "modes/btc/btc.h"
#include "modes/test/test.h"
volatile uint64_t ticks = 0;
volatile time_t unix = 0;

void main(void)
{
    init_hall_effect_sensor();
    init_motor();
    init_systick();
    delay_ms(1000);
    init_uart();
    puts("UART started...");
    init_led();
    puts("Initialising WiFi...");
    init_esp_01s();

    puts("Starting...");

    // initialise all motors to their home position
    init_split_flap();

    // display the message on the split flap display
    display_message("HELLO");

    delay_ms(5000);

    // disable all motors
    toggle_motor(0);
    toggle_motor(1);
    toggle_motor(2);
    toggle_motor(3);
    toggle_motor(4);

    mode_btc();

    while (1)
    {
        
        // mode_test();
        // 900
        if ((unix + 120) % 900 == 0)
            mode_btc();

    }
}