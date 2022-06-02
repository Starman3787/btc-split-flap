#include <stdio.h>
#include "main.h"
#include "drivers/split_flap/split_flap.h"
#include "drivers/stepper_motor/stepper_motor.h"
#include "util/delay/delay.h"
#include "modes/custom/custom.h"

int8_t mode_custom(char *customText)
{
    newModeUpdated = true;

    printf("Displaying: %s\n", customText);

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

    display_message(customText);

    // give motors a second to stabilise
    delay_ms(1000);

    // disable all motors
    toggle_motor(0);
    toggle_motor(1);
    toggle_motor(2);
    toggle_motor(3);
    toggle_motor(4);

    return 0;
}