#include "cr_std_string_tests.h"
#include "cr_std_string.h"
#include "cr_std_testing.h"
#include "cr_std_utils.h"
#include "cr_std_vector.h"
#include <stdio.h>
#include <stdlib.h>

void cr_std_string_test_all() {
    printf("%s\n", "Running String Tests:");

    Vector *tests = cr_std_vector_new(TestCase *);
    tests->free_function = cr_std_free_ptr;

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
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Compare C_String -> Same Length And Equal", cr_std_string_test_compare_c_str_fully_equal));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Compare C_String -> Empty", cr_std_string_test_compare_c_str_empty_string));

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

    // Remove Non Numeric
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Remove Non Numeric -> Normal", cr_std_string_test_remove_non_numeric));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Remove Non Numeric -> Empty", cr_std_string_test_remove_non_numeric_empty));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Remove Non Numeric -> NULL Value", cr_std_string_test_remove_non_numeric_null));

    // Remove Numeric
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Remove Numeric -> Normal", cr_std_string_test_remove_numeric));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Remove Numeric -> Empty", cr_std_string_test_remove_numeric_empty));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Remove Numeric -> NULL Value", cr_std_string_test_remove_numeric_null));

    // To Int
    cr_std_vector_push_back(tests, cr_std_testing_new_test("To Int -> Normal", cr_std_string_test_to_int));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("To Int -> Mixed String", cr_std_string_test_to_int_mixed_string));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("To Int -> Empty", cr_std_string_test_to_int_empty));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("To Int -> NULL Value", cr_std_string_test_to_int_null));

    // From Int
    cr_std_vector_push_back(tests, cr_std_testing_new_test("From Int -> Normal", cr_std_string_test_from_int));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("From Int -> Negative", cr_std_string_test_from_int_negative));

    // From String Pointer Vector
    cr_std_vector_push_back(tests, cr_std_testing_new_test("From String Ptr Vector -> Normal", cr_std_string_test_from_string_ptr_vector));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("From String Ptr Vector -> Empty", cr_std_string_test_from_string_ptr_vector_empty));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("From String Ptr Vector -> NULL", cr_std_string_test_from_string_ptr_vector_null));

    // From Char Pointer Vector
    cr_std_vector_push_back(tests, cr_std_testing_new_test("From Char Ptr Vector -> Normal", cr_std_string_test_from_char_ptr_vector));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("From Char Ptr Vector -> Empty", cr_std_string_test_from_char_ptr_vector_empty));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("From Char Ptr Vector -> NULL", cr_std_string_test_from_char_ptr_vector_null));

    // Sub String
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Sub String -> Normal", cr_std_string_test_sub_string));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Sub String -> Index Too Low", cr_std_string_test_sub_string_index_too_low));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Sub String -> Index Too High", cr_std_string_test_sub_string_index_too_high));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Sub String -> Index Invalid", cr_std_string_test_sub_string_index_invalid));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Sub String -> Empty", cr_std_string_test_sub_string_empty));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Sub String -> Null", cr_std_string_test_sub_string_null));

    /**
     * String Builder Tests Below
     */

    // New String Builder
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Make String Builder -> Normal", cr_std_string_builder_test_new_normal));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Make String Builder -> Empty", cr_std_string_builder_test_new_empty));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Make String Builder -> Formatted", cr_std_string_builder_test_new_formatted));

    // Free String Builder
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Free String Builder -> Normal", cr_std_string_builder_test_free));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("Free String Builder -> Null Value", cr_std_string_builder_test_free_null_value));

    // String Builder Append
    cr_std_vector_push_back(tests, cr_std_testing_new_test("String Builder Append -> Normal", cr_std_string_builder_test_append));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("String Builder Append -> Formatted", cr_std_string_builder_test_appendf));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("String Builder Append -> Single", cr_std_string_builder_test_append_single));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("String Builder Append -> Empty", cr_std_string_builder_test_append_empty));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("String Builder Append -> NULL Value", cr_std_string_builder_test_append_null_value));

    // String Builder Reset
    cr_std_vector_push_back(tests, cr_std_testing_new_test("String Builder Reset -> Normal", cr_std_string_builder_test_reset));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("String Builder Reset -> NULL Value", cr_std_string_builder_test_reset_null_value));

    // String Builder To String
    cr_std_vector_push_back(tests, cr_std_testing_new_test("String Builder To String -> Normal", cr_std_string_builder_test_to_string));
    cr_std_vector_push_back(tests, cr_std_testing_new_test("String Builder To String -> NULL Value", cr_std_string_builder_test_to_string_null_value));

    cr_std_testing_run_tests(tests);
    cr_std_vector_free(&tests);
}

