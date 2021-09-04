#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "drivers/split_flap/split_flap.h"
#include "drivers/hall_effect_sensor/hall_effect_sensor.h"
#include "drivers/stepper_motor/stepper_motor.h"
#include "util/delay/delay.h"
#include "main.h"

// checks if all modules are at the specified position
bool check_all_at_position(uint8_t position)
{
    for (uint8_t i = 0; i < MODULE_COUNT; i++)
        if (module_positions[i] != position)
            return false;
    return true;
}

// defines the position of each module
void set_all_positions(uint8_t position)
{
    for (uint8_t i = 0; i < MODULE_COUNT; i++)
        module_positions[i] = position;
}

int8_t init_split_flap(void)
{
    set_all_positions(1);
    uint8_t notAtHome;
    do
    {
        notAtHome = 0;
        for (uint8_t i = 0; i < MODULE_COUNT; i++)
            if (read_hall_effect_sensor(i))
            {
                write_motor(i);
                write_motor(i);
                if (!read_hall_effect_sensor(i))
                    notAtHome++;
            }
            else
                notAtHome++;
        delay_ms(10);
    } while (notAtHome != MODULE_COUNT);
    while (!check_all_at_position(0))
    {
        for (uint8_t i = 0; i < MODULE_COUNT; i++)
        {
            if (module_positions[i] != 0)
            {
                write_motor(i);
                write_motor(i);
                if (read_hall_effect_sensor(i))
                    module_positions[i] = 0;
            }
        }
        delay_ms(10);
    }

    return 0;
}

uint8_t convert_char_to_position(char *character)
{
    *character = toupper(*character);
    switch (*character)
    {
    case 'A':
        return 1;
    case 'B':
        return 2;
    case 'C':
        return 3;
    case 'D':
        return 4;
    case 'E':
        return 5;
    case 'F':
        return 6;
    case 'G':
        return 7;
    case 'H':
        return 8;
    case 'I':
        return 9;
    case 'J':
        return 10;
    case 'K':
        return 11;
    case 'L':
        return 12;
    case 'M':
        return 13;
    case 'N':
        return 14;
    case 'O':
        return 15;
    case 'P':
        return 16;
    case 'Q':
        return 17;
    case 'R':
        return 18;
    case 'S':
        return 19;
    case 'T':
        return 20;
    case 'U':
        return 21;
    case 'V':
        return 22;
    case 'W':
        return 23;
    case 'X':
        return 24;
    case 'Y':
        return 25;
    case 'Z':
        return 26;
    case '0':
        return 27;
    case '1':
        return 28;
    case '2':
        return 29;
    case '3':
        return 30;
    case '4':
        return 31;
    case '5':
        return 32;
    case '6':
        return 33;
    case '7':
        return 34;
    case '8':
        return 35;
    case '9':
        return 36;
    case ',':
        return 37;
    case ':':
        return 38;
    case '.':
        return 39;
    default:
        return 0;
    }
}

// converts a string to an array of motor positions
void convert_string_to_positions(char *message, uint8_t positions[])
{
    for (uint8_t i = 0; i < MODULE_COUNT; i++)
    {
        if (strlen(message) <= i)
            positions[i] = 0;
        else
            positions[i] = convert_char_to_position(&message[i]);
    }
}

// check each module is at it's own specified position
bool check_positions_have_been_reached(uint8_t requiredPositions[])
{
    for (uint8_t i = 0; i < MODULE_COUNT; i++)
        if (requiredPositions[i] != module_positions[i])
            return false;
    return true;
}

void display_message(char *message)
{
    uint8_t requiredPositions[MODULE_COUNT];
    convert_string_to_positions(message, requiredPositions);
    uint8_t increment[MODULE_COUNT];
    for (uint8_t i = 0; i < MODULE_COUNT; i++)
        increment[i] = 0;
    while (!check_positions_have_been_reached(requiredPositions))
    {
        for (uint8_t i = 0; i < MODULE_COUNT; i++)
        {
            if (requiredPositions[i] != module_positions[i])
            {
                write_motor(i);
                write_motor(i);
                increment[i]++;
                if (increment[i] == (STEPS / FLAPS))
                {
                    module_positions[i] == 39 ? module_positions[i] = 0 : module_positions[i]++;
                    increment[i] = 0;
                }
            }
        }
        delay_ms(10);
    }
}