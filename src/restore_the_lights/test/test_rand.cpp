#include <stdio.h>
#include "test_macros.h"
#include "../random_utils.h"

// #define DEBUG

#define MAX_RANGE 5

// Test functions declarations.
bool rand_init_test();
bool get_rand_test();
bool get_rand_multiple_test();

int main() {
    PRINT_TEST_STARTED("rand_init_test");
    if (!rand_init_test()) return 1;
    rand_init();
    PRINT_TEST_STARTED("get_rand_test");
    if (!get_rand_test()) return 1;
    PRINT_TEST_STARTED("get_rand_multiple_test");
    if (!get_rand_multiple_test()) return 1;
    PRINT_TEST_PASSED("ALL TESTS");
    return 0;
}

bool rand_init_test() {
    unsigned long num = get_rand(MAX_RANGE);
    unsigned long* num_array = get_rand_multiple(2, MAX_RANGE);
    if (num == MAX_RANGE && num_array == NULL) {
        PRINT_TEST_PASSED("rand_init_test");
        return true;
    }
    PRINT_TEST_FAILED("rand_init_test");
    return false;
}

bool get_rand_test() {
    for (int i = 0; i < 50; i++) {
        unsigned long result = get_rand(MAX_RANGE);
#ifdef DEBUG
        printf("[DEBUG%d] Generated: %lu\n", i, result);
#endif
        if (result >= MAX_RANGE) {
            PRINT_TEST_FAILED("get_rand_test");
            return false;
        }
    }
    PRINT_TEST_PASSED("get_rand_test");
    return true;
}

bool get_rand_multiple_test() {
    for (int i = 0; i < 5; i++) {
        unsigned long *result = get_rand_multiple(7, MAX_RANGE);
        for (int j = 0; j < (int) sizeof(result) - 1; j++) {
#ifdef DEBUG
            printf("[DEBUG%d] Generated: %lu\n", j, result[j]);
#endif
            if (result[i] >= MAX_RANGE) {
                PRINT_TEST_FAILED("get_rand_multiple_test");
                free_rand_array(result);
                return false;
            }
        }
        free_rand_array(result);
    }
    PRINT_TEST_PASSED("get_rand_multiple_test");
    return true;
}
