#include <time.h>
#include <stdlib.h>
#include "random_utils.h"

unsigned long get_rand(const unsigned int range_end) {
  srand(time(NULL));
  return rand() % range_end;
}

unsigned long* get_rand_multiple(const unsigned int times, const unsigned int range_end) {
  unsigned long *result = (unsigned long *) malloc(times);
  for (unsigned int i = 0; i < times; i++) {
    result[i] = get_rand(range_end);
  }
  return result;
}

void free_rand_array(unsigned long *array) {
  free(array);
}
