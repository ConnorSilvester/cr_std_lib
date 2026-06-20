#include "cr_std_string_tests.h"
#include "cr_std_arena.h"
#include "cr_std_string.h"
#include "cr_std_testing.h"
#include "cr_std_utils.h"
#include "cr_std_vector.h"
#include <stdio.h>
#include <stdlib.h>

Arena *g_string_testing_arena = NULL;

void cr_std_string_test_all() {
    printf("%s\n", "Running String Tests:");

    g_string_testing_arena = cr_std_arena_new(CR_STD_MB);
    Vector *tests = cr_std_vector_new(g_string_testing_arena);

    // clang-format off
    // New String
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Make String -> Normal", cr_std_string_test_new_string_normal));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Make String -> Empty", cr_std_string_test_new_string_empty));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Make String -> Formatted", cr_std_string_test_new_string_formatted));

    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Copy String -> Normal", cr_std_string_test_copy_string));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Copy String -> Empty", cr_std_string_test_copy_string_empty));

    // Concat String
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Concat String -> Normal", cr_std_string_test_concat_string));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Concat String -> Multi", cr_std_string_test_concat_string_multi));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Concat String -> Empty", cr_std_string_test_concat_string_empty));

    // Compare String
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Compare String -> Left String Longer", cr_std_string_test_compare_length_left));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Compare String -> Right String Longer", cr_std_string_test_compare_length_right));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Compare String -> Same Length Not Equal", cr_std_string_test_compare_partial_equal));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Compare String -> Same Length And Equal", cr_std_string_test_compare_fully_equal));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Compare String -> Empty", cr_std_string_test_compare_empty_string));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Compare C_String -> Same Length And Equal", cr_std_string_test_compare_c_str_fully_equal));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Compare C_String -> Empty", cr_std_string_test_compare_c_str_empty_string));

    // Trim String
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Trim String Left -> Normal", cr_std_string_test_trim_left));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Trim String Right -> Normal", cr_std_string_test_trim_right));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Trim String Both -> Normal", cr_std_string_test_trim_both));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Trim String Both -> Special Case", cr_std_string_test_trim_both_with_special));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Trim String Both -> Word Gaps", cr_std_string_test_trim_both_word_gaps));

    // Find Char
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Find Char In String -> Valid Char", cr_std_string_test_find_char_valid));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Find Char In String -> Invalid Char", cr_std_string_test_find_char_invalid));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Find Last Char In String -> Valid Char", cr_std_string_test_find_last_char_valid));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Find Last Char In String -> Invalid Char", cr_std_string_test_find_last_char_invalid));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Find n Char In String -> Valid n", cr_std_string_test_find_char_n_valid));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Find n Char In String -> Invalid Char", cr_std_string_test_find_char_n_invalid));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Find n Char In String -> Negative n", cr_std_string_test_find_char_n_negative));

    // Find String
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Find String In String -> Valid String", cr_std_string_test_find_string_valid));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Find String In String -> Invalid String", cr_std_string_test_find_string_invalid));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Find Last String In String -> Valid String", cr_std_string_test_find_last_string_valid));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Find Last String In String -> Invalid String", cr_std_string_test_find_last_string_invalid));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Find n String In String -> Valid n", cr_std_string_test_find_string_n_valid));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Find n String In String -> Invalid String", cr_std_string_test_find_string_n_invalid));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Find n String In String -> Negative n", cr_std_string_test_find_string_n_negative));

    // Starts With String
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Starts With String -> Normal", cr_std_string_test_starts_with_string));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Starts With String -> Invalid", cr_std_string_test_starts_with_string_invalid));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Starts With String -> NULL", cr_std_string_test_starts_with_string_null));

    // Ends With String
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Ends With String -> Normal", cr_std_string_test_ends_with_string));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Ends With String -> Invalid", cr_std_string_test_ends_with_string_invalid));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Ends With String -> NULL", cr_std_string_test_ends_with_string_null));

    // Starts With Char
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Starts With char -> Normal", cr_std_string_test_starts_with_char));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Starts With char -> Invalid", cr_std_string_test_starts_with_char_invalid));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Starts With char -> NULL", cr_std_string_test_starts_with_char_null));

    // Ends With Char
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Ends With char -> Normal", cr_std_string_test_ends_with_char));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Ends With char -> Invalid", cr_std_string_test_ends_with_char_invalid));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Ends With char -> NULL", cr_std_string_test_ends_with_char_null));

    // Char At
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Char At -> Normal", cr_std_string_test_char_at));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Char At -> Invalid Index Low", cr_std_string_test_char_at_invalid_index_low));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Char At -> Invalid Index High", cr_std_string_test_char_at_invalid_index_high));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Char At -> NULL", cr_std_string_test_char_at_null));

    // Contains String
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Contains String -> Normal", cr_std_string_test_contains_string));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Contains String -> Multi", cr_std_string_test_contains_string_multi));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Contains String -> Invalid", cr_std_string_test_contains_string_invalid));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Contains String -> Empty", cr_std_string_test_contains_string_empty));

    // Contains Char
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Contains Char -> Normal", cr_std_string_test_contains_char));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Contains Char -> Multi", cr_std_string_test_contains_char_multi));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Contains Char -> Invalid", cr_std_string_test_contains_char_invalid));

    // Hash Code
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Hash Code -> Normal", cr_std_string_test_hash_code));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Hash Code -> Different", cr_std_string_test_hash_code_different));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Hash Code -> Invalid", cr_std_string_test_hash_code_invalid));

    // Split
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Split -> Normal", cr_std_string_test_split));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Split -> Invalid Char", cr_std_string_test_split_invalid));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Split -> NULL Value", cr_std_string_test_split_null_value));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Split -> Empty String", cr_std_string_test_split_empty));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Split -> Empty Split String", cr_std_string_test_split_empty_split));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Split -> Hard Split String", cr_std_string_test_split_hard_empty_split));

    // To Upper
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "To Upper -> Normal", cr_std_string_test_to_upper));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "To Upper -> Empty String", cr_std_string_test_to_upper_empty));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "To Upper -> NULL Value", cr_std_string_test_to_upper_invalid));

    // To Lower
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "To Lower -> Normal", cr_std_string_test_to_lower));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "To Lower -> Empty String", cr_std_string_test_to_lower_empty));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "To Lower -> NULL Value", cr_std_string_test_to_lower_invalid));

    // To Lower
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "To Title -> Normal", cr_std_string_test_to_title));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "To Title -> Empty String", cr_std_string_test_to_title_empty));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "To Title -> NULL Value", cr_std_string_test_to_title_invalid));

    // Replace String
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Replace String -> Normal", cr_std_string_test_replace_string));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Replace String -> Multi", cr_std_string_test_replace_string_multi));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Replace String -> Invalid", cr_std_string_test_replace_string_invalid));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Replace String -> NULL Value", cr_std_string_test_replace_string_null_value));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Replace String -> Empty", cr_std_string_test_replace_string_empty));

    // Remove Non Numeric
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Remove Non Numeric -> Normal", cr_std_string_test_remove_non_numeric));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Remove Non Numeric -> Empty", cr_std_string_test_remove_non_numeric_empty));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Remove Non Numeric -> NULL Value", cr_std_string_test_remove_non_numeric_null));

    // Remove Numeric
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Remove Numeric -> Normal", cr_std_string_test_remove_numeric));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Remove Numeric -> Empty", cr_std_string_test_remove_numeric_empty));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Remove Numeric -> NULL Value", cr_std_string_test_remove_numeric_null));

    // To Int
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "To Int -> Normal", cr_std_string_test_to_int));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "To Int -> Mixed String", cr_std_string_test_to_int_mixed_string));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "To Int -> Empty", cr_std_string_test_to_int_empty));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "To Int -> NULL Value", cr_std_string_test_to_int_null));

    // From Int
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "From Int -> Normal", cr_std_string_test_from_int));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "From Int -> Negative", cr_std_string_test_from_int_negative));

    // Sub String
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Sub String -> Normal", cr_std_string_test_sub_string));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Sub String -> Index Too Low", cr_std_string_test_sub_string_index_too_low));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Sub String -> Index Too High", cr_std_string_test_sub_string_index_too_high));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Sub String -> Index Invalid", cr_std_string_test_sub_string_index_invalid));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Sub String -> Null", cr_std_string_test_sub_string_null));

    // Repeat
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Repeat String -> Normal", cr_std_string_test_repeat));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Repeat String -> Negative", cr_std_string_test_repeat_negative));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Repeat String -> 0", cr_std_string_test_repeat_0));

    /**
     * String Builder Tests Below
     */

    // New String Builder
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Make String Builder -> Normal", cr_std_string_builder_test_new_normal));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Make String Builder -> Empty", cr_std_string_builder_test_new_empty));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Make String Builder -> Formatted", cr_std_string_builder_test_new_formatted));

    // Ensure Capacity
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Ensure Capacity -> Normal", cr_std_string_builder_test_ensure_capacity));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Ensure Capacity -> Low", cr_std_string_builder_test_ensure_capacity_low));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "Ensure Capacity -> Null", cr_std_string_builder_test_ensure_capacity_null));

    // String Builder Append
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "String Builder Append -> Normal", cr_std_string_builder_test_append));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "String Builder Append -> Formatted", cr_std_string_builder_test_appendf));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "String Builder Append -> String", cr_std_string_builder_test_append_string));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "String Builder Append -> Char", cr_std_string_builder_test_append_char));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "String Builder Append -> Empty", cr_std_string_builder_test_append_empty));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "String Builder Append -> NULL Value", cr_std_string_builder_test_append_null_value));

    // String Builder Reset
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "String Builder Reset -> Normal", cr_std_string_builder_test_reset));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "String Builder Reset -> NULL Value", cr_std_string_builder_test_reset_null_value));

    // String Builder To String
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "String Builder To String -> Normal", cr_std_string_builder_test_to_string));
    cr_std_vector_push_back(g_string_testing_arena, tests, cr_std_testing_new_test(g_string_testing_arena, "String Builder To String -> NULL Value", cr_std_string_builder_test_to_string_null_value));

    // clang-format on
    cr_std_testing_run_tests(g_string_testing_arena, tests);
    cr_std_arena_free(&g_string_testing_arena);
}

