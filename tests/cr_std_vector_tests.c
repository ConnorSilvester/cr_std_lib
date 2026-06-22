#include "cr_std_vector_tests.h"
#include "cr_std_arena.h"
#include "cr_std_string.h"
#include "cr_std_testing.h"
#include "cr_std_utils.h"
#include "cr_std_vector.h"
#include <stdio.h>
#include <stdlib.h>

Arena *g_vec_testing_arena = NULL;

void cr_std_vector_test_all() {
    printf("%s\n", "Running Vector Tests:");

    g_vec_testing_arena = cr_std_arena_new(CR_STD_MB);
    Vector *tests = cr_std_vector_new(g_vec_testing_arena);

    // clang-format off
    // New Vector
    cr_std_vector_push_back(g_vec_testing_arena, tests, cr_std_testing_new_test(g_vec_testing_arena, "Make Vector -> Primitive", cr_std_vector_test_new_vector_normal));

    // Push Back
    cr_std_vector_push_back(g_vec_testing_arena, tests, cr_std_testing_new_test(g_vec_testing_arena, "Push Back -> Primitive", cr_std_vector_test_push_back_primitive));
    cr_std_vector_push_back(g_vec_testing_arena, tests, cr_std_testing_new_test(g_vec_testing_arena, "Push Back -> Custom", cr_std_vector_test_push_back_custom));

    // Remove Element
    cr_std_vector_push_back(g_vec_testing_arena, tests, cr_std_testing_new_test(g_vec_testing_arena, "Remove Element -> Primitive", cr_std_vector_test_remove_element_primitive));
    cr_std_vector_push_back(g_vec_testing_arena, tests, cr_std_testing_new_test(g_vec_testing_arena, "Remove Element -> Custom", cr_std_vector_test_remove_element_custom));
    cr_std_vector_push_back(g_vec_testing_arena, tests, cr_std_testing_new_test(g_vec_testing_arena, "Remove Element -> Invalid Vector", cr_std_vector_test_remove_element_invalid_vector));
    cr_std_vector_push_back(g_vec_testing_arena, tests, cr_std_testing_new_test(g_vec_testing_arena, "Remove Element -> Invalid Index", cr_std_vector_test_remove_element_invalid_index));
    cr_std_vector_push_back(g_vec_testing_arena, tests, cr_std_testing_new_test(g_vec_testing_arena, "Remove Element -> Invalid Index Negative", cr_std_vector_test_remove_element_invalid_index_negative));
    cr_std_vector_push_back(g_vec_testing_arena, tests, cr_std_testing_new_test(g_vec_testing_arena, "Remove Element -> Empty Vector", cr_std_vector_test_remove_element_empty_vector));

    // Get Element
    cr_std_vector_push_back(g_vec_testing_arena, tests, cr_std_testing_new_test(g_vec_testing_arena, "Get Element -> Primitive", cr_std_vector_test_get_element_primitive));
    cr_std_vector_push_back(g_vec_testing_arena, tests, cr_std_testing_new_test(g_vec_testing_arena, "Get Element -> Custom", cr_std_vector_test_get_element_custom));
    cr_std_vector_push_back(g_vec_testing_arena, tests, cr_std_testing_new_test(g_vec_testing_arena, "Get Element -> Invalid Vector", cr_std_vector_test_get_element_invalid_vector));
    cr_std_vector_push_back(g_vec_testing_arena, tests, cr_std_testing_new_test(g_vec_testing_arena, "Get Element -> Invalid Index", cr_std_vector_test_get_element_invalid_index));
    cr_std_vector_push_back(g_vec_testing_arena, tests, cr_std_testing_new_test(g_vec_testing_arena, "Get Element -> Invalid Index Negative", cr_std_vector_test_get_element_invalid_index_negative));
    cr_std_vector_push_back(g_vec_testing_arena, tests, cr_std_testing_new_test(g_vec_testing_arena, "Get Element -> Empty Vector", cr_std_vector_test_get_element_empty_vector));

    // Extend Vector
    cr_std_vector_push_back(g_vec_testing_arena, tests, cr_std_testing_new_test(g_vec_testing_arena, "Extend Vector -> Primitive", cr_std_vector_test_extend_primitive));
    cr_std_vector_push_back(g_vec_testing_arena, tests, cr_std_testing_new_test(g_vec_testing_arena, "Extend Vector -> Custom", cr_std_vector_test_extend_custom));

    // clang-format on
    cr_std_testing_run_tests(g_vec_testing_arena, tests);
    cr_std_arena_free(&g_vec_testing_arena);
}

