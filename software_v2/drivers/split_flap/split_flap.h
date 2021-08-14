/**
 * @file split_flap.h
 * @author Starman
 * @brief Split flap header file
 * @version 0.1
 * @date 2021-07-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef SPLIT_FLAP_H
#define SPLIT_FLAP_H

#define FLAPS           (40) /* number of flaps on each module */
#define STEPS           (200) /* number of steps on each stepper motor */
#define MODULE_COUNT    (5)

uint8_t module_positions[MODULE_COUNT];

#endif