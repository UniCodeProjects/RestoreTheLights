#ifndef DIFFICULTY_H
#define DIFFICULTY_H

#include <inttypes.h>

// Returns the difficulty corresponding to the provided value.
uint8_t get_difficulty(const uint16_t value);

// Returns the factor of the provided difficulty.
float get_difficulty_factor(const uint8_t difficulty);

// Returns the led which corresponds to the provided difficulty.
uint8_t get_corresponding_led(const uint8_t difficulty);

// Shows the user the game difficulties.
void view_difficulties();

// Returns the difficulty chosen by the user.
uint8_t get_chosen_difficulty();

#endif
