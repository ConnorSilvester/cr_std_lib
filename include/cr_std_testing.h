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

test_case_t *cr_std_testing_new_test(const char *name, int (*test_function)(void));
void cr_std_testing_run_tests(vector_t *tests);

#endif // CR_STD_TESTING_H
