#ifndef TEST_MACROS_H_
#define TEST_MACROS_H_

/*
 * Prints on stdout a test started message.
 *
 * Param:
 * - str: the test function name
 */
#define PRINT_TEST_STARTED(str) printf("%s STARTED\n", str)

/*
 * Prints on stdout a test passed message.
 *
 * Param:
 * - str: the test function name
 */
#define PRINT_TEST_PASSED(str) printf("%s PASSED\n", str)

/*
 * Prints on stdout a test failed message.
 *
 * Param:
 * - str: the test function name
 */
#define PRINT_TEST_FAILED(str) printf("%s FAILED\n", str)

#endif
