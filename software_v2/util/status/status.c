/**
 * @file status.c
 * @author Starman
 * @brief Functions for using the LEDs as status lights
 * @version 0.1
 * @date 2021-07-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdbool.h>
#include "util/status/status.h"
#include "drivers/led/led.h"

/**
 * @brief Switches the red LED on
 * 
 * @param status 
 */
void status_error(bool status)
{
    write_led(OK_PIN, false);
    write_led(ERROR_PIN, status);
}

/**
 * @brief Switched the blue LED on
 * 
 * @param status 
 */
void status_loading(bool status)
{
    write_led(LOADING_PIN, status);
}

/**
 * @brief Switches the green LED on
 * 
 * @param status 
 */
void status_ok(bool status)
{
    write_led(OK_PIN, status);
}

/**
 * @brief Toggles the blue LED on and off
 * 
 */
void status_loading_flash(void)
{
    toggle_led(LOADING_PIN);
}