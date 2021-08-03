#include <stdint.h>
#include <stdio.h>
#include "./timers/systick/systick.h"
#include "./main.h"
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
    init_wifi();

    print_full("Starting...");

    uint8_t module_positions[MODULE_COUNT];

    // initialise all motors to their home position
    init_split_flap(module_positions);

    // display the message on the split flap display
    display_message("HELLO", module_positions);

    delay_ms(5000);

    // disable all motors
    toggle_motor(0);
    toggle_motor(1);
    toggle_motor(2);
    toggle_motor(3);
    toggle_motor(4);

    while (1)
    {
        print_full("Fetching price...");

        // fetch price and convert it to a string
        char priceString[6];
        uint32_t price = fetch_price();
        sprintf(priceString, "%lu", price);

        // enable all motors
        toggle_motor(0);
        toggle_motor(1);
        toggle_motor(2);
        toggle_motor(3);
        toggle_motor(4);

        // give motors a second to stabilise
        delay_ms(1000);

        // initialise all motors to their home position
        init_split_flap(module_positions);

        // display the message on the split flap display
        display_message(priceString, module_positions);

        // give the motors a second to stabilise
        delay_ms(1000);

        // disable all motors
        toggle_motor(0);
        toggle_motor(1);
        toggle_motor(2);
        toggle_motor(3);
        toggle_motor(4);

        // wait 15 minutes
        delay_ms(900000);
        // 900000
    }
}