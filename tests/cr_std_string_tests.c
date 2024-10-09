#include "cr_std_string_tests.h"
#include "cr_std_string.h"
#include "cr_std_testing.h"
#include "cr_std_vector.h"
#include <stdio.h>
#include <stdlib.h>

void cr_std_string_test_all() {
    printf("%s\n", "Running String Tests:");

    vector_t *tests = cr_std_vector_new(sizeof(test_case_t *), NULL);

    // New String
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Make String -> Normal", cr_std_string_test_new_string_normal));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Make String -> Empty", cr_std_string_test_new_string_empty));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Make String -> Formatted", cr_std_string_test_new_string_formatted));

    // Free String
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Free String -> Normal", cr_std_string_test_free));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Free String -> NULL", cr_std_string_test_free_null_value));

    // Copy String
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Copy String -> Normal", cr_std_string_test_copy_string));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Copy String -> Empty", cr_std_string_test_copy_string_empty));

    // Concat String
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Concat String -> Normal", cr_std_string_test_concat_string));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Concat String -> Multi", cr_std_string_test_concat_string_multi));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Concat String -> Empty", cr_std_string_test_concat_string_empty));

    // Compare String
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Compare String -> Left String Longer", cr_std_string_test_compare_length_left));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Compare String -> Right String Longer", cr_std_string_test_compare_length_right));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Compare String -> Same Length Not Equal", cr_std_string_test_compare_partial_equal));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Compare String -> Same Length And Equal", cr_std_string_test_compare_fully_equal));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Compare String -> Empty", cr_std_string_test_compare_empty_string));

    // Trim String
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Trim String Left -> Normal", cr_std_string_test_trim_left));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Trim String Right -> Normal", cr_std_string_test_trim_right));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Trim String Both -> Normal", cr_std_string_test_trim_both));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Trim String Both -> Special Case", cr_std_string_test_trim_both_with_special));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Trim String Both -> Word Gaps", cr_std_string_test_trim_both_word_gaps));

    // Find Char
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Find Char In String -> Valid Char", cr_std_string_test_find_char_valid));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Find Char In String -> Invalid Char", cr_std_string_test_find_char_invalid));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Find Last Char In String -> Valid Char", cr_std_string_test_find_char_valid));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Find Last Char In String -> Invalid Char", cr_std_string_test_find_char_invalid));

    // Contains String
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Contains String -> Normal", cr_std_string_test_contains_string));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Contains String -> Multi", cr_std_string_test_contains_string_multi));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Contains String -> Invalid", cr_std_string_test_contains_string_invalid));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Contains String -> Empty", cr_std_string_test_contains_string_empty));

    // Contains Char
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Contains Char -> Normal", cr_std_string_test_contains_char));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Contains Char -> Multi", cr_std_string_test_contains_char_multi));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Contains Char -> Invalid", cr_std_string_test_contains_char_invalid));

    // Hash Code
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Hash Code -> Normal", cr_std_string_test_hash_code));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Hash Code -> Different", cr_std_string_test_hash_code_different));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Hash Code -> Invalid", cr_std_string_test_hash_code_invalid));

    // Split
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Split -> Normal", cr_std_string_test_split));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Split -> Invalid Char", cr_std_string_test_split_invalid));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Split -> NULL Value", cr_std_string_test_split_null_value));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Split -> Empty String", cr_std_string_test_split_empty));

    // To Upper
    cr_std_vector_push_back(tests, cr_std_testing_new_test("To Upper -> Normal", cr_std_string_test_to_upper));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("To Upper -> Empty String", cr_std_string_test_to_upper_empty));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("To Upper -> NULL Value", cr_std_string_test_to_upper_invalid));

    // To Lower
    cr_std_vector_push_back(tests, cr_std_testing_new_test("To Lower -> Normal", cr_std_string_test_to_lower));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("To Lower -> Empty String", cr_std_string_test_to_lower_empty));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("To Lower -> NULL Value", cr_std_string_test_to_lower_invalid));

    // Replace String
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Replace String -> Normal", cr_std_string_test_replace_string));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Replace String -> Multi", cr_std_string_test_replace_string_multi));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Replace String -> Invalid", cr_std_string_test_replace_string_invalid));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Replace String -> NULL Value", cr_std_string_test_replace_string_null_value));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Replace String -> Empty", cr_std_string_test_replace_string_empty));

    cr_std_testing_run_tests(tests);
    cr_std_vector_free(&tests);
}

