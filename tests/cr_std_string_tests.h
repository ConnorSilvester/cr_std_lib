#ifndef CR_STD_STRING_TESTS_H
#define CR_STD_STRING_TESTS_H
#include "cr_std_utils.h"

// Test functions for all functions in cr_std_string
void cr_std_string_test_all();

// New String
b8 cr_std_string_test_new_string_normal();
b8 cr_std_string_test_new_string_empty();
b8 cr_std_string_test_new_string_formatted();

// Copy String
b8 cr_std_string_test_copy_string();
b8 cr_std_string_test_copy_string_empty();

// Concat String
b8 cr_std_string_test_concat_string();
b8 cr_std_string_test_concat_string_multi();
b8 cr_std_string_test_concat_string_empty();

// Compare String
b8 cr_std_string_test_compare_length_left();
b8 cr_std_string_test_compare_length_right();
b8 cr_std_string_test_compare_partial_equal();
b8 cr_std_string_test_compare_fully_equal();
b8 cr_std_string_test_compare_empty_string();
b8 cr_std_string_test_compare_c_str_fully_equal();
b8 cr_std_string_test_compare_c_str_empty_string();

// Trim String
b8 cr_std_string_test_trim_left();
b8 cr_std_string_test_trim_right();
b8 cr_std_string_test_trim_both();
b8 cr_std_string_test_trim_both_with_special();
b8 cr_std_string_test_trim_both_word_gaps();

// Find Char
b8 cr_std_string_test_find_char_valid();
b8 cr_std_string_test_find_char_invalid();
b8 cr_std_string_test_find_last_char_valid();
b8 cr_std_string_test_find_last_char_invalid();
b8 cr_std_string_test_find_char_n_valid();
b8 cr_std_string_test_find_char_n_invalid();
b8 cr_std_string_test_find_char_n_negative();

// Find String
b8 cr_std_string_test_find_string_valid();
b8 cr_std_string_test_find_string_invalid();
b8 cr_std_string_test_find_last_string_valid();
b8 cr_std_string_test_find_last_string_invalid();
b8 cr_std_string_test_find_string_n_valid();
b8 cr_std_string_test_find_string_n_invalid();
b8 cr_std_string_test_find_string_n_negative();

// Starts With String
b8 cr_std_string_test_starts_with_string();
b8 cr_std_string_test_starts_with_string_invalid();
b8 cr_std_string_test_starts_with_string_null();

// Ends With String
b8 cr_std_string_test_ends_with_string();
b8 cr_std_string_test_ends_with_string_invalid();
b8 cr_std_string_test_ends_with_string_null();

// Starts With Char
b8 cr_std_string_test_starts_with_char();
b8 cr_std_string_test_starts_with_char_invalid();
b8 cr_std_string_test_starts_with_char_null();

// Ends With Char
b8 cr_std_string_test_ends_with_char();
b8 cr_std_string_test_ends_with_char_invalid();
b8 cr_std_string_test_ends_with_char_null();

// Char At
b8 cr_std_string_test_char_at();
b8 cr_std_string_test_char_at_invalid_index_low();
b8 cr_std_string_test_char_at_invalid_index_high();
b8 cr_std_string_test_char_at_null();

// Contains String
b8 cr_std_string_test_contains_string();
b8 cr_std_string_test_contains_string_multi();
b8 cr_std_string_test_contains_string_invalid();
b8 cr_std_string_test_contains_string_empty();

// Contains Char
b8 cr_std_string_test_contains_char();
b8 cr_std_string_test_contains_char_multi();
b8 cr_std_string_test_contains_char_invalid();

// Hash Code
b8 cr_std_string_test_hash_code();
b8 cr_std_string_test_hash_code_different();
b8 cr_std_string_test_hash_code_invalid();

// Split
b8 cr_std_string_test_split();
b8 cr_std_string_test_split_invalid();
b8 cr_std_string_test_split_null_value();
b8 cr_std_string_test_split_empty();
b8 cr_std_string_test_split_empty_split();
b8 cr_std_string_test_split_hard_empty_split();

// To Upper
b8 cr_std_string_test_to_upper();
b8 cr_std_string_test_to_upper_empty();
b8 cr_std_string_test_to_upper_invalid();

// To Lower
b8 cr_std_string_test_to_lower();
b8 cr_std_string_test_to_lower_empty();
b8 cr_std_string_test_to_lower_invalid();

// To Title
b8 cr_std_string_test_to_title();
b8 cr_std_string_test_to_title_empty();
b8 cr_std_string_test_to_title_invalid();

// Replace String
b8 cr_std_string_test_replace_string();
b8 cr_std_string_test_replace_string_multi();
b8 cr_std_string_test_replace_string_invalid();
b8 cr_std_string_test_replace_string_null_value();
b8 cr_std_string_test_replace_string_empty();

// Remove Non Numeric
b8 cr_std_string_test_remove_non_numeric();
b8 cr_std_string_test_remove_non_numeric_empty();
b8 cr_std_string_test_remove_non_numeric_null();

// Remove Numeric
b8 cr_std_string_test_remove_numeric();
b8 cr_std_string_test_remove_numeric_empty();
b8 cr_std_string_test_remove_numeric_null();

// To Int
b8 cr_std_string_test_to_int();
b8 cr_std_string_test_to_int_mixed_string();
b8 cr_std_string_test_to_int_empty();
b8 cr_std_string_test_to_int_null();

// From Int
b8 cr_std_string_test_from_int();
b8 cr_std_string_test_from_int_negative();

// Sub String
b8 cr_std_string_test_sub_string();
b8 cr_std_string_test_sub_string_index_too_low();
b8 cr_std_string_test_sub_string_index_too_high();
b8 cr_std_string_test_sub_string_index_invalid();
b8 cr_std_string_test_sub_string_null();

// Repeat
b8 cr_std_string_test_repeat();
b8 cr_std_string_test_repeat_0();
b8 cr_std_string_test_repeat_negative();

b8 cr_std_string_test_reverse();
b8 cr_std_string_test_reverse_null();

/**
 * String Builder Tests Below
 */

// String Builder New
b8 cr_std_string_builder_test_new_normal();
b8 cr_std_string_builder_test_new_empty();
b8 cr_std_string_builder_test_new_formatted();

// String Builder Ensure Capacity
b8 cr_std_string_builder_test_ensure_capacity();
b8 cr_std_string_builder_test_ensure_capacity_low();
b8 cr_std_string_builder_test_ensure_capacity_null();

// String Builder Append
b8 cr_std_string_builder_test_append();
b8 cr_std_string_builder_test_appendf();
b8 cr_std_string_builder_test_append_string();
b8 cr_std_string_builder_test_append_char();
b8 cr_std_string_builder_test_append_empty();
b8 cr_std_string_builder_test_append_null_value();

// String Builder Reset
b8 cr_std_string_builder_test_reset();
b8 cr_std_string_builder_test_reset_null_value();

// String Builder To String
b8 cr_std_string_builder_test_to_string();
b8 cr_std_string_builder_test_to_string_null_value();

#endif // CR_STD_STRING_TESTS_H
