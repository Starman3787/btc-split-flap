#include <stdio.h>
#include <string.h>
#include "util/http/http.h"
#include "drivers/wifi/wifi.h"
#include "drivers/split_flap/split_flap.h"
#include "drivers/esp_01s/esp_01s.h"
#include "drivers/motor/motor.h"
#include "timers/systick/systick.h"
#include "util/delay/delay.h"

void mode_btc(void)
{
    puts("Fetching price...");

    // fetch price and convert it to a string
    Http *parsedHttp = make_http_request(PROTOCOL, HOST, PORT, REQUEST_SIZE, REQUEST);

    if (parsedHttp->statusCode != 200)
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
        int64_t rate;
        for (uint8_t i = 0; i < parsedHttp->responseBody->data_size / sizeof(Json *); i++)
        {
            Json *element = *(parsedHttp->responseBody->data.data_json + i);
            if (element->type == JSON_NUMBER && strcmp(element->key, "rate") == 0)
                rate = element->data.json_number;
        }

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
        sprintf(priceString, "%llu", rate);
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

    free_http(&parsedHttp);
}