#include <stdbool.h>
#include "util/status/status.h"
#include "drivers/led/led.h"

void status_error(bool status)
{
    write_led(OK_PIN, false);
    write_led(ERROR_PIN, status);
}

void status_loading(bool status)
{
    write_led(LOADING_PIN, status);
}

void status_ok(bool status)
{
    write_led(OK_PIN, status);
}

void status_loading_flash(void)
{
    toggle_led(LOADING_PIN);
}