int cr_std_string_test_new_string_normal() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    int result = string != NULL;
    return result;
}

int cr_std_string_test_new_string_empty() {
    String *string = cr_std_string_new(g_string_testing_arena, "");
    int result = string != NULL;
    return result;
}

int cr_std_string_test_new_string_formatted() {
    String *string = cr_std_string_newf(g_string_testing_arena, "Hello %s", "World");
    int result = string != NULL;
    return result;
}

int cr_std_string_test_copy_string() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    String *string_copy = cr_std_string_make_copy(g_string_testing_arena, string);
    int compare_result = cr_std_string_compare(string, string_copy);
    int expected = 1;
    int result = string != NULL && string_copy != NULL && compare_result == expected;
    return result;
}

int cr_std_string_test_copy_string_empty() {
    String *string = cr_std_string_new(g_string_testing_arena, "");
    String *string_copy = cr_std_string_make_copy(g_string_testing_arena, string);
    int compare_result = cr_std_string_compare(string, string_copy);
    int expected = 1;
    int result = string != NULL && string_copy != NULL && compare_result == expected;
    return result;
}

int cr_std_string_test_concat_string() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    String *string_expected =
    cr_std_string_new(g_string_testing_arena, "Hello World, My Name Is John");
    int function_result = cr_std_string_concat(g_string_testing_arena, string, ", My Name Is John");
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result =
    string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    return result;
}

