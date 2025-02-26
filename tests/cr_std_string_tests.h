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
int cr_std_string_test_compare_c_str_fully_equal();
int cr_std_string_test_compare_c_str_empty_string();

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

// Remove Non Numeric
int cr_std_string_test_remove_non_numeric();
int cr_std_string_test_remove_non_numeric_empty();
int cr_std_string_test_remove_non_numeric_null();

// Remove Numeric
int cr_std_string_test_remove_numeric();
int cr_std_string_test_remove_numeric_empty();
int cr_std_string_test_remove_numeric_null();

// To Int
int cr_std_string_test_to_int();
int cr_std_string_test_to_int_mixed_string();
int cr_std_string_test_to_int_empty();
int cr_std_string_test_to_int_null();

// From Int
int cr_std_string_test_from_int();
int cr_std_string_test_from_int_negative();

// From String Pointer Vector
int cr_std_string_test_from_string_ptr_vector();
int cr_std_string_test_from_string_ptr_vector_empty();
int cr_std_string_test_from_string_ptr_vector_null();

// From Char Pointer Vector
int cr_std_string_test_from_char_ptr_vector();
int cr_std_string_test_from_char_ptr_vector_empty();
int cr_std_string_test_from_char_ptr_vector_null();

int cr_std_string_test_sub_string();
int cr_std_string_test_sub_string_index_too_low();
int cr_std_string_test_sub_string_index_too_high();
int cr_std_string_test_sub_string_index_invalid();
int cr_std_string_test_sub_string_empty();
int cr_std_string_test_sub_string_null();


/**
 * String Builder Tests Below
 */

// String Builder New
int cr_std_string_builder_test_new_normal();
int cr_std_string_builder_test_new_empty();
int cr_std_string_builder_test_new_formatted();

// String Builder Free
int cr_std_string_builder_test_free();
int cr_std_string_builder_test_free_null_value();

// String Builder Append
int cr_std_string_builder_test_append();
int cr_std_string_builder_test_appendf();
int cr_std_string_builder_test_append_single();
int cr_std_string_builder_test_append_empty();
int cr_std_string_builder_test_append_null_value();

// String Builder Reset
int cr_std_string_builder_test_reset();
int cr_std_string_builder_test_reset_null_value();

// String Builder To String
int cr_std_string_builder_test_to_string();
int cr_std_string_builder_test_to_string_null_value();

#endif // CR_STD_STRING_TESTS_H
