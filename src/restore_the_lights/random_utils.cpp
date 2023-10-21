#include <time.h>
#include <stdlib.h>
#include <inttypes.h>
#include "random_utils.h"
#include "Arduino.h"

static bool isInit = false;

/*********************************************************/
/*                                                       */
/*                   Utility functions                   */
/*                                                       */
/*********************************************************/

static void swap (unsigned long *a, unsigned long *b) {
  unsigned long temp = *a;
  *a = *b;
  *b = temp;
}

static void shuffle(unsigned long array[], uint8_t n) {
  for (uint8_t i = n - 1; i > 0; i--) {
    // Picks a random index from 0 to i.
    const uint8_t j = rand() % (i + 1);
    swap(&array[i], &array[j]);
  }
}

static unsigned long *generate_array(const uint8_t size) {
  unsigned long *array = (unsigned long *) malloc(size);
  if (array == NULL) {
    return NULL;
  }
  for (uint8_t i = 0; i < size; i++) {
    array[i] = i;
  }
  return array;
}

/*********************************************************/
/*                                                       */
/*                 Header file functions                 */
/*                                                       */
/*********************************************************/

void rand_init() {
  srand(time(NULL));
  isInit = true;
}

unsigned long get_rand(const unsigned int range_end) {
  return isInit ? rand() % range_end : range_end;
}

unsigned long *get_rand_multiple(const unsigned int size) {
  if (!isInit) {
    return NULL;
  }
  unsigned long *result = generate_array(size);
  shuffle(result, size);
  return result;
}

void free_rand_array(unsigned long *array) {
  free(array);
}