b8 cr_std_vector_test_new_vector_normal() {
    Vector *vector = cr_std_vector_new(g_vec_testing_arena);
    int result = vector != NULL;
    return result;
}

b8 cr_std_vector_test_push_back_primitive() {
    Vector *vector = cr_std_vector_new(g_vec_testing_arena);
    int expected_size = 5;

    cr_std_vector_push_back(g_vec_testing_arena, vector, "Hello");
    cr_std_vector_push_back(g_vec_testing_arena, vector, "Hello");
    cr_std_vector_push_back(g_vec_testing_arena, vector, "Hello");
    cr_std_vector_push_back(g_vec_testing_arena, vector, "Hello");
    cr_std_vector_push_back(g_vec_testing_arena, vector, "Hello");
    int result = vector != NULL && expected_size == vector->size;
    return result;
}

b8 cr_std_vector_test_push_back_custom() {
    Vector *vector = cr_std_vector_new(g_vec_testing_arena);

    int expected_size = 4;
    String *string_1 = cr_std_string_new(g_vec_testing_arena, "Hello World");
    String *string_2 = cr_std_string_new(g_vec_testing_arena, "Hello World");
    String *string_3 = cr_std_string_new(g_vec_testing_arena, "Hello World");
    String *string_4 = cr_std_string_new(g_vec_testing_arena, "Hello World");

    cr_std_vector_push_back(g_vec_testing_arena, vector, string_1);
    cr_std_vector_push_back(g_vec_testing_arena, vector, string_2);
    cr_std_vector_push_back(g_vec_testing_arena, vector, string_3);
    cr_std_vector_push_back(g_vec_testing_arena, vector, string_4);
    int result = vector != NULL && expected_size == vector->size;
    return result;
}

b8 cr_std_vector_test_remove_element_primitive() {
    Vector *vector = cr_std_vector_new(g_vec_testing_arena);
    int expected_size = 3;
    char *string_1 = "Hello";
    char *string_2 = "Hello";
    char *string_3 = "Hello";
    char *string_4 = "Hello";
    char *string_5 = "Hello";

    cr_std_vector_push_back(g_vec_testing_arena, vector, string_1);
    cr_std_vector_push_back(g_vec_testing_arena, vector, string_2);
    cr_std_vector_push_back(g_vec_testing_arena, vector, string_3);
    cr_std_vector_push_back(g_vec_testing_arena, vector, string_4);
    cr_std_vector_push_back(g_vec_testing_arena, vector, string_5);

    cr_std_vector_remove_element(vector, 0);
    cr_std_vector_remove_element(vector, 1);
    int result = vector != NULL && expected_size == vector->size;
    return result;
}

b8 cr_std_vector_test_remove_element_custom() {
    Vector *vector = cr_std_vector_new(g_vec_testing_arena);

    int expected_size = 2;
    String *string_1 = cr_std_string_new(g_vec_testing_arena, "Hello World");
    String *string_2 = cr_std_string_new(g_vec_testing_arena, "Hello World");
    String *string_3 = cr_std_string_new(g_vec_testing_arena, "Hello World");
    String *string_4 = cr_std_string_new(g_vec_testing_arena, "Hello World");

    cr_std_vector_push_back(g_vec_testing_arena, vector, string_1);
    cr_std_vector_push_back(g_vec_testing_arena, vector, string_2);
    cr_std_vector_push_back(g_vec_testing_arena, vector, string_3);
    cr_std_vector_push_back(g_vec_testing_arena, vector, string_4);

    cr_std_vector_remove_element(vector, 0);
    cr_std_vector_remove_element(vector, 1);
    int result = vector != NULL && expected_size == vector->size;
    return result;
}

