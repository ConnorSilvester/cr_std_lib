#ifndef CR_STD_STRING_TESTS_H
#define CR_STD_STRING_TESTS_H

// Test functions for all functions in cr_std_string
void cr_std_string_test_all();

// New String
int cr_std_string_test_new_string_normal();
int cr_std_string_test_new_string_empty();
int cr_std_string_test_new_string_formatted();

// Free String
int cr_std_string_test_free();
int cr_std_string_test_free_null_value();

// Copy String
int cr_std_string_test_copy_string();
int cr_std_string_test_copy_string_empty();

// Concat String
int cr_std_string_test_concat_string();
int cr_std_string_test_concat_string_multi();
int cr_std_string_test_concat_string_empty();

// Compare String
int cr_std_string_test_compare_length_left();
int cr_std_string_test_compare_length_right();
int cr_std_string_test_compare_partial_equal();
int cr_std_string_test_compare_fully_equal();
int cr_std_string_test_compare_empty_string();

// Trim String
int cr_std_string_test_trim_left();
int cr_std_string_test_trim_right();
int cr_std_string_test_trim_both();
int cr_std_string_test_trim_both_with_special();
int cr_std_string_test_trim_both_word_gaps();

// Find Char
int cr_std_string_test_find_char_valid();
int cr_std_string_test_find_char_invalid();
int cr_std_string_test_find_last_char_valid();
int cr_std_string_test_find_last_char_invalid();

// Contains String
int cr_std_string_test_contains_string();
int cr_std_string_test_contains_string_multi();
int cr_std_string_test_contains_string_invalid();
int cr_std_string_test_contains_string_empty();

// Contains Char
int cr_std_string_test_contains_char();
int cr_std_string_test_contains_char_multi();
int cr_std_string_test_contains_char_invalid();

// Hash Code
int cr_std_string_test_hash_code();
int cr_std_string_test_hash_code_different();
int cr_std_string_test_hash_code_invalid();

// Split
int cr_std_string_test_split();
int cr_std_string_test_split_invalid();
int cr_std_string_test_split_null_value();
int cr_std_string_test_split_empty();

// To Upper
int cr_std_string_test_to_upper();
int cr_std_string_test_to_upper_empty();
int cr_std_string_test_to_upper_invalid();

// To Lower
int cr_std_string_test_to_lower();
int cr_std_string_test_to_lower_empty();
int cr_std_string_test_to_lower_invalid();

// Replace String
int cr_std_string_test_replace_string();
int cr_std_string_test_replace_string_multi();
int cr_std_string_test_replace_string_invalid();
int cr_std_string_test_replace_string_null_value();
int cr_std_string_test_replace_string_empty();

#endif // CR_STD_STRING_TESTS_H