int cr_std_string_test_new_string_normal() {
    string_t *string = cr_std_string_new("Hello World");
    int result = string != NULL;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_new_string_empty() {
    string_t *string = cr_std_string_new("");
    int result = string != NULL;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_new_string_formatted() {
    string_t *string = cr_std_string_new("Hello %s", "World");
    int result = string != NULL;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_free() {
    string_t *string = cr_std_string_new("Hello %s", "World");
    int function_result = cr_std_string_free(&string);
    int result = function_result == 1 && string == NULL;
    return result;
}

int cr_std_string_test_free_null_value() {
    string_t *string = NULL;
    int function_result = cr_std_string_free(&string);
    return function_result == 0;
}

int cr_std_string_test_copy_string() {
    string_t *string = cr_std_string_new("Hello World");
    string_t *string_copy = cr_std_string_make_copy(string);
    int compare_result = cr_std_string_compare(string, string_copy);
    int expected = 1;
    int result = string != NULL && string_copy != NULL && compare_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string_copy);
    return result;
}

int cr_std_string_test_copy_string_empty() {
    string_t *string = cr_std_string_new("");
    string_t *string_copy = cr_std_string_make_copy(string);
    int compare_result = cr_std_string_compare(string, string_copy);
    int expected = 1;
    int result = string != NULL && string_copy != NULL && compare_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string_copy);
    return result;
}

int cr_std_string_test_concat_string() {
    string_t *string = cr_std_string_new("Hello World");
    string_t *string_expected = cr_std_string_new("Hello World, My Name Is John");
    int function_result = cr_std_string_concat(string, ", My Name Is John");
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_concat_string_multi() {
    string_t *string = cr_std_string_new("Hello World");
    string_t *string_expected = cr_std_string_new("Hello World, My Name Is John");
    int function_result = cr_std_string_concat(string, ", My Name Is", " John");
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_concat_string_empty() {
    string_t *string = cr_std_string_new("");
    string_t *string_expected = cr_std_string_new("Hello World");
    int function_result = cr_std_string_concat(string, "Hello World");
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_compare_length_left() {
    string_t *string = cr_std_string_new("Hello World");
    string_t *string2 = cr_std_string_new("Test");
    int compare_result = cr_std_string_compare(string, string2);
    int expected = -1;
    int result = string != NULL && string2 != NULL && compare_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string2);
    return result;
}

int cr_std_string_test_compare_length_right() {
    string_t *string = cr_std_string_new("Test");
    string_t *string2 = cr_std_string_new("Hello World");
    int compare_result = cr_std_string_compare(string, string2);
    int expected = -2;
    int result = string != NULL && string2 != NULL && compare_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string2);
    return result;
}

int cr_std_string_test_compare_partial_equal() {
    string_t *string = cr_std_string_new("hello world");
    string_t *string2 = cr_std_string_new("Hello World");
    int compare_result = cr_std_string_compare(string, string2);
    int expected = 0;
    int result = string != NULL && string2 != NULL && compare_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string2);
    return result;
}

int cr_std_string_test_compare_fully_equal() {
    string_t *string = cr_std_string_new("Hello World");
    string_t *string2 = cr_std_string_new("Hello World");
    int compare_result = cr_std_string_compare(string, string2);
    int expected = 1;
    int result = string != NULL && string2 != NULL && compare_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string2);
    return result;
}

int cr_std_string_test_compare_empty_string() {
    string_t *string = cr_std_string_new("");
    string_t *string2 = cr_std_string_new("");
    int compare_result = cr_std_string_compare(string, string2);
    int expected = 1;
    int result = string != NULL && string2 != NULL && compare_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string2);
    return result;
}

