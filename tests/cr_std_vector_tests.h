#ifndef CR_STD_VECTOR_TESTS_H
#define CR_STD_VECTOR_TESTS_H

// Test functions for all functions in cr_std_vector
#include "cr_std_vector.h"
void cr_std_vector_test_all();

// New Vector
int cr_std_vector_test_new_vector_normal();
int cr_std_vector_test_new_vector_normal_pointer();
int cr_std_vector_test_new_vector_custom();
int cr_std_vector_test_new_vector_custom_pointer();

// Free Vector
int cr_std_vector_test_free();
int cr_std_vector_test_free_pointer_vector();
int cr_std_vector_test_free_null();

// Push Back
int cr_std_vector_test_push_back_primitive();
int cr_std_vector_test_push_back_primitive_pointer();
int cr_std_vector_test_push_back_custom();
int cr_std_vector_test_push_back_custom_pointer();

// Remove Element
int cr_std_vector_test_remove_element_primitive();
int cr_std_vector_test_remove_element_primitive_pointer();
int cr_std_vector_test_remove_element_custom();
int cr_std_vector_test_remove_element_custom_pointer();
int cr_std_vector_test_remove_element_invalid_vector();
int cr_std_vector_test_remove_element_invalid_index();
int cr_std_vector_test_remove_element_invalid_index_negative();
int cr_std_vector_test_remove_element_empty_vector();

// Get Element
int cr_std_vector_test_get_element_primitive();
int cr_std_vector_test_get_element_primitive_pointer();
int cr_std_vector_test_get_element_custom();
int cr_std_vector_test_get_element_custom_pointer();
int cr_std_vector_test_get_element_invalid_vector();
int cr_std_vector_test_get_element_invalid_index();
int cr_std_vector_test_get_element_invalid_index_negative();
int cr_std_vector_test_get_element_empty_vector();

// Extend Vector
int cr_std_vector_test_extend_primitive_type();
int cr_std_vector_test_extend_primitive_pointer();
int cr_std_vector_test_extend_custom();
int cr_std_vector_test_extend_custom_pointer();
int cr_std_vector_test_extend_custom_pointer_copy_elements();
int cr_std_vector_test_extend_incorrect_types();

#endif // CR_STD_VECTOR_TESTS_H