int cr_std_string_test_concat_string_multi() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    String *string_expected =
    cr_std_string_new(g_string_testing_arena, "Hello World, My Name Is John");
    int function_result =
    cr_std_string_concat(g_string_testing_arena, string, ", My Name Is", " John");
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result =
    string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    return result;
}

int cr_std_string_test_concat_string_empty() {
    String *string = cr_std_string_new(g_string_testing_arena, "");
    String *string_expected = cr_std_string_new(g_string_testing_arena, "Hello World");
    int function_result = cr_std_string_concat(g_string_testing_arena, string, "Hello World");
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result =
    string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    return result;
}

int cr_std_string_test_compare_length_left() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    String *string2 = cr_std_string_new(g_string_testing_arena, "Test");
    int compare_result = cr_std_string_compare(string, string2);
    int expected = -1;
    int result = string != NULL && string2 != NULL && compare_result == expected;
    return result;
}

int cr_std_string_test_compare_length_right() {
    String *string = cr_std_string_new(g_string_testing_arena, "Test");
    String *string2 = cr_std_string_new(g_string_testing_arena, "Hello World");
    int compare_result = cr_std_string_compare(string, string2);
    int expected = -2;
    int result = string != NULL && string2 != NULL && compare_result == expected;
    return result;
}

int cr_std_string_test_compare_partial_equal() {
    String *string = cr_std_string_new(g_string_testing_arena, "hello world");
    String *string2 = cr_std_string_new(g_string_testing_arena, "Hello World");
    int compare_result = cr_std_string_compare(string, string2);
    int expected = 0;
    int result = string != NULL && string2 != NULL && compare_result == expected;
    return result;
}

int cr_std_string_test_compare_fully_equal() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    String *string2 = cr_std_string_new(g_string_testing_arena, "Hello World");
    int compare_result = cr_std_string_compare(string, string2);
    int expected = 1;
    int result = string != NULL && string2 != NULL && compare_result == expected;
    return result;
}

int cr_std_string_test_compare_empty_string() {
    String *string = cr_std_string_new(g_string_testing_arena, "");
    String *string2 = cr_std_string_new(g_string_testing_arena, "");
    int compare_result = cr_std_string_compare(string, string2);
    int expected = 1;
    int result = string != NULL && string2 != NULL && compare_result == expected;
    return result;
}

int cr_std_string_test_compare_c_str_fully_equal() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    int compare_result = cr_std_string_compare_c_str(string, "Hello World");
    int expected = 1;
    int result = string != NULL && compare_result == expected;
    return result;
}

int cr_std_string_test_compare_c_str_empty_string() {
    String *string = cr_std_string_new(g_string_testing_arena, "");
    int compare_result = cr_std_string_compare_c_str(string, "");
    int expected = 1;
    int result = string != NULL && compare_result == expected;
    return result;
}

int cr_std_string_test_trim_left() {
    String *string = cr_std_string_new(g_string_testing_arena, "     Hello World   ");
    String *string_expected = cr_std_string_new(g_string_testing_arena, "Hello World   ");
    int function_result = cr_std_string_trim(string, CR_STD_STRING_TRIM_LEFT);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result =
    string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    return result;
}

int cr_std_string_test_trim_right() {
    String *string = cr_std_string_new(g_string_testing_arena, "   Hello World     ");
    String *string_expected = cr_std_string_new(g_string_testing_arena, "   Hello World");
    int function_result = cr_std_string_trim(string, CR_STD_STRING_TRIM_RIGHT);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result =
    string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    return result;
}

int cr_std_string_test_trim_both() {
    String *string = cr_std_string_new(g_string_testing_arena, "   Hello World     ");
    String *string_expected = cr_std_string_new(g_string_testing_arena, "Hello World");
    int function_result = cr_std_string_trim(string, CR_STD_STRING_TRIM_BOTH);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result =
    string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    return result;
}

