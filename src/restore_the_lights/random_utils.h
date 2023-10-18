#ifndef RANDOM_UTILS_H_
#define RANDOM_UTILS_H_

// Returns a random unsigned long number generated in the range [0, range_end).
unsigned long get_rand(const unsigned int range_end);

// Returns an array of length 'times' of unsigned long numbers generated in the range [0, range_end).
unsigned long* get_rand_multiple(const unsigned int times, const unsigned int range_end);

// Frees the memory allocated by 'get_rand_multiple(const unsigned int times, const unsigned int range_end)'.
void free_rand_array(unsigned long* array);

#endif
