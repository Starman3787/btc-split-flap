#include <stdio.h>
#include <string.h>
#include "util/http/http.h"
#include "main.h"
#include "drivers/split_flap/split_flap.h"
#include "drivers/esp_01s/esp_01s.h"
#include "drivers/stepper_motor/stepper_motor.h"
#include "timers/systick/systick.h"
#include "util/delay/delay.h"

int8_t mode_btc(void)
{
    puts("Fetching price...");

    Http parsedHttp;
    // fetch price and convert it to a string
    if (make_http_request(&parsedHttp, "rate", PROTOCOL, HOST, PORT, REQUEST_SIZE, REQUEST) != 0)
        return -1;

    if (parsedHttp.statusCode != 200)
    {

        // enable all motors
        toggle_motor(0);
        toggle_motor(1);
        toggle_motor(2);
        toggle_motor(3);
        toggle_motor(4);

        // give motors a second to stabilise
        delay_ms(1000);

        // initialise all motors to their home position
        init_split_flap();

        display_message("ERROR");

        // give motors a second to stabilise
        delay_ms(1000);

        // disable all motors
        toggle_motor(0);
        toggle_motor(1);
        toggle_motor(2);
        toggle_motor(3);
        toggle_motor(4);

    }
    else
    {
        int64_t rate = parsedHttp.responseBody.data.data_json.data.json_number;

        printf("PRICE: %llu\n", rate);

        printf("UNIX_TIME: %lli\n", unix);

        printf("STARTUP_TIME: %llu\n", ticks);

        // enable all motors
        toggle_motor(0);
        toggle_motor(1);
        toggle_motor(2);
        toggle_motor(3);
        toggle_motor(4);

        // give motors a second to stabilise
        delay_ms(1000);

        // initialise all motors to their home position
        init_split_flap();

        // display the message on the split flap display
        char priceString[6];
        snprintf(priceString, 6, "%llu", rate);
        display_message(priceString);

        // give the motors a second to stabilise
        delay_ms(1000);

        // disable all motors
        toggle_motor(0);
        toggle_motor(1);
        toggle_motor(2);
        toggle_motor(3);
        toggle_motor(4);

    }

    return 0;
}