int cr_std_string_test_trim_both_with_special() {
    String *string = cr_std_string_new(g_string_testing_arena, " \n\r  Hello World   \n\n\r  ");
    String *string_expected = cr_std_string_new(g_string_testing_arena, "Hello World");
    int function_result = cr_std_string_trim(string, CR_STD_STRING_TRIM_BOTH);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result =
    string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    return result;
}

int cr_std_string_test_trim_both_word_gaps() {
    String *string =
    cr_std_string_new(g_string_testing_arena, " \n\r  Hello \n\n World   \n\n\r  ");
    String *string_expected = cr_std_string_new(g_string_testing_arena, "Hello \n\n World");
    int function_result = cr_std_string_trim(string, CR_STD_STRING_TRIM_BOTH);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result =
    string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    return result;
}

int cr_std_string_test_find_char_valid() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    int function_result = cr_std_string_find_char(string, 'W');
    int expected = 6;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_find_char_invalid() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    int function_result = cr_std_string_find_char(string, 'x');
    int expected = -1;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_find_char_n_valid() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    int function_result = cr_std_string_find_char_n(string, 'l', 2);
    int expected = 3;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_find_char_n_invalid() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    int function_result = cr_std_string_find_char_n(string, 'x', 2);
    int expected = -1;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_find_char_n_negative() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    int function_result = cr_std_string_find_char_n(string, 'l', -2);
    int expected = 2;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_find_last_char_valid() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello Hello");
    int function_result = cr_std_string_find_char_last(string, 'H');
    int expected = 6;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_find_last_char_invalid() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    int function_result = cr_std_string_find_char_last(string, 'x');
    int expected = -1;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_find_string_valid() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    int function_result = cr_std_string_find_string(string, "World");
    int expected = 6;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_find_string_invalid() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    int function_result = cr_std_string_find_string(string, "Bench");
    int expected = -1;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_find_last_string_valid() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello Hello Hello Hello");
    int function_result = cr_std_string_find_string_last(string, "Hello");
    int expected = 18;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_find_last_string_invalid() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    int function_result = cr_std_string_find_string_last(string, "Bench");
    int expected = -1;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_find_string_n_valid() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World World");
    int function_result = cr_std_string_find_string_n(string, "World", 2);
    int expected = 12;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_find_string_n_invalid() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    int function_result = cr_std_string_find_string_n(string, "Bench", 1);
    int expected = -1;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_find_string_n_negative() {
    String *string = cr_std_string_new(g_string_testing_arena, "World World World");
    int function_result = cr_std_string_find_string_n(string, "World", -2);
    int expected = 0;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_starts_with_string() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    char *phrase = "Hello";
    int function_result = cr_std_string_starts_with_string(string, phrase);
    int expected = 1;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_starts_with_string_invalid() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    char *phrase = "Helo";
    int function_result = cr_std_string_starts_with_string(string, phrase);
    int expected = 0;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_starts_with_string_null() {
    String *string = NULL;
    char *phrase = "Hello";
    int function_result = cr_std_string_starts_with_string(string, phrase);
    int expected = 0;
    int result = function_result == expected;
    return result;
}

int cr_std_string_test_ends_with_string() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    char *phrase = "World";
    int function_result = cr_std_string_ends_with_string(string, phrase);
    int expected = 1;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_ends_with_string_invalid() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    char *phrase = "Hello";
    int function_result = cr_std_string_ends_with_string(string, phrase);
    int expected = 0;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_ends_with_string_null() {
    String *string = NULL;
    char *phrase = "Hello";
    int function_result = cr_std_string_ends_with_string(string, phrase);
    int expected = 0;
    int result = function_result == expected;
    return result;
}

int cr_std_string_test_starts_with_char() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    int function_result = cr_std_string_starts_with_char(string, 'H');
    int expected = 1;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_starts_with_char_invalid() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    int function_result = cr_std_string_starts_with_char(string, 'x');
    int expected = 0;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_starts_with_char_null() {
    String *string = NULL;
    int function_result = cr_std_string_starts_with_char(string, 'H');
    int expected = 0;
    int result = function_result == expected;
    return result;
}

int cr_std_string_test_ends_with_char() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    int function_result = cr_std_string_ends_with_char(string, 'd');
    int expected = 1;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_ends_with_char_invalid() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    int function_result = cr_std_string_ends_with_char(string, 'x');
    int expected = 0;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_ends_with_char_null() {
    String *string = NULL;
    int function_result = cr_std_string_ends_with_char(string, 'H');
    int expected = 0;
    int result = function_result == expected;
    return result;
}

int cr_std_string_test_char_at() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    char function_result = cr_std_string_char_at(string, 1);
    char expected = 'e';
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_char_at_invalid_index_low() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    char function_result = cr_std_string_char_at(string, -1);
    char expected = 'H';
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_char_at_invalid_index_high() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    char function_result = cr_std_string_char_at(string, 111);
    char expected = 'd';
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_char_at_null() {
    String *string = NULL;
    char function_result = cr_std_string_char_at(string, 1);
    char expected = ' ';
    int result = function_result == expected;
    return result;
}

int cr_std_string_test_contains_string() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    char *phrase = "Hello";
    int function_result = cr_std_string_contains_string(string, phrase);
    int expected = 1;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_contains_string_multi() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World Hello");
    char *phrase = "Hello";
    int function_result = cr_std_string_contains_string(string, phrase);
    int expected = 2;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_contains_string_invalid() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    char *phrase = "John";
    int function_result = cr_std_string_contains_string(string, phrase);
    int expected = 0;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_contains_string_empty() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    char *phrase = "";
    int function_result = cr_std_string_contains_string(string, phrase);
    int expected = 0;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_contains_char() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    char ch = 'H';
    int function_result = cr_std_string_contains_char(string, ch);
    int expected = 1;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_contains_char_multi() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    char ch = 'o';
    int function_result = cr_std_string_contains_char(string, ch);
    int expected = 2;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_contains_char_invalid() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    char ch = 'x';
    int function_result = cr_std_string_contains_char(string, ch);
    int expected = 0;
    int result = string != NULL && function_result == expected;
    return result;
}

