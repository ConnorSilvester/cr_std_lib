#include "cr_std_testing.h"
#include "cr_std_logger.h"
#include "cr_std_vector.h"
#include <stdio.h>
#include <stdlib.h>

test_case_t *cr_std_testing_new_test(const char *name, int (*test_function)(void)) {
    test_case_t *new_test = (test_case_t *)malloc(sizeof(test_case_t));
    if (!new_test) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_testing_new_test -> failed to allocate memory for new test_case_t struct");
        return NULL;
    }

    new_test->name = name;
    new_test->test_function = test_function;

    return new_test;
}

void cr_std_testing_run_tests(vector_t *tests) {
    vector_t *list_of_errors = cr_std_vector_new_n(sizeof(test_case_t *));
    printf("--------------------------------------------------------\n");
    for (int i = 0; i < tests->size; i++) {
        test_case_t *test = (test_case_t *)cr_std_vector_get_element(tests, i);

        if (test) {
            if (tests->size >= 10 && i + 1 < 10) {
                printf("Running test (0%d / %ld) : %s ", i + 1, tests->size, test->name);
            } else {
                printf("Running test (%d / %ld) : %s ", i + 1, tests->size, test->name);
            }
            int result = test->test_function();
            if (result) {
                printf("%s\n", TEST_PASSED);
            } else {
                printf("%s\n", TEST_FAILED);
                cr_std_vector_push_back(list_of_errors, test);
            }
        }
    }
    size_t total_passed = tests->size - list_of_errors->size;
    printf("\n\033[32mTOTAL_PASSED\033[0m (%ld / %ld)\n", total_passed, tests->size);
    if (list_of_errors->size > 0) {
        printf("\033[31mTOTAL_FAILED\033[0m (%ld / %ld)\n\n", list_of_errors->size, tests->size);
        for (int i = 0; i < list_of_errors->size; i++) {
            test_case_t *test = (test_case_t *)cr_std_vector_get_element(list_of_errors, i);
            printf("\033[31mFailed Test\033[0m : %s\n", test->name);
        }
    }
    free(list_of_errors);
}
