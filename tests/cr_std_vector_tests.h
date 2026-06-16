#ifndef CR_STD_VECTOR_TESTS_H
#define CR_STD_VECTOR_TESTS_H

// Test functions for all functions in cr_std_vector
#include "cr_std_vector.h"
void cr_std_vector_test_all();

// New Vector
int cr_std_vector_test_new_vector_normal();
int cr_std_vector_test_new_vector_custom();

// Push Back
int cr_std_vector_test_push_back_primitive();
int cr_std_vector_test_push_back_custom();

// Remove Element
int cr_std_vector_test_remove_element_primitive();
int cr_std_vector_test_remove_element_custom();
int cr_std_vector_test_remove_element_invalid_vector();
int cr_std_vector_test_remove_element_invalid_index();
int cr_std_vector_test_remove_element_invalid_index_negative();
int cr_std_vector_test_remove_element_empty_vector();

// Get Element
int cr_std_vector_test_get_element_primitive();
int cr_std_vector_test_get_element_custom();
int cr_std_vector_test_get_element_invalid_vector();
int cr_std_vector_test_get_element_invalid_index();
int cr_std_vector_test_get_element_invalid_index_negative();
int cr_std_vector_test_get_element_empty_vector();

// Extend Vector
int cr_std_vector_test_extend_primitive();
int cr_std_vector_test_extend_custom();

#endif // CR_STD_VECTOR_TESTS_H