int cr_std_string_test_hash_code() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    String *test_case = cr_std_string_new(g_string_testing_arena, "Hello World");
    u64 function_one_result;
    cr_std_string_hash_code(string, &function_one_result);
    u64 function_two_result;
    cr_std_string_hash_code(test_case, &function_two_result);
    int result = string != NULL && test_case != NULL && function_one_result == function_two_result;
    return result;
}

int cr_std_string_test_hash_code_different() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    String *test_case = cr_std_string_new(g_string_testing_arena, " Hello World");
    u64 function_one_result;
    cr_std_string_hash_code(string, &function_one_result);
    u64 function_two_result;
    cr_std_string_hash_code(test_case, &function_two_result);
    int result = string != NULL && test_case != NULL && function_one_result != function_two_result;
    return result;
}

int cr_std_string_test_hash_code_invalid() {
    String *string = NULL;
    u8 function_result = cr_std_string_hash_code(string, NULL);
    u8 expected = CR_STD_FAIL;
    int result = function_result == expected;
    return result;
}

int cr_std_string_test_split() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    Vector *vector = cr_std_string_split(g_string_testing_arena, string, ' ');
    int actual_size = vector->size;
    int expected_size = 2;
    int result = string != NULL && vector != NULL && actual_size == expected_size;
    return result;
}

int cr_std_string_test_split_invalid() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    Vector *vector = cr_std_string_split(g_string_testing_arena, string, 'x');
    int actual_size = vector->size;
    int expected_size = 1;
    int result = string != NULL && vector != NULL && actual_size == expected_size;
    return result;
}

int cr_std_string_test_split_null_value() {
    String *string = NULL;
    Vector *vector = cr_std_string_split(g_string_testing_arena, string, 'x');
    int result = vector == NULL;
    return result;
}

int cr_std_string_test_split_empty() {
    String *string = cr_std_string_new(g_string_testing_arena, "");
    Vector *vector = cr_std_string_split(g_string_testing_arena, string, 'x');
    int actual_size = vector->size;
    int expected_size = 0;
    int result = string != NULL && vector != NULL && actual_size == expected_size;
    return result;
}

int cr_std_string_test_split_empty_split() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World   ");
    Vector *vector = cr_std_string_split(g_string_testing_arena, string, ' ');
    int actual_size = vector->size;
    int expected_size = 2;
    int result = string != NULL && vector != NULL && actual_size == expected_size;
    return result;
}

int cr_std_string_test_split_hard_empty_split() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World   ");
    Vector *vector = cr_std_string_split_hard(g_string_testing_arena, string, ' ');
    int actual_size = vector->size;
    int expected_size = 4;
    int result = string != NULL && vector != NULL && actual_size == expected_size;
    return result;
}

int cr_std_string_test_to_upper() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    String *string_expected = cr_std_string_new(g_string_testing_arena, "HELLO WORLD");
    int function_result = cr_std_string_to_upper(string);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result =
    string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    return result;
}

int cr_std_string_test_to_upper_empty() {
    String *string = cr_std_string_new(g_string_testing_arena, "");
    String *string_expected = cr_std_string_new(g_string_testing_arena, "");
    int function_result = cr_std_string_to_upper(string);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result =
    string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    return result;
}

int cr_std_string_test_to_upper_invalid() {
    String *string = NULL;
    int function_result = cr_std_string_to_upper(string);
    return function_result != 0;
}

int cr_std_string_test_to_lower() {
    String *string = cr_std_string_new(g_string_testing_arena, "HELLO WORLD");
    String *string_expected = cr_std_string_new(g_string_testing_arena, "hello world");
    int function_result = cr_std_string_to_lower(string);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result =
    string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    return result;
}

int cr_std_string_test_to_lower_empty() {
    String *string = cr_std_string_new(g_string_testing_arena, "");
    String *string_expected = cr_std_string_new(g_string_testing_arena, "");
    int function_result = cr_std_string_to_lower(string);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result =
    string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    return result;
}

int cr_std_string_test_to_lower_invalid() {
    String *string = NULL;
    int function_result = cr_std_string_to_lower(string);
    return function_result != 0;
}

int cr_std_string_test_to_title() {
    String *string = cr_std_string_new(g_string_testing_arena, "HELLO WORLD");
    String *string_expected = cr_std_string_new(g_string_testing_arena, "Hello World");
    int function_result = cr_std_string_to_title(string);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result =
    string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    return result;
}

