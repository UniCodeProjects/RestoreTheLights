#include <time.h>
#include <stdlib.h>
#include "random_utils.h"

bool isInit = false;

void rand_init() {
  srand(time(NULL));
  isInit = true;
}

unsigned long get_rand(const unsigned int range_end) {
  return isInit ? rand() % range_end : range_end;
}

unsigned long* get_rand_multiple(const unsigned int times, const unsigned int range_end) {
  if (!isInit) {
    return NULL;
  }
  unsigned long *result = (unsigned long *) malloc(times);
  for (unsigned int i = 0; i < times; i++) {
    result[i] = get_rand(range_end);
    if (result[i] == range_end) {
      return NULL;
    }
    
  }
  return result;
}

void free_rand_array(unsigned long *array) {
  free(array);
}
