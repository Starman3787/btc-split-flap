#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "./timers/systick/systick.h"
#include "./main.h"
#include "./drivers/wifi/wifi.h"
#include "./util/http/http.h"
volatile uint64_t ticks = 0;
volatile time_t unix = 0;

void main(void)
{
    init_hall_effect_sensor();
    init_motor();
    init_systick();
    delay_ms(1000);
    init_uart();
    print_full("UART started...");
    init_led();
    print_full("Initialising WiFi...");
    init_esp_01s();

    print_full("Starting...");

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

        if ((unix + 120) % 900 == 0)
            mode_btc(); 

    }
}