int cr_std_string_test_to_title_empty() {
    String *string = cr_std_string_new(g_string_testing_arena, "");
    String *string_expected = cr_std_string_new(g_string_testing_arena, "");
    int function_result = cr_std_string_to_upper(string);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result =
    string != NULL && string_expected != NULL && compare_result == expected && function_result == 0;
    return result;
}

int cr_std_string_test_to_title_invalid() {
    String *string = NULL;
    int function_result = cr_std_string_to_title(string);
    return function_result != 0;
}

int cr_std_string_test_replace_string() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    String *string_expected = cr_std_string_new(g_string_testing_arena, "Test World");
    int function_result =
    cr_std_string_replace_string(g_string_testing_arena, string, "Hello", "Test");
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected = 1;
    int result = string != NULL && string_expected != NULL && compare_result == expected &&
                 function_result == expected;
    return result;
}

int cr_std_string_test_replace_string_multi() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello, Hello World");
    String *string_expected = cr_std_string_new(g_string_testing_arena, "Test, Test World");
    int function_result =
    cr_std_string_replace_string(g_string_testing_arena, string, "Hello", "Test");
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected_function_result = 2;
    int expected_compare_result = 1;
    int result = string != NULL && string_expected != NULL &&
                 compare_result == expected_compare_result &&
                 function_result == expected_function_result;
    return result;
}

int cr_std_string_test_replace_string_invalid() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello World");
    String *string_expected = cr_std_string_new(g_string_testing_arena, "Hello World");
    int function_result = cr_std_string_replace_string(g_string_testing_arena, string, "x", "Test");
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected_function_result = 0;
    int expected_compare_result = 1;
    int result = string != NULL && string_expected != NULL &&
                 compare_result == expected_compare_result &&
                 function_result == expected_function_result;
    return result;
}

int cr_std_string_test_replace_string_null_value() {
    String *string = NULL;
    int function_result =
    cr_std_string_replace_string(g_string_testing_arena, string, "Hello", "Test");
    int expected = 0;
    return function_result == expected;
}

int cr_std_string_test_replace_string_empty() {
    String *string = cr_std_string_new(g_string_testing_arena, "");
    String *string_expected = cr_std_string_new(g_string_testing_arena, "");
    int function_result = cr_std_string_replace_string(g_string_testing_arena, string, "x", "Test");
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected_function_result = 0;
    int expected_compare_result = 1;
    int result = string != NULL && string_expected != NULL &&
                 compare_result == expected_compare_result &&
                 function_result == expected_function_result;
    return result;
}

int cr_std_string_test_remove_non_numeric() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello667788, Hello World");
    String *string_expected = cr_std_string_new(g_string_testing_arena, "667788");
    int function_result = cr_std_string_remove_non_numeric(string);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected_function_result = 0;
    int expected_compare_result = 1;
    int result = string != NULL && string_expected != NULL &&
                 compare_result == expected_compare_result &&
                 function_result == expected_function_result;
    return result;
}

int cr_std_string_test_remove_non_numeric_empty() {
    String *string = cr_std_string_new(g_string_testing_arena, "");
    String *string_expected = cr_std_string_new(g_string_testing_arena, "");
    int function_result = cr_std_string_remove_non_numeric(string);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected_function_result = 0;
    int expected_compare_result = 1;
    int result = string != NULL && string_expected != NULL &&
                 compare_result == expected_compare_result &&
                 function_result == expected_function_result;
    return result;
}

int cr_std_string_test_remove_non_numeric_null() {
    String *string = NULL;
    int function_result = cr_std_string_remove_non_numeric(string);
    return function_result != 0;
}

int cr_std_string_test_remove_numeric() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello667788, Hello World");
    String *string_expected = cr_std_string_new(g_string_testing_arena, "Hello, Hello World");
    int function_result = cr_std_string_remove_numeric(string);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected_function_result = 0;
    int expected_compare_result = 1;
    int result = string != NULL && string_expected != NULL &&
                 compare_result == expected_compare_result &&
                 function_result == expected_function_result;
    return result;
}

int cr_std_string_test_remove_numeric_empty() {
    String *string = cr_std_string_new(g_string_testing_arena, "");
    String *string_expected = cr_std_string_new(g_string_testing_arena, "");
    int function_result = cr_std_string_remove_numeric(string);
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected_function_result = 0;
    int expected_compare_result = 1;
    int result = string != NULL && string_expected != NULL &&
                 compare_result == expected_compare_result &&
                 function_result == expected_function_result;
    return result;
}

int cr_std_string_test_remove_numeric_null() {
    String *string = NULL;
    int function_result = cr_std_string_remove_numeric(string);
    return function_result != 0;
}

int cr_std_string_test_to_int() {
    String *string = cr_std_string_new(g_string_testing_arena, "555666");
    int function_result;
    cr_std_string_to_int(string, &function_result);
    int expected_function_result = 555666;
    int result = string != NULL && function_result == expected_function_result;
    return result;
}

int cr_std_string_test_to_int_mixed_string() {
    String *string = cr_std_string_new(g_string_testing_arena, "Hello555, Hello 666World");
    int function_result;
    cr_std_string_to_int(string, &function_result);
    int expected_function_result = 555666;
    int result = string != NULL && function_result == expected_function_result;
    return result;
}

