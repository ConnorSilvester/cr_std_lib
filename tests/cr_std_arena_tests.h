#ifndef CR_STD_ARENA_TESTS_H
#define CR_STD_ARENA_TESTS_H
#include "cr_std_utils.h"

// Test functions for all functions in cr_std_arena
void cr_std_arena_test_all();

// New Arena
b8 cr_std_arena_test_new_arena_normal();
b8 cr_std_arena_test_new_arena_negative();
b8 cr_std_arena_test_new_arena_0();

// Init Arena
b8 cr_std_arena_test_init_normal();
b8 cr_std_arena_test_init_null_arena();
b8 cr_std_arena_test_init_null_memory();
b8 cr_std_arena_test_init_used_check();
b8 cr_std_arena_test_init_used_check_allignment();

// Arena Alloc
b8 cr_std_arena_test_arena_alloc_normal();
b8 cr_std_arena_test_arena_alloc_0();
b8 cr_std_arena_test_arena_alloc_null();
b8 cr_std_arena_test_arena_alloc_negative();

// Arena Reset
b8 cr_std_arena_test_arena_reset_normal();
b8 cr_std_arena_test_arena_reset_null();

// Arena Reset To Mark
b8 cr_std_arena_test_arena_reset_to_mark_normal();
b8 cr_std_arena_test_arena_reset_to_mark_large();
b8 cr_std_arena_test_arena_reset_to_mark_negative();
b8 cr_std_arena_test_arena_reset_to_mark_null();

// Free Arena
b8 cr_std_arena_test_free_arena_normal();
b8 cr_std_arena_test_free_arena_null();

// Remaining
b8 cr_std_arena_test_remaining_normal();
b8 cr_std_arena_test_remaining_normal_with_alloc();
b8 cr_std_arena_test_remaining_null_arena();
b8 cr_std_arena_test_remaining_null_remaining();

// Get Mark
b8 cr_std_arena_test_get_mark_normal();
b8 cr_std_arena_test_get_mark_null_arena();
b8 cr_std_arena_test_get_mark_null_mark();

#endif // CR_STD_ARENA_TESTS_H