int cr_std_string_test_trim_left() {
    string_t *string = cr_std_string_new("     Hello World   ");
    string_t *string_expected = cr_std_string_new("Hello World   ");
    int function_result = cr_std_string_trim(string, CR_STD_STRING_TRIM_LEFT);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_trim_right() {
    string_t *string = cr_std_string_new("   Hello World     ");
    string_t *string_expected = cr_std_string_new("   Hello World");
    int function_result = cr_std_string_trim(string, CR_STD_STRING_TRIM_RIGHT);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_trim_both() {
    string_t *string = cr_std_string_new("   Hello World     ");
    string_t *string_expected = cr_std_string_new("Hello World");
    int function_result = cr_std_string_trim(string, CR_STD_STRING_TRIM_BOTH);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_trim_both_with_special() {
    string_t *string = cr_std_string_new(" \n\r  Hello World   \n\n\r  ");
    string_t *string_expected = cr_std_string_new("Hello World");
    int function_result = cr_std_string_trim(string, CR_STD_STRING_TRIM_BOTH);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_trim_both_word_gaps() {
    string_t *string = cr_std_string_new(" \n\r  Hello \n\n World   \n\n\r  ");
    string_t *string_expected = cr_std_string_new("Hello \n\n World");
    int function_result = cr_std_string_trim(string, CR_STD_STRING_TRIM_BOTH);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_find_char_valid() {
    string_t *string = cr_std_string_new("Hello World");
    int function_result = cr_std_string_find_char(string, 'W');
    int expected = 6;
    int result = string != NULL && function_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_find_char_invalid() {
    string_t *string = cr_std_string_new("Hello World");
    int function_result = cr_std_string_find_char(string, 'x');
    int expected = -1;
    int result = string != NULL && function_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_find_last_char_valid() {
    string_t *string = cr_std_string_new("Hello Hello");
    int function_result = cr_std_string_find_char(string, 'H');
    int expected = 6;
    int result = string != NULL && function_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_find_last_char_invalid() {
    string_t *string = cr_std_string_new("Hello World");
    int function_result = cr_std_string_find_char(string, 'x');
    int expected = -1;
    int result = string != NULL && function_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_contains_string() {
    string_t *string = cr_std_string_new("Hello World");
    char *phrase = "Hello";
    int function_result = cr_std_string_contains_string(string, phrase);
    int expected = 1;
    int result = string != NULL && function_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_contains_string_multi() {
    string_t *string = cr_std_string_new("Hello World Hello");
    char *phrase = "Hello";
    int function_result = cr_std_string_contains_string(string, phrase);
    int expected = 2;
    int result = string != NULL && function_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_contains_string_invalid() {
    string_t *string = cr_std_string_new("Hello World");
    char *phrase = "John";
    int function_result = cr_std_string_contains_string(string, phrase);
    int expected = 0;
    int result = string != NULL && function_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_contains_string_empty() {
    string_t *string = cr_std_string_new("Hello World");
    char *phrase = "";
    int function_result = cr_std_string_contains_string(string, phrase);
    int expected = 0;
    int result = string != NULL && function_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_contains_char() {
    string_t *string = cr_std_string_new("Hello World");
    char ch = 'H';
    int function_result = cr_std_string_contains_char(string, ch);
    int expected = 1;
    int result = string != NULL && function_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_contains_char_multi() {
    string_t *string = cr_std_string_new("Hello World");
    char ch = 'o';
    int function_result = cr_std_string_contains_char(string, ch);
    int expected = 2;
    int result = string != NULL && function_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_contains_char_invalid() {
    string_t *string = cr_std_string_new("Hello World");
    char ch = 'x';
    int function_result = cr_std_string_contains_char(string, ch);
    int expected = 0;
    int result = string != NULL && function_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_hash_code() {
    string_t *string = cr_std_string_new("Hello World");
    string_t *test_case = cr_std_string_new("Hello World");
    int function_one_result = cr_std_string_hash_code(string);
    int function_two_result = cr_std_string_hash_code(test_case);
    int result = string != NULL && test_case != NULL && function_one_result == function_two_result;
    cr_std_string_free(&string);
    cr_std_string_free(&test_case);
    return result;
}

int cr_std_string_test_hash_code_different() {
    string_t *string = cr_std_string_new("Hello World");
    string_t *test_case = cr_std_string_new(" Hello World");
    int function_one_result = cr_std_string_hash_code(string);
    int function_two_result = cr_std_string_hash_code(test_case);
    int result = string != NULL && test_case != NULL && function_one_result != function_two_result;
    cr_std_string_free(&string);
    cr_std_string_free(&test_case);
    return result;
}

int cr_std_string_test_hash_code_invalid() {
    string_t *string = NULL;
    int function_result = cr_std_string_hash_code(string);
    int expected = -1;
    int result = function_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_split() {
    string_t *string = cr_std_string_new("Hello World");
    vector_t *vector = cr_std_string_split(string, ' ');
    int actual_size = vector->size;
    int expected_size = 2;
    int result = string != NULL && vector != NULL && actual_size == expected_size;
    cr_std_string_free(&string);
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_string_test_split_invalid() {
    string_t *string = cr_std_string_new("Hello World");
    vector_t *vector = cr_std_string_split(string, 'x');
    int actual_size = vector->size;
    int expected_size = 1;
    int result = string != NULL && vector != NULL && actual_size == expected_size;
    cr_std_string_free(&string);
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_string_test_split_null_value() {
    string_t *string = NULL;
    vector_t *vector = cr_std_string_split(string, 'x');
    int result = vector == NULL;
    cr_std_string_free(&string);
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_string_test_split_empty() {
    string_t *string = cr_std_string_new("");
    vector_t *vector = cr_std_string_split(string, 'x');
    int actual_size = vector->size;
    int expected_size = 0;
    int result = string != NULL && vector != NULL && actual_size == expected_size;
    cr_std_string_free(&string);
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_string_test_to_upper() {
    string_t *string = cr_std_string_new("Hello World");
    string_t *string_expected = cr_std_string_new("HELLO WORLD");
    int function_result = cr_std_string_to_upper(string);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_to_upper_empty() {
    string_t *string = cr_std_string_new("");
    string_t *string_expected = cr_std_string_new("");
    int function_result = cr_std_string_to_upper(string);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_to_upper_invalid() {
    string_t *string = NULL;
    int function_result = cr_std_string_to_upper(string);
    int expected = 0;
    return function_result == expected;
}

int cr_std_string_test_to_lower() {
    string_t *string = cr_std_string_new("HELLO WORLD");
    string_t *string_expected = cr_std_string_new("hello world");
    int function_result = cr_std_string_to_lower(string);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_to_lower_empty() {
    string_t *string = cr_std_string_new("");
    string_t *string_expected = cr_std_string_new("");
    int function_result = cr_std_string_to_lower(string);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_to_lower_invalid() {
    string_t *string = NULL;
    int function_result = cr_std_string_to_lower(string);
    int expected = 0;
    return function_result == expected;
}

int cr_std_string_test_replace_string() {
    string_t *string = cr_std_string_new("Hello World");
    string_t *string_expected = cr_std_string_new("Test World");
    int function_result = cr_std_string_replace_string(string, "Hello", "Test");
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_replace_string_multi() {
    string_t *string = cr_std_string_new("Hello, Hello World");
    string_t *string_expected = cr_std_string_new("Test, Test World");
    int function_result = cr_std_string_replace_string(string, "Hello", "Test");
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected_function_result = 2;
    int expected_compare_result = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected_compare_result && function_result == expected_function_result;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_replace_string_invalid() {
    string_t *string = cr_std_string_new("Hello World");
    string_t *string_expected = cr_std_string_new("Hello World");
    int function_result = cr_std_string_replace_string(string, "x", "Test");
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected_function_result = 0;
    int expected_compare_result = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected_compare_result && function_result == expected_function_result;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_replace_string_null_value() {
    string_t *string = NULL;
    int function_result = cr_std_string_replace_string(string, "Hello", "Test");
    int expected = 0;
    cr_std_string_free(&string);
    return function_result == expected;
}

int cr_std_string_test_replace_string_empty() {
    string_t *string = cr_std_string_new("");
    string_t *string_expected = cr_std_string_new("");
    int function_result = cr_std_string_replace_string(string, "x", "Test");
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected_function_result = 0;
    int expected_compare_result = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected_compare_result && function_result == expected_function_result;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}
