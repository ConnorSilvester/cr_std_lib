#ifndef CR_STD_TESTING_H
#define CR_STD_TESTING_H
#include <stdio.h>

#define TEST_PASSED "\033[32mPASSED\033[0m"
#define TEST_FAILED "\033[31mFAILED\033[0m"

typedef struct vector_t vector_t;
typedef struct test_case_t {
    const char *name;
    int (*test_function)(void); // Function that returns 1 for pass, 0 for fail
} test_case_t;

/**
 * @brief Creates a new `test_case_t` struct.
 *
 * @param `name` The name of the test.
 * @param `test_function` The custom test function to be called when running the test.
 *
 * @return A pointer to the new `test_case_t` struct.
 * @return `NULL` if allocation fails.
 */
test_case_t *cr_std_testing_new_test(const char *name, int (*test_function)(void));

/**
 * @brief Runs the list of tests passed in.
 *
 * @param `tests` A pointer to a `vector_t` struct containing the list of `test_case_t` structs
 */
void cr_std_testing_run_tests(vector_t *tests);

#endif // CR_STD_TESTING_H