b8 cr_std_vector_test_remove_element_invalid_vector() {
    Vector *vector = NULL;

    int function_result = cr_std_vector_remove_element(vector, 0);
    int result = function_result != 0;
    return result;
}

b8 cr_std_vector_test_remove_element_invalid_index() {
    Vector *vector = cr_std_vector_new(g_vec_testing_arena);

    int expected_size = 4;
    String *string_1 = cr_std_string_new(g_vec_testing_arena, "Hello World");
    String *string_2 = cr_std_string_new(g_vec_testing_arena, "Hello World");
    String *string_3 = cr_std_string_new(g_vec_testing_arena, "Hello World");
    String *string_4 = cr_std_string_new(g_vec_testing_arena, "Hello World");

    cr_std_vector_push_back(g_vec_testing_arena, vector, string_1);
    cr_std_vector_push_back(g_vec_testing_arena, vector, string_2);
    cr_std_vector_push_back(g_vec_testing_arena, vector, string_3);
    cr_std_vector_push_back(g_vec_testing_arena, vector, string_4);

    int function_result = cr_std_vector_remove_element(vector, 8);
    int result = vector != NULL && expected_size == vector->size && function_result != 0;
    return result;
}

b8 cr_std_vector_test_remove_element_invalid_index_negative() {
    Vector *vector = cr_std_vector_new(g_vec_testing_arena);

    int expected_size = 0;
    int function_result = cr_std_vector_remove_element(vector, -2);
    int result = vector != NULL && expected_size == vector->size && function_result != 0;
    return result;
}

b8 cr_std_vector_test_remove_element_empty_vector() {
    Vector *vector = cr_std_vector_new(g_vec_testing_arena);

    int expected_size = 0;
    int function_result = cr_std_vector_remove_element(vector, 0);
    int result = vector != NULL && expected_size == vector->size && function_result != 0;
    return result;
}

b8 cr_std_vector_test_get_element_primitive() {
    Vector *vector = cr_std_vector_new(g_vec_testing_arena);
    int expected_size = 5;
    int number_1 = 1;
    int number_2 = 2;
    int number_3 = 3;
    int number_4 = 4;
    int number_5 = 5;

    cr_std_vector_push_back(g_vec_testing_arena, vector, &number_1);
    cr_std_vector_push_back(g_vec_testing_arena, vector, &number_2);
    cr_std_vector_push_back(g_vec_testing_arena, vector, &number_3);
    cr_std_vector_push_back(g_vec_testing_arena, vector, &number_4);
    cr_std_vector_push_back(g_vec_testing_arena, vector, &number_5);

    // Change the number
    int *number = cr_std_vector_get_at(vector, int, 2);
    *number = 10;

    int actual_number = *cr_std_vector_get_at(vector, int, 2);

    int result =
    vector != NULL && expected_size == vector->size && actual_number == 10 && number_3 == 10;
    return result;
}

b8 cr_std_vector_test_get_element_custom() {
    Vector *vector = cr_std_vector_new(g_vec_testing_arena);

    int expected_size = 4;
    String *string_1 = cr_std_string_new(g_vec_testing_arena, "String 1");
    String *string_2 = cr_std_string_new(g_vec_testing_arena, "String 2");
    String *string_3 = cr_std_string_new(g_vec_testing_arena, "String 3");
    String *string_4 = cr_std_string_new(g_vec_testing_arena, "String 4");

    cr_std_vector_push_back(g_vec_testing_arena, vector, string_1);
    cr_std_vector_push_back(g_vec_testing_arena, vector, string_2);
    cr_std_vector_push_back(g_vec_testing_arena, vector, string_3);
    cr_std_vector_push_back(g_vec_testing_arena, vector, string_4);

    String *expected_string = cr_std_string_new(g_vec_testing_arena, "String 3");
    String *string = cr_std_vector_get_at(vector, String, 2);

    int function_result = cr_std_string_compare(string, expected_string);
    int result = vector != NULL && expected_size == vector->size && function_result == 1;
    return result;
}

b8 cr_std_vector_test_get_element_invalid_vector() {
    Vector *vector = NULL;

    int result = cr_std_vector_get_element(vector, 0) == NULL;
    return result;
}

