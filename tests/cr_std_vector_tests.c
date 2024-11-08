#include "cr_std_vector_tests.h"
#include "cr_std_string.h"
#include "cr_std_testing.h"
#include "cr_std_utils.h"
#include "cr_std_vector.h"
#include <stdio.h>
#include <stdlib.h>

void cr_std_vector_test_all() {
    printf("%s\n", "Running Vector Tests:");

    vector_t *tests = cr_std_vector_new(sizeof(test_case_t *), cr_std_free_ptr, NULL);

    // New Vector
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Make Vector -> Primitive", cr_std_vector_test_new_vector_normal));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Make Vector -> Primitive Pointer", cr_std_vector_test_new_vector_normal_pointer));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Make Vector -> Custom", cr_std_vector_test_new_vector_custom));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Make Vector -> Custom Pointer", cr_std_vector_test_new_vector_custom_pointer));

    // Free Vector
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Free Vector -> Normal", cr_std_vector_test_free));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Free Vector -> Custom Free", cr_std_vector_test_free_pointer_vector));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Free Vector -> NULL Value", cr_std_vector_test_free_null));

    // Push Back
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Push Back -> Primitive", cr_std_vector_test_push_back_primitive));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Push Back -> Primitive Pointer", cr_std_vector_test_push_back_primitive_pointer));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Push Back -> Custom", cr_std_vector_test_push_back_custom));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Push Back -> Custom Pointer", cr_std_vector_test_push_back_custom_pointer));

    // Remove Element
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Remove Element -> Primitive", cr_std_vector_test_remove_element_primitive));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Remove Element -> Primitive Pointer", cr_std_vector_test_remove_element_primitive_pointer));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Remove Element -> Custom", cr_std_vector_test_remove_element_custom));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Remove Element -> Custom Pointer", cr_std_vector_test_remove_element_custom_pointer));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Remove Element -> Invalid Vector", cr_std_vector_test_remove_element_invalid_vector));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Remove Element -> Invalid Index", cr_std_vector_test_remove_element_invalid_index));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Remove Element -> Invalid Index Negative", cr_std_vector_test_remove_element_invalid_index_negative));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Remove Element -> Empty Vector", cr_std_vector_test_remove_element_empty_vector));

    // Get Element
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Get Element -> Primitive", cr_std_vector_test_get_element_primitive));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Get Element -> Primitive Pointer", cr_std_vector_test_get_element_primitive_pointer));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Get Element -> Custom", cr_std_vector_test_get_element_custom));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Get Element -> Custom Pointer", cr_std_vector_test_get_element_custom_pointer));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Get Element -> Invalid Vector", cr_std_vector_test_get_element_invalid_vector));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Get Element -> Invalid Index", cr_std_vector_test_get_element_invalid_index));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Get Element -> Invalid Index Negative", cr_std_vector_test_get_element_invalid_index_negative));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Get Element -> Empty Vector", cr_std_vector_test_get_element_empty_vector));

    // Get Element
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Extend Vector -> Primitive", cr_std_vector_test_extend_primitive_type));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Extend Vector -> Primitive Pointer", cr_std_vector_test_extend_primitive_pointer));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Extend Vector -> Custom", cr_std_vector_test_extend_custom));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Extend Vector -> Custom Pointer", cr_std_vector_test_extend_custom_pointer));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Extend Vector -> Custom Pointer Copy Elements", cr_std_vector_test_extend_custom_pointer_copy_elements));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Extend Vector -> Incorrect Types", cr_std_vector_test_extend_incorrect_types));

    cr_std_testing_run_tests(tests);
    cr_std_vector_free(&tests);
}