int cr_std_string_test_to_int_empty() {
    String *string = cr_std_string_new(g_string_testing_arena, "");
    int function_result;
    cr_std_string_to_int(string, &function_result);
    int expected_function_result = 0;
    int result = string != NULL && function_result == expected_function_result;
    return result;
}

int cr_std_string_test_to_int_null() {
    String *string = NULL;
    int function_result;
    cr_std_string_to_int(string, &function_result);
    int expected_function_result = 0;
    int result = function_result == expected_function_result;
    return result;
}

int cr_std_string_test_from_int() {
    String *string = cr_std_string_from_int(g_string_testing_arena, 12345);
    String *string_expected = cr_std_string_new(g_string_testing_arena, "12345");
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected_compare_result = 1;
    int result =
    string != NULL && string_expected != NULL && compare_result == expected_compare_result;
    return result;
}

int cr_std_string_test_from_int_negative() {
    String *string = cr_std_string_from_int(g_string_testing_arena, -12345);
    String *string_expected = cr_std_string_new(g_string_testing_arena, "-12345");
    int compare_result = cr_std_string_compare(string, string_expected);
    int expected_compare_result = 1;
    int result =
    string != NULL && string_expected != NULL && compare_result == expected_compare_result;
    return result;
}

int cr_std_string_test_sub_string() {
    String *string =
    cr_std_string_new(g_string_testing_arena, "This is a Test String which i want to use!");
    String *string_expected = cr_std_string_new(g_string_testing_arena, "Test String");
    String *result_string = cr_std_string_sub_string(g_string_testing_arena, string, 10, 21);

    int compare_result = cr_std_string_compare(result_string, string_expected);
    int expected_compare_result = 1;
    int result =
    string_expected != NULL && result_string != NULL && compare_result == expected_compare_result;

    return result;
}

int cr_std_string_test_sub_string_index_too_low() {
    String *string =
    cr_std_string_new(g_string_testing_arena, "This is a Test String which i want to use!");
    String *string_expected = cr_std_string_new(g_string_testing_arena, "This is a Test String");
    String *result_string = cr_std_string_sub_string(g_string_testing_arena, string, -24, 21);

    int compare_result = cr_std_string_compare(result_string, string_expected);
    int expected_compare_result = 1;
    int result =
    string_expected != NULL && result_string != NULL && compare_result == expected_compare_result;

    return result;
}

int cr_std_string_test_sub_string_index_too_high() {
    String *string =
    cr_std_string_new(g_string_testing_arena, "This is a Test String which i want to use!");
    String *string_expected =
    cr_std_string_new(g_string_testing_arena, "Test String which i want to use!");
    String *result_string = cr_std_string_sub_string(g_string_testing_arena, string, 10, 291);

    int compare_result = cr_std_string_compare(result_string, string_expected);
    int expected_compare_result = 1;
    int result =
    string_expected != NULL && result_string != NULL && compare_result == expected_compare_result;

    return result;
}

int cr_std_string_test_sub_string_index_invalid() {
    String *string =
    cr_std_string_new(g_string_testing_arena, "This is a Test String which i want to use!");
    String *string_expected = cr_std_string_new(g_string_testing_arena, "");
    String *result_string = cr_std_string_sub_string(g_string_testing_arena, string, 10, 5);

    int compare_result = cr_std_string_compare(result_string, string_expected);
    int expected_compare_result = 1;
    int result =
    string_expected != NULL && result_string != NULL && compare_result == expected_compare_result;

    return result;
}

int cr_std_string_test_sub_string_null() {
    String *string = NULL;
    String *string_expected = cr_std_string_new(g_string_testing_arena, "");
    String *result_string = cr_std_string_sub_string(g_string_testing_arena, string, 10, 20);

    int compare_result = cr_std_string_compare(result_string, string_expected);
    int expected_compare_result = 1;
    int result =
    string_expected != NULL && result_string != NULL && compare_result == expected_compare_result;

    return result;
}

int cr_std_string_test_repeat() {
    String *string = cr_std_string_repeat(g_string_testing_arena, "Test", 2);
    String *string_expected = cr_std_string_new(g_string_testing_arena, "TestTest");

    int compare_result = cr_std_string_compare(string, string_expected);
    int expected_compare_result = 1;
    int result =
    string_expected != NULL && string != NULL && compare_result == expected_compare_result;

    return result;
}

int cr_std_string_test_repeat_0() {
    String *string = cr_std_string_repeat(g_string_testing_arena, "Test", 0);
    String *string_expected = cr_std_string_new(g_string_testing_arena, "");

    int compare_result = cr_std_string_compare(string, string_expected);
    int expected_compare_result = 1;
    int result =
    string_expected != NULL && string != NULL && compare_result == expected_compare_result;

    return result;
}

int cr_std_string_test_repeat_negative() {
    String *string = cr_std_string_repeat(g_string_testing_arena, "Test", -1);
    String *string_expected = cr_std_string_new(g_string_testing_arena, "");

    int compare_result = cr_std_string_compare(string, string_expected);
    int expected_compare_result = 1;
    int result =
    string_expected != NULL && string != NULL && compare_result == expected_compare_result;

    return result;
}