b8 cr_std_vector_test_get_element_invalid_index() {
    Vector *vector = cr_std_vector_new(g_vec_testing_arena);

    int expected_size = 4;
    String *string_1 = cr_std_string_new(g_vec_testing_arena, "Hello World");
    String *string_2 = cr_std_string_new(g_vec_testing_arena, "Hello World");
    String *string_3 = cr_std_string_new(g_vec_testing_arena, "Hello World");
    String *string_4 = cr_std_string_new(g_vec_testing_arena, "Hello World");

    cr_std_vector_push_back(g_vec_testing_arena, vector, string_1);
    cr_std_vector_push_back(g_vec_testing_arena, vector, string_2);
    cr_std_vector_push_back(g_vec_testing_arena, vector, string_3);
    cr_std_vector_push_back(g_vec_testing_arena, vector, string_4);

    void *function_result = cr_std_vector_get_element(vector, 8);
    int result = vector != NULL && expected_size == vector->size && function_result == NULL;
    return result;
}

b8 cr_std_vector_test_get_element_invalid_index_negative() {
    Vector *vector = cr_std_vector_new(g_vec_testing_arena);

    int expected_size = 0;
    void *function_result = cr_std_vector_get_element(vector, -2);
    int result = vector != NULL && expected_size == vector->size && function_result == NULL;
    return result;
}

b8 cr_std_vector_test_get_element_empty_vector() {
    Vector *vector = cr_std_vector_new(g_vec_testing_arena);

    int expected_size = 0;
    void *function_result = cr_std_vector_get_element(vector, -2);
    int result = vector != NULL && expected_size == vector->size && function_result == NULL;
    return result;
}

b8 cr_std_vector_test_extend_primitive() {
    Vector *dest = cr_std_vector_new(g_vec_testing_arena);
    Vector *src = cr_std_vector_new(g_vec_testing_arena);

    int expected_size = 5;
    char *string_1 = "Hello";
    char *string_2 = "Hello";
    char *string_3 = "Hello";
    char *string_4 = "Hello";
    char *string_5 = "Hello";

    cr_std_vector_push_back(g_vec_testing_arena, dest, string_1);
    cr_std_vector_push_back(g_vec_testing_arena, dest, string_2);
    cr_std_vector_push_back(g_vec_testing_arena, dest, string_3);

    cr_std_vector_push_back(g_vec_testing_arena, src, string_4);
    cr_std_vector_push_back(g_vec_testing_arena, src, string_5);

    int function_result = cr_std_vector_extend(g_vec_testing_arena, dest, src);

    char *string = cr_std_vector_get_at(dest, char, 3);
    int result = dest != NULL && expected_size == dest->size && string != NULL &&
                 function_result == 0;
    return result;
}

b8 cr_std_vector_test_extend_custom() {
    Vector *dest = cr_std_vector_new(g_vec_testing_arena);
    Vector *src = cr_std_vector_new(g_vec_testing_arena);

    int expected_size = 4;
    String *string_1 = cr_std_string_new(g_vec_testing_arena, "String 1");
    String *string_2 = cr_std_string_new(g_vec_testing_arena, "String 2");
    String *string_3 = cr_std_string_new(g_vec_testing_arena, "String 3");
    String *string_4 = cr_std_string_new(g_vec_testing_arena, "String 4");

    cr_std_vector_push_back(g_vec_testing_arena, dest, string_1);
    cr_std_vector_push_back(g_vec_testing_arena, dest, string_2);

    cr_std_vector_push_back(g_vec_testing_arena, src, string_3);
    cr_std_vector_push_back(g_vec_testing_arena, src, string_4);

    int extend_function_result = cr_std_vector_extend(g_vec_testing_arena, dest, src);

    String *expected_string = cr_std_string_new(g_vec_testing_arena, "String 4");
    String *string = cr_std_vector_get_at(dest, String, 3);

    int compare_function_result = cr_std_string_compare(string, expected_string);
    int result = dest != NULL && src != NULL && expected_size == dest->size &&
                 extend_function_result == 0 && compare_function_result == 1;
    return result;
}
