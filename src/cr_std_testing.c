#include "cr_std_testing.h"
#include "cr_std_arena.h"
#include "cr_std_logger.h"
#include "cr_std_vector.h"
#include <stdio.h>
#include <stdlib.h>

TestCase *cr_std_testing_new_test(Arena *arena, const char *name, b8 (*test_function)(void)) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_testing_new_test -> arena* is NULL");
        return NULL;
    }

    if (!name) {
        CR_LOG_ERROR("cr_std_testing_new_test -> name* is NULL");
        return NULL;
    }

    TestCase *new_test = cr_std_arena_alloc(arena, sizeof(*new_test));
    if (!new_test) {
        CR_LOG_ERROR("cr_std_testing_new_test -> arena alloc for new TestCase struct");
        return NULL;
    }

    new_test->name = name;
    new_test->test_function = test_function;

    return new_test;
}

b8 cr_std_testing_run_tests(Arena *arena, Vector *tests) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_testing_run_tests -> arena* is NULL");
        return CR_STD_FAIL;
    }

    if (!tests) {
        CR_LOG_ERROR("cr_std_testing_run_tests -> tests* is NULL");
        return CR_STD_FAIL;
    }

    Vector *list_of_errors = cr_std_vector_new(arena);
    if (!list_of_errors) {
        return CR_STD_FAIL;
    }

    printf("--------------------------------------------------------\n");
    for (size_t i = 0; i < tests->size; i++) {
        TestCase *test = cr_std_vector_get_at(tests, TestCase, i);

        if (test) {
            if (tests->size >= 10 && i + 1 < 10) {
                printf("Running test (0%d / %ld) : %s ", i + 1, tests->size, test->name);
            } else {
                printf("Running test (%d / %ld) : %s ", i + 1, tests->size, test->name);
            }
            b8 result = test->test_function();
            if (result) {
                printf("%s\n", TEST_PASSED);
            } else {
                printf("%s\n", TEST_FAILED);
                cr_std_vector_push_back(arena, list_of_errors, test);
            }
        }
    }
    size_t total_passed = tests->size - list_of_errors->size;
    printf("\n\033[32mTOTAL_PASSED\033[0m (%ld / %ld)\n", total_passed, tests->size);
    if (list_of_errors->size > 0) {
        printf("\033[31mTOTAL_FAILED\033[0m (%ld / %ld)\n\n", list_of_errors->size, tests->size);
        for (int i = 0; i < list_of_errors->size; i++) {
            TestCase *test = cr_std_vector_get_at(list_of_errors, TestCase, i);
            printf("\033[31mFailed Test\033[0m : %s\n", test->name);
        }
    }
    return CR_STD_OK;
}
