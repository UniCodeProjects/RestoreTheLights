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

static void swap (uint8_t *a, uint8_t *b) {
  unsigned long temp = *a;
  *a = *b;
  *b = temp;
}

static void shuffle(uint8_t array[], uint8_t n) {
  for (uint8_t i = n - 1; i > 0; i--) {
    // Picks a random index from 0 to i.
    const uint8_t j = rand() % (i + 1);
    swap(&array[i], &array[j]);
  }
}

static uint8_t *generate_array(const uint8_t size) {
  uint8_t *array = (uint8_t *)malloc(size * sizeof(uint8_t));
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

uint8_t get_rand(const uint8_t range_end) {
  return isInit ? rand() % range_end : range_end;
}

uint8_t *get_rand_multiple(const uint8_t size) {
  if (!isInit) {
    return NULL;
  }
  uint8_t *result = generate_array(size);
  shuffle(result, size);
  return result;
}

void free_rand_array(uint8_t *array) {
  free(array);
}
