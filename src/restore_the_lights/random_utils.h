#ifndef RANDOM_UTILS_H_
#define RANDOM_UTILS_H_

#include <inttypes.h>

// Sets up the random generator by calling srand(time(NULL)).
void rand_init();

/*
 * Returns a random unsigned long number generated in the range [0, range_end).
 * In case of any error, 'range_end' is returned.
 */
uint8_t get_rand(const uint8_t range_end);

// Returns an array of length 'size' of unsigned long numbers generated in the range [0, size).
uint8_t *get_rand_multiple(const uint8_t size);

// Frees the memory allocated by 'get_rand_multiple(const unsigned int times, const unsigned int range_end)'.
void free_rand_array(uint8_t *array);

#endif
