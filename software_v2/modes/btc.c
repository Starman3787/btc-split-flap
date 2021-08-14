#include <stdio.h>
#include "../util/http/http.h"
#include "../drivers/wifi/wifi.h"
#include "../drivers/split_flap/split_flap.h"
#include "../timers/systick/systick.h"

void mode_btc(void)
{
    print_full("Fetching price...");

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

        char priceString[6];
        sprintf(priceString, "%llu", rate);
        print_full(priceString);

        char timeString[11];
        sprintf(timeString, "%lli", unix);
        print_full(timeString);

        char timeStringTicks[13];
        sprintf(timeStringTicks, "%llu", ticks);
        print_full(timeStringTicks);

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