/**
 * String Builder Tests Below
 */

int cr_std_string_builder_test_new_normal() {
    StringBuilder *sb = cr_std_string_builder_new(g_string_testing_arena, "Hello World");
    int result = sb != NULL;
    return result;
}

int cr_std_string_builder_test_new_empty() {
    StringBuilder *sb = cr_std_string_builder_new(g_string_testing_arena, "");
    int result = sb != NULL;
    return result;
}

int cr_std_string_builder_test_new_formatted() {
    StringBuilder *sb = cr_std_string_builder_newf(g_string_testing_arena, "Hello %s", "World");
    int result = sb != NULL;
    return result;
}

int cr_std_string_builder_test_ensure_capacity() {
    StringBuilder *sb = cr_std_string_builder_new(g_string_testing_arena, "");
    size_t initial_capacity = sb->capacity;
    size_t desired_add = 1024;

    cr_std_string_builder_ensure_capacity(g_string_testing_arena, sb, desired_add);

    size_t expected_min = sb->size + desired_add + 1;
    int result = sb != NULL && sb->capacity >= expected_min;

    return result;
}

int cr_std_string_builder_test_ensure_capacity_low() {
    StringBuilder *sb = cr_std_string_builder_new(g_string_testing_arena, "");
    int desired_add = 16;
    cr_std_string_builder_ensure_capacity(g_string_testing_arena, sb, desired_add);
    int result = sb != NULL && sb->capacity == CR_STD_STRING_BUILDER_DEFAULT_CAP;
    return result;
}

int cr_std_string_builder_test_ensure_capacity_null() {
    StringBuilder *sb = NULL;
    int desired_add = 16;
    int function_result =
    cr_std_string_builder_ensure_capacity(g_string_testing_arena, sb, desired_add);
    int result = function_result == 1;
    return result;
}

int cr_std_string_builder_test_append() {
    StringBuilder *sb = cr_std_string_builder_new(g_string_testing_arena, "Hello World");
    int expected_size = 11 + 15 + 7;
    int function_result =
    cr_std_string_builder_append(g_string_testing_arena, sb, " This is a test", " Test 2");
    int result = sb != NULL && function_result == 0 && sb->size == expected_size;
    return result;
}

int cr_std_string_builder_test_appendf() {
    StringBuilder *sb = cr_std_string_builder_new(g_string_testing_arena, "Hello World");
    int expected_size = 11 + 15 + 7;
    int function_result =
    cr_std_string_builder_appendf(g_string_testing_arena, sb, " This is a test%s", " Test 2");
    int result = sb != NULL && function_result == 0 && sb->size == expected_size;
    return result;
}

int cr_std_string_builder_test_append_string() {
    StringBuilder *sb = cr_std_string_builder_new(g_string_testing_arena, "Hello World");
    int expected_size = 11 + 15;
    int function_result =
    cr_std_string_builder_append_string(g_string_testing_arena, sb, " This is a test");
    int result = sb != NULL && function_result == 0 && sb->size == expected_size;
    return result;
}

int cr_std_string_builder_test_append_char() {
    StringBuilder *sb = cr_std_string_builder_new(g_string_testing_arena, "Hello World");
    int expected_size = 11 + 1;
    int function_result = cr_std_string_builder_append_char(g_string_testing_arena, sb, '!');
    int result = sb != NULL && function_result == 0 && sb->size == expected_size;
    return result;
}

int cr_std_string_builder_test_append_empty() {
    StringBuilder *sb = cr_std_string_builder_new(g_string_testing_arena, "Hello World");
    int expected_size = 11;
    int function_result = cr_std_string_builder_append(g_string_testing_arena, sb, "");
    int result = sb != NULL && function_result == 0 && sb->size == expected_size;
    return result;
}

int cr_std_string_builder_test_append_null_value() {
    StringBuilder *sb = cr_std_string_builder_new(g_string_testing_arena, "Hello World");
    int expected_size = 11;
    int function_result = cr_std_string_builder_append(g_string_testing_arena, sb, NULL);
    int result = sb != NULL && function_result == 0 && sb->size == expected_size;
    return result;
}

int cr_std_string_builder_test_reset() {
    StringBuilder *sb = cr_std_string_builder_new(g_string_testing_arena, "Hello World");
    int expected_size = 0;
    int function_result = cr_std_string_builder_reset(sb);
    int result = sb != NULL && function_result == 0 && sb->size == expected_size;
    return result;
}

int cr_std_string_builder_test_reset_null_value() {
    StringBuilder *sb = NULL;
    int function_result = cr_std_string_builder_reset(sb);
    int result = sb == NULL && function_result != 0;
    return result;
}

int cr_std_string_builder_test_to_string() {
    StringBuilder *sb = cr_std_string_builder_new(g_string_testing_arena, "Hello World");
    int expected_size = 11;
    String *string = cr_std_string_builder_to_string(g_string_testing_arena, sb);
    int result =
    sb != NULL && string != NULL && sb->size == expected_size && string->length == expected_size;
    return result;
}

int cr_std_string_builder_test_to_string_null_value() {
    StringBuilder *sb = NULL;
    String *string = cr_std_string_builder_to_string(g_string_testing_arena, sb);
    int result = sb == NULL && string == NULL;
    return result;
}
