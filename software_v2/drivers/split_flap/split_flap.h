#ifndef DRIVERS_SPLIT_FLAP_H_
#define DRIVERS_SPLIT_FLAP_H_

#define FLAPS           (40) /* number of flaps on each module */
#define STEPS           (200) /* number of steps on each stepper motor */
#define MODULE_COUNT    (5)

uint8_t module_positions[MODULE_COUNT];

int8_t init_split_flap(void);
void display_message(char *message);

#endif