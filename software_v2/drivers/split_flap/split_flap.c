/**
 * @file split_flap.c
 * @author Starman
 * @brief Standard functions for interacting with the split-flap display
 * @version 0.1
 * @date 2021-07-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "./split_flap.h"
#include "../main.h"

/**
 * @brief Checks all positions of the motors to see if they match the specified position
 * 
 * @param display_positions A pointer to an array of all the current motor positions
 * @param position The position to check for
 * @return true All modules match the specified position
 * @return false Not all modules match the specified position
 */
bool check_all_at_position(uint8_t *display_positions, uint8_t position)
{
    for (uint8_t i = 0; i < MODULE_COUNT; i++)
        if (*(display_positions + i) != position)
            return false;
    return true;
}

/**
 * @brief Defines the position of all modules.
 * 
 * @param display_positions A pointer to an array of positions for each flap.
 * @param position The position to define all modules to.
 */
void set_all_positions(uint8_t *display_positions, uint8_t position)
{
    for (uint8_t i = 0; i < MODULE_COUNT; i++)
        *(display_positions + i) = position;
}

/**
 * @brief Initialises each display by defining position 0 to a known position
 * 
 * @param display_positions 
 */
void init_split_flap(uint8_t *display_positions)
{
    set_all_positions(display_positions, 1);
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
    }
    while (notAtHome != MODULE_COUNT);
    while (!check_all_at_position(display_positions, 0))
    {
        for (uint8_t i = 0; i < MODULE_COUNT; i++)
        {
            if (*(display_positions + i) != 0)
            {
                write_motor(i);
                write_motor(i);
                if (read_hall_effect_sensor(i))
                    *(display_positions + i) = 0;
            }
        }
        delay_ms(10);
    }
}

/**
 * @brief Converts a character to a position on the display
 * 
 * @param character 
 * @return uint8_t The motor position of the character on the display
 */
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

/**
 * @brief Converts a string of characters to an array of motor positions
 * 
 * @param message 
 * @param positions 
 */
void convert_string_to_positions(char *message, uint8_t *positions)
{
    for (uint8_t i = 0; i < MODULE_COUNT; i++)
    {
        if (strlen(message) <= i)
            *(positions + i) = 0;
        else
            *(positions + i) = convert_char_to_position(&message[i]);
    }
}

/**
 * @brief Checks that all motor positions match the required positions
 * 
 * @param requiredPositions 
 * @param actualPositions 
 * @return true All positions have been reached
 * @return false Not all positions have been reached
 */
bool check_positions_have_been_reached(uint8_t *requiredPositions, uint8_t *actualPositions)
{
    for (uint8_t i = 0; i < MODULE_COUNT; i++)
        if (*(requiredPositions + i) != *(actualPositions + i))
            return false;
    return true;
}

/**
 * @brief Displays a message on the split-flap display
 * 
 * @param message 
 * @param currentPositions 
 */
void display_message(char *message, uint8_t *currentPositions)
{
    uint8_t requiredPositions[MODULE_COUNT];
    convert_string_to_positions(message, requiredPositions);
    uint8_t increment[MODULE_COUNT];
    for (uint8_t i = 0; i < MODULE_COUNT; i++)
        increment[i] = 0;
    while (!check_positions_have_been_reached(requiredPositions, currentPositions))
    {
        for (uint8_t i = 0; i < MODULE_COUNT; i++)
        {
            if (requiredPositions[i] != *(currentPositions + i))
            {
                write_motor(i);
                write_motor(i);
                increment[i]++;
                if (increment[i] == (STEPS / FLAPS))
                {
                    *(currentPositions + i) == 39 ? *(currentPositions + i) = 0 : (*(currentPositions + i))++;
                    increment[i] = 0;
                }
            }
        }
        delay_ms(10);
    }
}