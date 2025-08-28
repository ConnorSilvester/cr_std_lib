#ifndef CR_STD_TESTING_H
#define CR_STD_TESTING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#define TEST_PASSED "\033[32mPASSED\033[0m"
#define TEST_FAILED "\033[31mFAILED\033[0m"

typedef struct Vector Vector;

/**
 * @brief Represents a test case.
 *
 * Includes a const char * which is not allocated or freed.
 *
 * Memory ownership:
 * - This struct owns no memory.
 */
typedef struct TestCase {
    const char *name;
    int (*test_function)(void); // Function that returns 1 for pass, 0 for fail
} TestCase;

/**
 * @brief Creates a new `TestCase` struct.
 *
 * @param `name` The name of the test.
 * @param `test_function` The custom test function to be called when running the test.
 *
 * @return A pointer to the new `TestCase` struct.
 * @return `NULL` if allocation fails.
 */
TestCase *cr_std_testing_new_test(const char *name, int (*test_function)(void));

/**
 * @brief Runs the list of tests passed in.
 *
 * @param `tests` A pointer to a `Vector` struct containing the list of `TestCase` structs
 */
void cr_std_testing_run_tests(Vector *tests);

#ifdef __cplusplus
}
#endif
#endif // CR_STD_TESTING_H