int cr_std_string_test_new_string_normal() {
    String *string = cr_std_string_new("Hello World");
    int result = string != NULL;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_new_string_empty() {
    String *string = cr_std_string_new("");
    int result = string != NULL;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_new_string_formatted() {
    String *string = cr_std_string_newf("Hello %s", "World");
    int result = string != NULL;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_free() {
    String *string = cr_std_string_newf("Hello %s", "World");
    int function_result = cr_std_string_free(&string);
    int result = function_result == 0 && string == NULL;
    return result;
}

int cr_std_string_test_free_null_value() {
    String *string = NULL;
    int function_result = cr_std_string_free(&string);
    return function_result != 0;
}

int cr_std_string_test_copy_string() {
    String *string = cr_std_string_new("Hello World");
    String *string_copy = cr_std_string_make_copy(string);
    int compare_result = cr_std_string_compare(string, string_copy);
    int expected = 1;
    int result = string != NULL && string_copy != NULL && compare_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string_copy);
    return result;
}

int cr_std_string_test_copy_string_empty() {
    String *string = cr_std_string_new("");
    String *string_copy = cr_std_string_make_copy(string);
    int compare_result = cr_std_string_compare(string, string_copy);
    int expected = 1;
    int result = string != NULL && string_copy != NULL && compare_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string_copy);
    return result;
}

int cr_std_string_test_concat_string() {
    String *string = cr_std_string_new("Hello World");
    String *string_expected = cr_std_string_new("Hello World, My Name Is John");
    int function_result = cr_std_string_concat(string, ", My Name Is John");
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_concat_string_multi() {
    String *string = cr_std_string_new("Hello World");
    String *string_expected = cr_std_string_new("Hello World, My Name Is John");
    int function_result = cr_std_string_concat(string, ", My Name Is", " John");
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_concat_string_empty() {
    String *string = cr_std_string_new("");
    String *string_expected = cr_std_string_new("Hello World");
    int function_result = cr_std_string_concat(string, "Hello World");
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_compare_length_left() {
    String *string = cr_std_string_new("Hello World");
    String *string2 = cr_std_string_new("Test");
    int compare_result = cr_std_string_compare(string, string2);
    int expected = -1;
    int result = string != NULL && string2 != NULL && compare_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string2);
    return result;
}

int cr_std_string_test_compare_length_right() {
    String *string = cr_std_string_new("Test");
    String *string2 = cr_std_string_new("Hello World");
    int compare_result = cr_std_string_compare(string, string2);
    int expected = -2;
    int result = string != NULL && string2 != NULL && compare_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string2);
    return result;
}

int cr_std_string_test_compare_partial_equal() {
    String *string = cr_std_string_new("hello world");
    String *string2 = cr_std_string_new("Hello World");
    int compare_result = cr_std_string_compare(string, string2);
    int expected = 0;
    int result = string != NULL && string2 != NULL && compare_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string2);
    return result;
}

int cr_std_string_test_compare_fully_equal() {
    String *string = cr_std_string_new("Hello World");
    String *string2 = cr_std_string_new("Hello World");
    int compare_result = cr_std_string_compare(string, string2);
    int expected = 1;
    int result = string != NULL && string2 != NULL && compare_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string2);
    return result;
}

int cr_std_string_test_compare_empty_string() {
    String *string = cr_std_string_new("");
    String *string2 = cr_std_string_new("");
    int compare_result = cr_std_string_compare(string, string2);
    int expected = 1;
    int result = string != NULL && string2 != NULL && compare_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string2);
    return result;
}

