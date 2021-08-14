#include <stdint.h>

void mode_test(void)
{

    // enable all motors
    toggle_motor(0);
    toggle_motor(1);
    toggle_motor(2);
    toggle_motor(3);
    toggle_motor(4);

    delay_ms(1000);

    const char *strings[] = {
        "APPLE",
        "ABOUT",
        "ABUSE",
        "ADULT",
        "ADMIT",
        "BROKE",
        "BREAK",
        "BRINK",
        "BOAST",
        "CHIMP",
        "CHOMP",
        "CABLE",
        "DRIVE",
        "DRAPE",
        "DIVER",
        "DUMMY",
        "DRONE",
        "EAGLE",
        "EARLY",
        "EARTH",
        "EASEL",
        "FIRST",
        "FORTH",
        "FUNNY",
        "FRIES",
        "FORTE",
        "GRIPE",
        "HOUSE",
        "HEARD",
        "HEART"};

    for (uint8_t i = 0; i < 30; i++)
    {
        // initialise all motors to their home position
        init_split_flap();

        // display the message on the split flap display
        display_message(strings[i]);

        delay_ms(1000);
    }

    delay_ms(1000);

    // disable all motors
    toggle_motor(0);
    toggle_motor(1);
    toggle_motor(2);
    toggle_motor(3);
    toggle_motor(4);
}