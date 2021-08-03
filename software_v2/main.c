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
            init_split_flap(module_positions);

            display_message("ERROR");

            // give motors a second to stabilise
            delay_ms(1000);

            // disable all motors
            toggle_motor(0);
            toggle_motor(1);
            toggle_motor(2);
            toggle_motor(3);
            toggle_motor(4);

            delay_ms(1000 * 60);
        }
        else
        {
            int64_t rate;
            for (uint8_t i = 0; i < parsedHttp->responseBody->data_size / sizeof(Json *); i++)
            {
                Json *element = *(parsedHttp->responseBody->data.data_json + i);
                print_full(element->key);
                if (element->type == JSON_NUMBER && strcmp(element->key, "rate") == 0)
                    rate = element->data.json_number;
            }

            char priceString[6];
            sprintf(priceString, "%llu", rate);
            print_full(priceString);

            Header *currentTimeString = find_header(parsedHttp->headers, parsedHttp->headersLength, "date");

            unix = parse_date(currentTimeString->value);

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
}