int cr_std_vector_test_new_vector_normal() {
    vector_t *vector = cr_std_vector_new_n(sizeof(int));
    int result = vector != NULL;
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_vector_test_new_vector_normal_pointer() {
    vector_t *vector = cr_std_vector_new_n(sizeof(int *));
    int result = vector != NULL;
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_vector_test_new_vector_custom() {
    vector_t *vector = cr_std_vector_new_n(sizeof(string_t));
    int result = vector != NULL;
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_vector_test_new_vector_custom_pointer() {
    vector_t *vector = cr_std_vector_new(sizeof(string_t *), cr_std_string_free_ptr, NULL);
    int result = vector != NULL;
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_vector_test_free() {
    vector_t *vector = cr_std_vector_new_n(sizeof(int));
    int function_result = cr_std_vector_free(&vector);
    int result = function_result == 1 && vector == NULL;
    return result;
}

int cr_std_vector_test_free_pointer_vector() {
    vector_t *vector = cr_std_vector_new(sizeof(string_t *), cr_std_string_free_ptr, NULL);
    string_t *string = cr_std_string_new("Hello World");
    cr_std_vector_push_back(vector, string);
    int function_result = cr_std_vector_free(&vector);
    int result = function_result == 1 && vector == NULL;
    return result;
}

int cr_std_vector_test_free_null() {
    vector_t *vector = NULL;
    int function_result = cr_std_vector_free(&vector);
    int result = function_result == 0;
    return result;
}

int cr_std_vector_test_push_back_primitive() {
    vector_t *vector = cr_std_vector_new_n(sizeof(int));
    int expected_size = 0;
    int number_1 = 1;
    int number_2 = 2;
    int number_3 = 3;
    int number_4 = 4;
    int number_5 = 5;

    expected_size += cr_std_vector_push_back(vector, &number_1);
    expected_size += cr_std_vector_push_back(vector, &number_2);
    expected_size += cr_std_vector_push_back(vector, &number_3);
    expected_size += cr_std_vector_push_back(vector, &number_4);
    expected_size += cr_std_vector_push_back(vector, &number_5);
    int result = vector != NULL && expected_size == vector->size;
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_vector_test_push_back_primitive_pointer() {
    vector_t *vector = cr_std_vector_new_n(sizeof(char *));
    int expected_size = 0;

    expected_size += cr_std_vector_push_back(vector, "Hello");
    expected_size += cr_std_vector_push_back(vector, "Hello");
    expected_size += cr_std_vector_push_back(vector, "Hello");
    expected_size += cr_std_vector_push_back(vector, "Hello");
    expected_size += cr_std_vector_push_back(vector, "Hello");
    int result = vector != NULL && expected_size == vector->size;
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_vector_test_push_back_custom() {
    typedef struct test_struct_t {
        int x;
        int y;
    } test_struct_t;

    vector_t *vector = cr_std_vector_new_n(sizeof(test_struct_t));
    int expected_size = 0;

    test_struct_t test_1 = {4, 7};
    test_struct_t test_2 = {5, 7};
    test_struct_t test_3 = {7, 7};
    test_struct_t test_4 = {2, 7};
    test_struct_t test_5 = {6, 7};

    expected_size += cr_std_vector_push_back(vector, &test_1);
    expected_size += cr_std_vector_push_back(vector, &test_2);
    expected_size += cr_std_vector_push_back(vector, &test_3);
    expected_size += cr_std_vector_push_back(vector, &test_4);
    expected_size += cr_std_vector_push_back(vector, &test_5);
    int result = vector != NULL && expected_size == vector->size;
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_vector_test_push_back_custom_pointer() {
    vector_t *vector = cr_std_vector_new(sizeof(string_t *), cr_std_string_free_ptr, NULL);

    int expected_size = 0;
    string_t *string_1 = cr_std_string_new("Hello World");
    string_t *string_2 = cr_std_string_new("Hello World");
    string_t *string_3 = cr_std_string_new("Hello World");
    string_t *string_4 = cr_std_string_new("Hello World");

    expected_size += cr_std_vector_push_back(vector, string_1);
    expected_size += cr_std_vector_push_back(vector, string_2);
    expected_size += cr_std_vector_push_back(vector, string_3);
    expected_size += cr_std_vector_push_back(vector, string_4);
    int result = vector != NULL && expected_size == vector->size;
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_vector_test_remove_element_primitive() {
    vector_t *vector = cr_std_vector_new_n(sizeof(int));
    int expected_size = 0;
    int number_1 = 1;
    int number_2 = 2;
    int number_3 = 3;
    int number_4 = 4;
    int number_5 = 5;

    expected_size += cr_std_vector_push_back(vector, &number_1);
    expected_size += cr_std_vector_push_back(vector, &number_2);
    expected_size += cr_std_vector_push_back(vector, &number_3);
    expected_size += cr_std_vector_push_back(vector, &number_4);
    expected_size += cr_std_vector_push_back(vector, &number_5);

    expected_size -= cr_std_vector_remove_element(vector, 0);
    expected_size -= cr_std_vector_remove_element(vector, 1);
    int result = vector != NULL && expected_size == vector->size;
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_vector_test_remove_element_primitive_pointer() {
    vector_t *vector = cr_std_vector_new_n(sizeof(char *));
    int expected_size = 0;
    char *string_1 = "Hello";
    char *string_2 = "Hello";
    char *string_3 = "Hello";
    char *string_4 = "Hello";
    char *string_5 = "Hello";

    expected_size += cr_std_vector_push_back(vector, string_1);
    expected_size += cr_std_vector_push_back(vector, string_2);
    expected_size += cr_std_vector_push_back(vector, string_3);
    expected_size += cr_std_vector_push_back(vector, string_4);
    expected_size += cr_std_vector_push_back(vector, string_5);

    expected_size -= cr_std_vector_remove_element(vector, 0);
    expected_size -= cr_std_vector_remove_element(vector, 1);
    int result = vector != NULL && expected_size == vector->size;
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_vector_test_remove_element_custom() {
    typedef struct test_struct_t {
        int x;
        int y;
    } test_struct_t;

    vector_t *vector = cr_std_vector_new_n(sizeof(test_struct_t));
    int expected_size = 0;

    test_struct_t test_1 = {4, 7};
    test_struct_t test_2 = {5, 7};
    test_struct_t test_3 = {7, 7};
    test_struct_t test_4 = {2, 7};
    test_struct_t test_5 = {6, 7};

    expected_size += cr_std_vector_push_back(vector, &test_1);
    expected_size += cr_std_vector_push_back(vector, &test_2);
    expected_size += cr_std_vector_push_back(vector, &test_3);
    expected_size += cr_std_vector_push_back(vector, &test_4);
    expected_size += cr_std_vector_push_back(vector, &test_5);

    expected_size -= cr_std_vector_remove_element(vector, 0);
    expected_size -= cr_std_vector_remove_element(vector, 1);
    int result = vector != NULL && expected_size == vector->size;
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_vector_test_remove_element_custom_pointer() {
    vector_t *vector = cr_std_vector_new(sizeof(string_t *), cr_std_string_free_ptr, cr_std_string_make_copy_ptr);

    int expected_size = 0;
    string_t *string_1 = cr_std_string_new("Hello World");
    string_t *string_2 = cr_std_string_new("Hello World");
    string_t *string_3 = cr_std_string_new("Hello World");
    string_t *string_4 = cr_std_string_new("Hello World");

    expected_size += cr_std_vector_push_back(vector, string_1);
    expected_size += cr_std_vector_push_back(vector, string_2);
    expected_size += cr_std_vector_push_back(vector, string_3);
    expected_size += cr_std_vector_push_back(vector, string_4);

    expected_size -= cr_std_vector_remove_element(vector, 0);
    expected_size -= cr_std_vector_remove_element(vector, 1);
    int result = vector != NULL && expected_size == vector->size;

    // Free strings, they were copied on push.
    cr_std_string_free(&string_1);
    cr_std_string_free(&string_2);
    cr_std_string_free(&string_3);
    cr_std_string_free(&string_4);

    cr_std_vector_free(&vector);
    return result;
}

int cr_std_vector_test_remove_element_invalid_vector() {
    vector_t *vector = NULL;

    int function_result = cr_std_vector_remove_element(vector, 0);
    int result = function_result == 0;
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_vector_test_remove_element_invalid_index() {
    vector_t *vector = cr_std_vector_new(sizeof(string_t *), cr_std_string_free_ptr, NULL);

    int expected_size = 0;
    string_t *string_1 = cr_std_string_new("Hello World");
    string_t *string_2 = cr_std_string_new("Hello World");
    string_t *string_3 = cr_std_string_new("Hello World");
    string_t *string_4 = cr_std_string_new("Hello World");

    expected_size += cr_std_vector_push_back(vector, string_1);
    expected_size += cr_std_vector_push_back(vector, string_2);
    expected_size += cr_std_vector_push_back(vector, string_3);
    expected_size += cr_std_vector_push_back(vector, string_4);

    int function_result = cr_std_vector_remove_element(vector, 8);
    int result = vector != NULL && expected_size == vector->size && function_result == 0;
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_vector_test_remove_element_invalid_index_negative() {
    vector_t *vector = cr_std_vector_new(sizeof(string_t *), cr_std_string_free_ptr, NULL);

    int expected_size = 0;
    int function_result = cr_std_vector_remove_element(vector, -2);
    int result = vector != NULL && expected_size == vector->size && function_result == 0;
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_vector_test_remove_element_empty_vector() {
    vector_t *vector = cr_std_vector_new(sizeof(string_t *), cr_std_string_free_ptr, NULL);

    int expected_size = 0;
    int function_result = cr_std_vector_remove_element(vector, 0);
    int result = vector != NULL && expected_size == vector->size && function_result == 0;
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_vector_test_get_element_primitive() {
    vector_t *vector = cr_std_vector_new_n(sizeof(int));
    int expected_size = 0;
    int number_1 = 1;
    int number_2 = 2;
    int number_3 = 3;
    int number_4 = 4;
    int number_5 = 5;

    expected_size += cr_std_vector_push_back(vector, &number_1);
    expected_size += cr_std_vector_push_back(vector, &number_2);
    expected_size += cr_std_vector_push_back(vector, &number_3);
    expected_size += cr_std_vector_push_back(vector, &number_4);
    expected_size += cr_std_vector_push_back(vector, &number_5);

    // Change the number
    int *number = (int *)cr_std_vector_get_element(vector, 2);
    *number = 10;

    int actual_number = *(int *)cr_std_vector_get_element(vector, 2);

    int result = vector != NULL && expected_size == vector->size && actual_number == 10;
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_vector_test_get_element_primitive_pointer() {
    vector_t *vector = cr_std_vector_new_n(sizeof(char *));
    int expected_size = 0;
    char *string_1 = "Hello";
    char *string_2 = "Hello";
    char *string_3 = "Hello";
    char *string_4 = "Hello";
    char *string_5 = "Hello";

    expected_size += cr_std_vector_push_back(vector, string_1);
    expected_size += cr_std_vector_push_back(vector, string_2);
    expected_size += cr_std_vector_push_back(vector, string_3);
    expected_size += cr_std_vector_push_back(vector, string_4);
    expected_size += cr_std_vector_push_back(vector, string_5);

    char *string = (char *)cr_std_vector_get_element(vector, 2);
    int result = vector != NULL && expected_size == vector->size && string != NULL;
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_vector_test_get_element_custom() {
    typedef struct test_struct_t {
        int x;
        int y;
    } test_struct_t;

    vector_t *vector = cr_std_vector_new_n(sizeof(test_struct_t));
    int expected_size = 0;

    test_struct_t test_1 = {4, 7};
    test_struct_t test_2 = {5, 7};
    test_struct_t test_3 = {7, 7};
    test_struct_t test_4 = {2, 7};
    test_struct_t test_5 = {6, 7};

    expected_size += cr_std_vector_push_back(vector, &test_1);
    expected_size += cr_std_vector_push_back(vector, &test_2);
    expected_size += cr_std_vector_push_back(vector, &test_3);
    expected_size += cr_std_vector_push_back(vector, &test_4);
    expected_size += cr_std_vector_push_back(vector, &test_5);

    test_struct_t *test = (test_struct_t *)cr_std_vector_get_element(vector, 2);
    // Change the number
    test->x = 10;
    test_struct_t actual_test = *(test_struct_t *)cr_std_vector_get_element(vector, 2);

    int result = vector != NULL && expected_size == vector->size && actual_test.x == 10;
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_vector_test_get_element_custom_pointer() {
    vector_t *vector = cr_std_vector_new(sizeof(string_t *), cr_std_string_free_ptr, NULL);

    int expected_size = 0;
    string_t *string_1 = cr_std_string_new("String 1");
    string_t *string_2 = cr_std_string_new("String 2");
    string_t *string_3 = cr_std_string_new("String 3");
    string_t *string_4 = cr_std_string_new("String 4");

    expected_size += cr_std_vector_push_back(vector, string_1);
    expected_size += cr_std_vector_push_back(vector, string_2);
    expected_size += cr_std_vector_push_back(vector, string_3);
    expected_size += cr_std_vector_push_back(vector, string_4);

    string_t *expected_string = cr_std_string_new("String 3");
    string_t *string = (string_t *)cr_std_vector_get_element(vector, 2);

    int function_result = cr_std_string_compare(string, expected_string);
    int result = vector != NULL && expected_size == vector->size && function_result == 1;
    cr_std_vector_free(&vector);
    cr_std_string_free(&expected_string);
    return result;
}

int cr_std_vector_test_get_element_invalid_vector() {
    vector_t *vector = NULL;

    int result = cr_std_vector_get_element(vector, 0) == NULL;
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_vector_test_get_element_invalid_index() {
    vector_t *vector = cr_std_vector_new(sizeof(string_t *), cr_std_string_free_ptr, NULL);

    int expected_size = 0;
    string_t *string_1 = cr_std_string_new("Hello World");
    string_t *string_2 = cr_std_string_new("Hello World");
    string_t *string_3 = cr_std_string_new("Hello World");
    string_t *string_4 = cr_std_string_new("Hello World");

    expected_size += cr_std_vector_push_back(vector, string_1);
    expected_size += cr_std_vector_push_back(vector, string_2);
    expected_size += cr_std_vector_push_back(vector, string_3);
    expected_size += cr_std_vector_push_back(vector, string_4);

    void *function_result = cr_std_vector_get_element(vector, 8);
    int result = vector != NULL && expected_size == vector->size && function_result == NULL;
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_vector_test_get_element_invalid_index_negative() {
    vector_t *vector = cr_std_vector_new(sizeof(string_t *), cr_std_string_free_ptr, NULL);

    int expected_size = 0;
    void *function_result = cr_std_vector_get_element(vector, -2);
    int result = vector != NULL && expected_size == vector->size && function_result == NULL;
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_vector_test_get_element_empty_vector() {
    vector_t *vector = cr_std_vector_new(sizeof(string_t *), cr_std_string_free_ptr, NULL);

    int expected_size = 0;
    void *function_result = cr_std_vector_get_element(vector, -2);
    int result = vector != NULL && expected_size == vector->size && function_result == NULL;
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_vector_test_extend_primitive_type() {
    vector_t *dest = cr_std_vector_new_n(sizeof(int));
    vector_t *src = cr_std_vector_new_n(sizeof(int));
    int expected_size = 0;
    int number_1 = 1;
    int number_2 = 2;
    int number_3 = 3;
    int number_4 = 4;
    int number_5 = 5;

    expected_size += cr_std_vector_push_back(dest, &number_1);
    expected_size += cr_std_vector_push_back(dest, &number_2);
    expected_size += cr_std_vector_push_back(dest, &number_3);

    expected_size += cr_std_vector_push_back(src, &number_4);
    expected_size += cr_std_vector_push_back(src, &number_5);

    int function_result = cr_std_vector_extend(dest, src);
    cr_std_vector_free(&src);

    // Change the number
    int *number = (int *)cr_std_vector_get_element(dest, 3);
    *number = 10;
    int actual_number = *(int *)cr_std_vector_get_element(dest, 3);

    int result = dest != NULL && src == NULL && expected_size == dest->size && actual_number == 10 && function_result == 1;
    cr_std_vector_free(&dest);
    return result;
}

int cr_std_vector_test_extend_primitive_pointer() {
    vector_t *dest = cr_std_vector_new_n(sizeof(char *));
    vector_t *src = cr_std_vector_new_n(sizeof(char *));
    int expected_size = 0;
    char *string_1 = "Hello";
    char *string_2 = "Hello";
    char *string_3 = "Hello";
    char *string_4 = "Hello";
    char *string_5 = "Hello";

    expected_size += cr_std_vector_push_back(dest, string_1);
    expected_size += cr_std_vector_push_back(dest, string_2);
    expected_size += cr_std_vector_push_back(dest, string_3);

    expected_size += cr_std_vector_push_back(src, string_4);
    expected_size += cr_std_vector_push_back(src, string_5);

    int function_result = cr_std_vector_extend(dest, src);
    cr_std_vector_free(&src);

    char *string = (char *)cr_std_vector_get_element(dest, 3);
    int result = dest != NULL && src == NULL && expected_size == dest->size && string != NULL && function_result == 1;
    cr_std_vector_free(&dest);
    return result;
}

int cr_std_vector_test_extend_custom() {
    typedef struct test_struct_t {
        int x;
        int y;
    } test_struct_t;

    vector_t *dest = cr_std_vector_new_n(sizeof(test_struct_t));
    vector_t *src = cr_std_vector_new_n(sizeof(test_struct_t));
    int expected_size = 0;

    test_struct_t test_1 = {4, 7};
    test_struct_t test_2 = {5, 7};
    test_struct_t test_3 = {7, 7};
    test_struct_t test_4 = {2, 7};
    test_struct_t test_5 = {6, 7};

    expected_size += cr_std_vector_push_back(dest, &test_1);
    expected_size += cr_std_vector_push_back(dest, &test_2);
    expected_size += cr_std_vector_push_back(dest, &test_3);

    expected_size += cr_std_vector_push_back(src, &test_4);
    expected_size += cr_std_vector_push_back(src, &test_5);

    int function_result = cr_std_vector_extend(dest, src);
    cr_std_vector_free(&src);

    test_struct_t *test = (test_struct_t *)cr_std_vector_get_element(dest, 3);
    // Change the number
    test->x = 10;
    test_struct_t actual_test = *(test_struct_t *)cr_std_vector_get_element(dest, 3);

    int result = dest != NULL && src == NULL && expected_size == dest->size && actual_test.x == 10 && function_result == 1;
    cr_std_vector_free(&dest);
    return result;
}

int cr_std_vector_test_extend_custom_pointer() {
    vector_t *dest = cr_std_vector_new(sizeof(string_t *), cr_std_string_free_ptr, NULL);
    vector_t *src = cr_std_vector_new(sizeof(string_t *), cr_std_string_free_ptr, NULL);

    int expected_size = 0;
    string_t *string_1 = cr_std_string_new("String 1");
    string_t *string_2 = cr_std_string_new("String 2");
    string_t *string_3 = cr_std_string_new("String 3");
    string_t *string_4 = cr_std_string_new("String 4");

    expected_size += cr_std_vector_push_back(dest, string_1);
    expected_size += cr_std_vector_push_back(dest, string_2);

    expected_size += cr_std_vector_push_back(src, string_3);
    expected_size += cr_std_vector_push_back(src, string_4);

    int extend_function_result = cr_std_vector_extend(dest, src);

    string_t *expected_string = cr_std_string_new("String 4");
    string_t *string = (string_t *)cr_std_vector_get_element(dest, 3);

    int compare_function_result = cr_std_string_compare(string, expected_string);
    int result = dest != NULL && src != NULL && expected_size == dest->size && extend_function_result == 1 && compare_function_result == 1;

    free(src);
    cr_std_vector_free(&dest);
    cr_std_string_free(&expected_string);
    return result;
}

int cr_std_vector_test_extend_custom_pointer_copy_elements() {
    vector_t *dest = cr_std_vector_new(sizeof(string_t *), cr_std_string_free_ptr, cr_std_string_make_copy_ptr);
    vector_t *src = cr_std_vector_new(sizeof(string_t *), cr_std_string_free_ptr, cr_std_string_make_copy_ptr);

    int expected_size = 0;
    string_t *string_1 = cr_std_string_new("String 1");
    string_t *string_2 = cr_std_string_new("String 2");
    string_t *string_3 = cr_std_string_new("String 3");
    string_t *string_4 = cr_std_string_new("String 4");

    expected_size += cr_std_vector_push_back(dest, string_1);
    expected_size += cr_std_vector_push_back(dest, string_2);

    expected_size += cr_std_vector_push_back(src, string_3);
    expected_size += cr_std_vector_push_back(src, string_4);

    int extend_function_result = cr_std_vector_extend(dest, src);
    cr_std_vector_free(&src);
    cr_std_string_free(&string_1);
    cr_std_string_free(&string_2);
    cr_std_string_free(&string_3);
    cr_std_string_free(&string_4);

    string_t *expected_string = cr_std_string_new("String 4");
    string_t *string = (string_t *)cr_std_vector_get_element(dest, 3);

    int compare_function_result = cr_std_string_compare(string, expected_string);
    int result = dest != NULL && src == NULL && expected_size == dest->size && extend_function_result == 1 && compare_function_result == 1;

    cr_std_vector_free(&dest);
    cr_std_string_free(&expected_string);
    return result;
}


int cr_std_vector_test_extend_incorrect_types() {
    vector_t *dest = cr_std_vector_new_n(sizeof(int));
    vector_t *src = cr_std_vector_new(sizeof(string_t *), cr_std_string_free_ptr, cr_std_string_make_copy_ptr);
    int expected_size = 0;
    int number_1 = 1;
    int number_2 = 2;
    int number_3 = 3;
    string_t *string_4 = cr_std_string_new("String 4");
    string_t *string_5 = cr_std_string_new("String 5");

    expected_size += cr_std_vector_push_back(dest, &number_1);
    expected_size += cr_std_vector_push_back(dest, &number_2);
    expected_size += cr_std_vector_push_back(dest, &number_3);

    cr_std_vector_push_back(src, string_4);
    cr_std_vector_push_back(src, string_5);

    int function_result = cr_std_vector_extend(dest, src);

    int result = dest != NULL && src != NULL && expected_size == dest->size && function_result == 0;
    cr_std_vector_free(&src);
    cr_std_vector_free(&dest);
    return result;
}
