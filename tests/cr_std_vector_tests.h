#ifndef CR_STD_VECTOR_TESTS_H
#define CR_STD_VECTOR_TESTS_H
#include "cr_std_utils.h"

// Test functions for all functions in cr_std_vector
void cr_std_vector_test_all();

// New Vector
b8 cr_std_vector_test_new_vector_normal();
b8 cr_std_vector_test_new_vector_custom();

// Push Back
b8 cr_std_vector_test_push_back_primitive();
b8 cr_std_vector_test_push_back_custom();

// Remove Element
b8 cr_std_vector_test_remove_element_primitive();
b8 cr_std_vector_test_remove_element_custom();
b8 cr_std_vector_test_remove_element_invalid_vector();
b8 cr_std_vector_test_remove_element_invalid_index();
b8 cr_std_vector_test_remove_element_invalid_index_negative();
b8 cr_std_vector_test_remove_element_empty_vector();

// Get Element
b8 cr_std_vector_test_get_element_primitive();
b8 cr_std_vector_test_get_element_custom();
b8 cr_std_vector_test_get_element_invalid_vector();
b8 cr_std_vector_test_get_element_invalid_index();
b8 cr_std_vector_test_get_element_invalid_index_negative();
b8 cr_std_vector_test_get_element_empty_vector();

// Extend Vector
b8 cr_std_vector_test_extend_primitive();
b8 cr_std_vector_test_extend_custom();

#endif // CR_STD_VECTOR_TESTS_H