int cr_std_string_test_compare_c_str_fully_equal() {
    String *string = cr_std_string_new("Hello World");
    int compare_result = cr_std_string_compare_c_str(string, "Hello World");
    int expected = 1;
    int result = string != NULL && compare_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_compare_c_str_empty_string() {
    String *string = cr_std_string_new("");
    int compare_result = cr_std_string_compare_c_str(string, "");
    int expected = 1;
    int result = string != NULL && compare_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_trim_left() {
    String *string = cr_std_string_new("     Hello World   ");
    String *string_expected = cr_std_string_new("Hello World   ");
    int function_result = cr_std_string_trim(string, CR_STD_STRING_TRIM_LEFT);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_trim_right() {
    String *string = cr_std_string_new("   Hello World     ");
    String *string_expected = cr_std_string_new("   Hello World");
    int function_result = cr_std_string_trim(string, CR_STD_STRING_TRIM_RIGHT);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_trim_both() {
    String *string = cr_std_string_new("   Hello World     ");
    String *string_expected = cr_std_string_new("Hello World");
    int function_result = cr_std_string_trim(string, CR_STD_STRING_TRIM_BOTH);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_trim_both_with_special() {
    String *string = cr_std_string_new(" \n\r  Hello World   \n\n\r  ");
    String *string_expected = cr_std_string_new("Hello World");
    int function_result = cr_std_string_trim(string, CR_STD_STRING_TRIM_BOTH);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_trim_both_word_gaps() {
    String *string = cr_std_string_new(" \n\r  Hello \n\n World   \n\n\r  ");
    String *string_expected = cr_std_string_new("Hello \n\n World");
    int function_result = cr_std_string_trim(string, CR_STD_STRING_TRIM_BOTH);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_find_char_valid() {
    String *string = cr_std_string_new("Hello World");
    int function_result = cr_std_string_find_char(string, 'W');
    int expected = 6;
    int result = string != NULL && function_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_find_char_invalid() {
    String *string = cr_std_string_new("Hello World");
    int function_result = cr_std_string_find_char(string, 'x');
    int expected = -1;
    int result = string != NULL && function_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_find_last_char_valid() {
    String *string = cr_std_string_new("Hello Hello");
    int function_result = cr_std_string_find_char(string, 'H');
    int expected = 6;
    int result = string != NULL && function_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_find_last_char_invalid() {
    String *string = cr_std_string_new("Hello World");
    int function_result = cr_std_string_find_char(string, 'x');
    int expected = -1;
    int result = string != NULL && function_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_contains_string() {
    String *string = cr_std_string_new("Hello World");
    char *phrase = "Hello";
    int function_result = cr_std_string_contains_string(string, phrase);
    int expected = 1;
    int result = string != NULL && function_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_contains_string_multi() {
    String *string = cr_std_string_new("Hello World Hello");
    char *phrase = "Hello";
    int function_result = cr_std_string_contains_string(string, phrase);
    int expected = 2;
    int result = string != NULL && function_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_contains_string_invalid() {
    String *string = cr_std_string_new("Hello World");
    char *phrase = "John";
    int function_result = cr_std_string_contains_string(string, phrase);
    int expected = 0;
    int result = string != NULL && function_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_contains_string_empty() {
    String *string = cr_std_string_new("Hello World");
    char *phrase = "";
    int function_result = cr_std_string_contains_string(string, phrase);
    int expected = 0;
    int result = string != NULL && function_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_contains_char() {
    String *string = cr_std_string_new("Hello World");
    char ch = 'H';
    int function_result = cr_std_string_contains_char(string, ch);
    int expected = 1;
    int result = string != NULL && function_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_contains_char_multi() {
    String *string = cr_std_string_new("Hello World");
    char ch = 'o';
    int function_result = cr_std_string_contains_char(string, ch);
    int expected = 2;
    int result = string != NULL && function_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_contains_char_invalid() {
    String *string = cr_std_string_new("Hello World");
    char ch = 'x';
    int function_result = cr_std_string_contains_char(string, ch);
    int expected = 0;
    int result = string != NULL && function_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_hash_code() {
    String *string = cr_std_string_new("Hello World");
    String *test_case = cr_std_string_new("Hello World");
    int function_one_result = cr_std_string_hash_code(string);
    int function_two_result = cr_std_string_hash_code(test_case);
    int result = string != NULL && test_case != NULL && function_one_result == function_two_result;
    cr_std_string_free(&string);
    cr_std_string_free(&test_case);
    return result;
}

int cr_std_string_test_hash_code_different() {
    String *string = cr_std_string_new("Hello World");
    String *test_case = cr_std_string_new(" Hello World");
    int function_one_result = cr_std_string_hash_code(string);
    int function_two_result = cr_std_string_hash_code(test_case);
    int result = string != NULL && test_case != NULL && function_one_result != function_two_result;
    cr_std_string_free(&string);
    cr_std_string_free(&test_case);
    return result;
}

int cr_std_string_test_hash_code_invalid() {
    String *string = NULL;
    int function_result = cr_std_string_hash_code(string);
    int expected = -1;
    int result = function_result == expected;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_split() {
    String *string = cr_std_string_new("Hello World");
    Vector *vector = cr_std_string_split(string, ' ');
    int actual_size = vector->size;
    int expected_size = 2;
    int result = string != NULL && vector != NULL && actual_size == expected_size;
    cr_std_string_free(&string);
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_string_test_split_invalid() {
    String *string = cr_std_string_new("Hello World");
    Vector *vector = cr_std_string_split(string, 'x');
    int actual_size = vector->size;
    int expected_size = 1;
    int result = string != NULL && vector != NULL && actual_size == expected_size;
    cr_std_string_free(&string);
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_string_test_split_null_value() {
    String *string = NULL;
    Vector *vector = cr_std_string_split(string, 'x');
    int result = vector == NULL;
    cr_std_string_free(&string);
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_string_test_split_empty() {
    String *string = cr_std_string_new("");
    Vector *vector = cr_std_string_split(string, 'x');
    int actual_size = vector->size;
    int expected_size = 0;
    int result = string != NULL && vector != NULL && actual_size == expected_size;
    cr_std_string_free(&string);
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_string_test_to_upper() {
    String *string = cr_std_string_new("Hello World");
    String *string_expected = cr_std_string_new("HELLO WORLD");
    int function_result = cr_std_string_to_upper(string);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_to_upper_empty() {
    String *string = cr_std_string_new("");
    String *string_expected = cr_std_string_new("");
    int function_result = cr_std_string_to_upper(string);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_to_upper_invalid() {
    String *string = NULL;
    int function_result = cr_std_string_to_upper(string);
    return function_result != 0;
}

int cr_std_string_test_to_lower() {
    String *string = cr_std_string_new("HELLO WORLD");
    String *string_expected = cr_std_string_new("hello world");
    int function_result = cr_std_string_to_lower(string);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_to_lower_empty() {
    String *string = cr_std_string_new("");
    String *string_expected = cr_std_string_new("");
    int function_result = cr_std_string_to_lower(string);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_to_lower_invalid() {
    String *string = NULL;
    int function_result = cr_std_string_to_lower(string);
    return function_result != 0;
}

int cr_std_string_test_replace_string() {
    String *string = cr_std_string_new("Hello World");
    String *string_expected = cr_std_string_new("Test World");
    int function_result = cr_std_string_replace_string(string, "Hello", "Test");
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected && function_result == expected;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_replace_string_multi() {
    String *string = cr_std_string_new("Hello, Hello World");
    String *string_expected = cr_std_string_new("Test, Test World");
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
    String *string = cr_std_string_new("Hello World");
    String *string_expected = cr_std_string_new("Hello World");
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
    String *string = NULL;
    int function_result = cr_std_string_replace_string(string, "Hello", "Test");
    int expected = 0;
    cr_std_string_free(&string);
    return function_result == expected;
}

int cr_std_string_test_replace_string_empty() {
    String *string = cr_std_string_new("");
    String *string_expected = cr_std_string_new("");
    int function_result = cr_std_string_replace_string(string, "x", "Test");
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected_function_result = 0;
    int expected_compare_result = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected_compare_result && function_result == expected_function_result;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_remove_non_numeric() {
    String *string = cr_std_string_new("Hello667788, Hello World");
    String *string_expected = cr_std_string_new("667788");
    int function_result = cr_std_string_remove_non_numeric(string);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected_function_result = 0;
    int expected_compare_result = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected_compare_result && function_result == expected_function_result;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_remove_non_numeric_empty() {
    String *string = cr_std_string_new("");
    String *string_expected = cr_std_string_new("");
    int function_result = cr_std_string_remove_non_numeric(string);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected_function_result = 0;
    int expected_compare_result = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected_compare_result && function_result == expected_function_result;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_remove_non_numeric_null() {
    String *string = NULL;
    int function_result = cr_std_string_remove_non_numeric(string);
    return function_result != 0;
}

int cr_std_string_test_remove_numeric() {
    String *string = cr_std_string_new("Hello667788, Hello World");
    String *string_expected = cr_std_string_new("Hello, Hello World");
    int function_result = cr_std_string_remove_numeric(string);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected_function_result = 0;
    int expected_compare_result = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected_compare_result && function_result == expected_function_result;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_remove_numeric_empty() {
    String *string = cr_std_string_new("");
    String *string_expected = cr_std_string_new("");
    int function_result = cr_std_string_remove_numeric(string);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected_function_result = 0;
    int expected_compare_result = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected_compare_result && function_result == expected_function_result;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_remove_numeric_null() {
    String *string = NULL;
    int function_result = cr_std_string_remove_numeric(string);
    return function_result != 0;
}

int cr_std_string_test_to_int() {
    String *string = cr_std_string_new("555666");
    int function_result = cr_std_string_to_int(string);
    int expected_function_result = 555666;
    int result = string != NULL && function_result == expected_function_result;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_to_int_mixed_string() {
    String *string = cr_std_string_new("Hello555, Hello 666World");
    int function_result = cr_std_string_to_int(string);
    int expected_function_result = 555666;
    int result = string != NULL && function_result == expected_function_result;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_to_int_empty() {
    String *string = cr_std_string_new("");
    long int function_result = cr_std_string_to_int(string);
    long int expected_function_result = 0;
    int result = string != NULL && function_result == expected_function_result;
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_test_to_int_null() {
    String *string = NULL;
    long int function_result = cr_std_string_to_int(string);
    long int expected_function_result = 0;
    int result = function_result == expected_function_result;
    return result;
}

int cr_std_string_test_from_int() {
    String *string = cr_std_string_from_int(12345);
    String *string_expected = cr_std_string_new("12345");
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected_compare_result = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected_compare_result;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_from_int_negative() {
    String *string = cr_std_string_from_int(-12345);
    String *string_expected = cr_std_string_new("-12345");
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected_compare_result = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected_compare_result;
    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    return result;
}

int cr_std_string_test_from_string_ptr_vector() {
    String *string = cr_std_string_new("Hello World This Is A Test String");
    String *string_expected = cr_std_string_new("Hello, World, This, Is, A, Test, String");
    Vector *vector = cr_std_string_split(string, ' ');
    String *result_string = cr_std_string_from_string_ptr_vector(vector, ", ");

    int compare_result = cr_std_string_compare(result_string, string_expected);
    int expected_compare_result = 1;
    int result = string != NULL && string_expected != NULL && vector != NULL && result_string != NULL && compare_result == expected_compare_result;

    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    cr_std_string_free(&result_string);
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_string_test_from_string_ptr_vector_empty() {
    String *string_expected = cr_std_string_new("");
    Vector *vector = cr_std_vector_new(String *);
    vector->free_function = cr_std_free_ptr;
    vector->copy_function = cr_std_string_make_copy_ptr;

    String *result_string = cr_std_string_from_string_ptr_vector(vector, ", ");

    int compare_result = cr_std_string_compare(result_string, string_expected);
    int expected_compare_result = 1;
    int result = string_expected != NULL && vector != NULL && result_string != NULL && compare_result == expected_compare_result;

    cr_std_string_free(&string_expected);
    cr_std_string_free(&result_string);
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_string_test_from_string_ptr_vector_null() {
    Vector *vector = NULL;
    String *result_string = cr_std_string_from_string_ptr_vector(vector, ", ");

    int result = result_string == NULL;
    return result;
}

int cr_std_string_test_from_char_ptr_vector() {
    Vector *vector = cr_std_vector_new(char *);
    cr_std_vector_push_back(vector, "Hello");
    cr_std_vector_push_back(vector, "World");
    cr_std_vector_push_back(vector, "This");
    cr_std_vector_push_back(vector, "Is");
    cr_std_vector_push_back(vector, "A");
    cr_std_vector_push_back(vector, "Test");
    cr_std_vector_push_back(vector, "String");

    String *string_expected = cr_std_string_new("Hello, World, This, Is, A, Test, String");
    String *result_string = cr_std_string_from_char_ptr_vector(vector, ", ");

    int compare_result = cr_std_string_compare(result_string, string_expected);
    int expected_compare_result = 1;
    int result = string_expected != NULL && vector != NULL && result_string != NULL && compare_result == expected_compare_result;

    cr_std_string_free(&string_expected);
    cr_std_string_free(&result_string);
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_string_test_from_char_ptr_vector_empty() {
    String *string_expected = cr_std_string_new("");
    Vector *vector = cr_std_vector_new(char *);
    String *result_string = cr_std_string_from_char_ptr_vector(vector, ", ");

    int compare_result = cr_std_string_compare(result_string, string_expected);
    int expected_compare_result = 1;
    int result = string_expected != NULL && vector != NULL && result_string != NULL && compare_result == expected_compare_result;

    cr_std_string_free(&string_expected);
    cr_std_string_free(&result_string);
    cr_std_vector_free(&vector);
    return result;
}

int cr_std_string_test_from_char_ptr_vector_null() {
    Vector *vector = NULL;
    String *result_string = cr_std_string_from_char_ptr_vector(vector, ", ");

    int result = result_string == NULL;
    return result;
}

int cr_std_string_test_sub_string() {
    String *string = cr_std_string_new("This is a Test String which i want to use!");
    String *string_expected = cr_std_string_new("Test String");
    String *result_string = cr_std_string_sub_string(string, 10, 21);

    int compare_result = cr_std_string_compare(result_string, string_expected);
    int expected_compare_result = 1;
    int result = string_expected != NULL && result_string != NULL && compare_result == expected_compare_result;

    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    cr_std_string_free(&result_string);
    return result;
}

int cr_std_string_test_sub_string_index_too_low() {
    String *string = cr_std_string_new("This is a Test String which i want to use!");
    String *string_expected = cr_std_string_new("This is a Test String");
    String *result_string = cr_std_string_sub_string(string, -24, 21);

    int compare_result = cr_std_string_compare(result_string, string_expected);
    int expected_compare_result = 1;
    int result = string_expected != NULL && result_string != NULL && compare_result == expected_compare_result;

    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    cr_std_string_free(&result_string);
    return result;
}

int cr_std_string_test_sub_string_index_too_high() {
    String *string = cr_std_string_new("This is a Test String which i want to use!");
    String *string_expected = cr_std_string_new("Test String which i want to use!");
    String *result_string = cr_std_string_sub_string(string, 10, 291);

    int compare_result = cr_std_string_compare(result_string, string_expected);
    int expected_compare_result = 1;
    int result = string_expected != NULL && result_string != NULL && compare_result == expected_compare_result;

    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    cr_std_string_free(&result_string);
    return result;
}

int cr_std_string_test_sub_string_index_invalid() {
    String *string = cr_std_string_new("This is a Test String which i want to use!");
    String *string_expected = cr_std_string_new("");
    String *result_string = cr_std_string_sub_string(string, 10, 5);

    int compare_result = cr_std_string_compare(result_string, string_expected);
    int expected_compare_result = 1;
    int result = string_expected != NULL && result_string != NULL && compare_result == expected_compare_result;

    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    cr_std_string_free(&result_string);
    return result;
}

int cr_std_string_test_sub_string_empty() {
    String *string = cr_std_string_new("");
    String *string_expected = cr_std_string_new("");
    String *result_string = cr_std_string_sub_string(string, 10, 20);

    int compare_result = cr_std_string_compare(result_string, string_expected);
    int expected_compare_result = 1;
    int result = string_expected != NULL && result_string != NULL && compare_result == expected_compare_result;

    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    cr_std_string_free(&result_string);
    return result;
}

int cr_std_string_test_sub_string_null() {
    String *string = NULL;
    String *string_expected = cr_std_string_new("");
    String *result_string = cr_std_string_sub_string(string, 10, 20);

    int compare_result = cr_std_string_compare(result_string, string_expected);
    int expected_compare_result = 1;
    int result = string_expected != NULL && result_string != NULL && compare_result == expected_compare_result;

    cr_std_string_free(&string);
    cr_std_string_free(&string_expected);
    cr_std_string_free(&result_string);
    return result;
}

/**
 * String Builder Tests Below
 */

int cr_std_string_builder_test_new_normal() {
    StringBuilder *sb = cr_std_string_builder_new("Hello World");
    int result = sb != NULL;
    cr_std_string_builder_free(&sb);
    return result;
}

int cr_std_string_builder_test_new_empty() {
    StringBuilder *sb = cr_std_string_builder_new("");
    int result = sb != NULL;
    cr_std_string_builder_free(&sb);
    return result;
}

int cr_std_string_builder_test_new_formatted() {
    StringBuilder *sb = cr_std_string_builder_newf("Hello %s", "World");
    int result = sb != NULL;
    cr_std_string_builder_free(&sb);
    return result;
}

int cr_std_string_builder_test_free() {
    StringBuilder *sb = cr_std_string_builder_new("Hello World");
    int function_result = cr_std_string_builder_free(&sb);
    int result = sb == NULL && function_result == 0;
    return result;
}

int cr_std_string_builder_test_free_null_value() {
    StringBuilder *sb = NULL;
    int function_result = cr_std_string_builder_free(&sb);
    int result = sb == NULL && function_result != 0;
    return result;
}

int cr_std_string_builder_test_append() {
    StringBuilder *sb = cr_std_string_builder_new("Hello World");
    int expected_size = 11 + 15 + 7;
    int function_result = cr_std_string_builder_append(sb, " This is a test", " Test 2");
    int result = sb != NULL && function_result == 0 && sb->size == expected_size;
    cr_std_string_builder_free(&sb);
    return result;
}

int cr_std_string_builder_test_appendf() {
    StringBuilder *sb = cr_std_string_builder_new("Hello World");
    int expected_size = 11 + 15 + 7;
    int function_result = cr_std_string_builder_appendf(sb, " This is a test%s", " Test 2");
    int result = sb != NULL && function_result == 0 && sb->size == expected_size;
    cr_std_string_builder_free(&sb);
    return result;
}

int cr_std_string_builder_test_append_single() {
    StringBuilder *sb = cr_std_string_builder_new("Hello World");
    int expected_size = 11 + 15;
    int function_result = cr_std_string_builder_append_single(sb, " This is a test");
    int result = sb != NULL && function_result == 0 && sb->size == expected_size;
    cr_std_string_builder_free(&sb);
    return result;
}

int cr_std_string_builder_test_append_empty() {
    StringBuilder *sb = cr_std_string_builder_new("Hello World");
    int expected_size = 11;
    int function_result = cr_std_string_builder_append(sb, "");
    int result = sb != NULL && function_result == 0 && sb->size == expected_size;
    cr_std_string_builder_free(&sb);
    return result;
}

int cr_std_string_builder_test_append_null_value() {
    StringBuilder *sb = cr_std_string_builder_new("Hello World");
    int expected_size = 11;
    int function_result = cr_std_string_builder_append(sb, NULL);
    int result = sb != NULL && function_result == 0 && sb->size == expected_size;
    cr_std_string_builder_free(&sb);
    return result;
}

int cr_std_string_builder_test_reset() {
    StringBuilder *sb = cr_std_string_builder_new("Hello World");
    int expected_size = 0;
    int function_result = cr_std_string_builder_reset(sb);
    int result = sb != NULL && function_result == 0 && sb->size == expected_size;
    cr_std_string_builder_free(&sb);
    return result;
}

int cr_std_string_builder_test_reset_null_value() {
    StringBuilder *sb = NULL;
    int function_result = cr_std_string_builder_reset(sb);
    int result = sb == NULL && function_result != 0;
    cr_std_string_builder_free(&sb);
    return result;
}

int cr_std_string_builder_test_to_string() {
    StringBuilder *sb = cr_std_string_builder_new("Hello World");
    int expected_size = 11;
    String *string = cr_std_string_builder_to_string(sb);
    int result = sb != NULL && string != NULL && sb->size == expected_size && string->length == expected_size;
    cr_std_string_builder_free(&sb);
    cr_std_string_free(&string);
    return result;
}

int cr_std_string_builder_test_to_string_null_value() {
    StringBuilder *sb = NULL;
    String *string = cr_std_string_builder_to_string(sb);
    int result = sb == NULL && string == NULL;
    return result;
}
