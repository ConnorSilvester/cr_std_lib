#include "cr_std_arena_tests.h"
#include "cr_std_arena.h"
#include "cr_std_string.h"
#include "cr_std_testing.h"
#include "cr_std_utils.h"
#include "cr_std_vector.h"
#include <stdio.h>
#include <stdlib.h>

Arena *g_arena_testing_arena = NULL;

void cr_std_arena_test_all() {
    printf("%s\n", "Running Arena Tests:");

    g_arena_testing_arena = cr_std_arena_new(CR_STD_MB);
    Vector *tests = cr_std_vector_new(g_arena_testing_arena);

    // clang-format off
    // New Arena
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Make Arena -> Normal", cr_std_arena_test_new_arena_normal));
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Make Arena -> 0", cr_std_arena_test_new_arena_0));
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Make Arena -> Negative", cr_std_arena_test_new_arena_negative));

    // Init Arena
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Init Arena -> Normal", cr_std_arena_test_init_normal));
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Init Arena -> Null Arena", cr_std_arena_test_init_null_arena));
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Init Arena -> Null Memory", cr_std_arena_test_init_null_memory));
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Init Arena -> Used Check", cr_std_arena_test_init_used_check));
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Init Arena -> Used Check Allignmnet", cr_std_arena_test_init_used_check_allignment));

    // Arena Alloc
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Arena Alloc -> Normal", cr_std_arena_test_arena_alloc_normal));
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Arena Alloc -> 0", cr_std_arena_test_arena_alloc_0));
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Arena Alloc -> Null", cr_std_arena_test_arena_alloc_null));
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Arena Alloc -> Negative", cr_std_arena_test_arena_alloc_negative));

    // Arena Reset
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Arena Reset -> Normal", cr_std_arena_test_arena_reset_normal));
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Arena Reset -> Null", cr_std_arena_test_arena_reset_null));

    // Arena Reset To Mark
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Arena Reset To Mark -> Normal", cr_std_arena_test_arena_reset_to_mark_normal));
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Arena Reset To Mark -> Large", cr_std_arena_test_arena_reset_to_mark_large));
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Arena Reset To Mark -> Negative", cr_std_arena_test_arena_reset_to_mark_negative));
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Arena Reset To Mark -> Null", cr_std_arena_test_arena_reset_to_mark_null));

    // Free Arena
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Free Arena -> Normal", cr_std_arena_test_free_arena_normal));
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Free Arena -> Null", cr_std_arena_test_free_arena_null));

    // Remaining
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Remaining -> Normal", cr_std_arena_test_remaining_normal));
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Remaining -> Alloc", cr_std_arena_test_remaining_normal_with_alloc));
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Remaining -> Null Arena", cr_std_arena_test_remaining_null_arena));
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Remaining -> Null Remaining", cr_std_arena_test_remaining_null_remaining));

    // Get Mark
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Get Mark -> Normal", cr_std_arena_test_get_mark_normal));
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Get Mark -> Null Arena", cr_std_arena_test_get_mark_null_arena));
    cr_std_vector_push_back(g_arena_testing_arena, tests, cr_std_testing_new_test(g_arena_testing_arena, "Get Mark -> Null Mark", cr_std_arena_test_get_mark_null_mark));

    // clang-format on
    cr_std_testing_run_tests(g_arena_testing_arena, tests);
    cr_std_arena_free(&g_arena_testing_arena);
}

b8 cr_std_arena_test_new_arena_normal() {
    Arena *arena = cr_std_arena_new(CR_STD_KB);
    if (!arena) {
        return false;
    }
    b8 result = arena->capacity == CR_STD_KB;
    cr_std_arena_free(&arena);
    return result;
}

b8 cr_std_arena_test_new_arena_0() {
    Arena *arena = cr_std_arena_new(0);
    if (!arena) {
        return false;
    }
    b8 result = arena->capacity == CR_STD_ARENA_DEFAULT_CAPACITY;
    cr_std_arena_free(&arena);
    return result;
}

b8 cr_std_arena_test_new_arena_negative() {
    Arena *arena = cr_std_arena_new(-10);
    if (!arena) {
        return false;
    }
    b8 result = arena->capacity == CR_STD_ARENA_DEFAULT_CAPACITY;
    cr_std_arena_free(&arena);
    return result;
}

b8 cr_std_arena_test_init_normal() {
    Arena arena;
    unsigned char memory[CR_STD_KB];
    if (cr_std_arena_init(&arena, memory, sizeof(memory)) == CR_STD_OK) {
        return arena.capacity == CR_STD_KB;
    }
    return false;
}

b8 cr_std_arena_test_init_null_arena() {
    unsigned char memory[CR_STD_KB];
    if (cr_std_arena_init(NULL, memory, sizeof(memory)) != CR_STD_OK) {
        return true;
    }
    return false;
}

b8 cr_std_arena_test_init_null_memory() {
    Arena arena;
    if (cr_std_arena_init(&arena, NULL, sizeof(NULL)) != CR_STD_OK) {
        return true;
    }
    return false;
}

b8 cr_std_arena_test_init_used_check() {
    Arena arena;
    unsigned char memory[CR_STD_KB];
    if (cr_std_arena_init(&arena, memory, sizeof(memory)) == CR_STD_OK) {
        cr_std_arena_alloc(&arena, 8);
        return arena.capacity == CR_STD_KB && arena.used == 8;
    }
    return false;
}

b8 cr_std_arena_test_init_used_check_allignment() {
    Arena arena;
    unsigned char memory[CR_STD_KB];
    if (cr_std_arena_init(&arena, memory, sizeof(memory)) == CR_STD_OK) {
        cr_std_arena_alloc(&arena, 10);
        return arena.capacity == CR_STD_KB && arena.used == 16;
    }
    return false;
}

b8 cr_std_arena_test_arena_alloc_normal() {
    int *result = cr_std_arena_alloc(g_arena_testing_arena, sizeof(*result));
    if (!result) {
        return false;
    }
    *result = 42;
    return *result == 42;
}

b8 cr_std_arena_test_arena_alloc_0() {
    int *result = cr_std_arena_alloc(g_arena_testing_arena, 0);
    if (!result) {
        return true;
    }
    return false;
}

b8 cr_std_arena_test_arena_alloc_null() {
    int *result = cr_std_arena_alloc(NULL, sizeof(*result));
    if (!result) {
        return true;
    }
    return false;
}

b8 cr_std_arena_test_arena_alloc_negative() {
    int *result = cr_std_arena_alloc(g_arena_testing_arena, -2);
    if (!result) {
        return true;
    }
    return false;
}

b8 cr_std_arena_test_arena_reset_normal() {
    Arena arena;
    unsigned char memory[CR_STD_KB];
    if (cr_std_arena_init(&arena, memory, sizeof(memory)) != CR_STD_OK) {
        return false;
    }
    int *result = cr_std_arena_alloc(&arena, sizeof(*result));
    if (!result) {
        return false;
    }
    return cr_std_arena_reset(&arena) == CR_STD_OK && arena.used == 0;
}

b8 cr_std_arena_test_arena_reset_null() {
    return cr_std_arena_reset(NULL) != CR_STD_OK;
}

b8 cr_std_arena_test_arena_reset_to_mark_normal() {
    Arena arena;
    unsigned char memory[CR_STD_KB];
    if (cr_std_arena_init(&arena, memory, sizeof(memory)) != CR_STD_OK) {
        return false;
    }
    int *alloc = cr_std_arena_alloc(&arena, sizeof(*alloc));
    if (!alloc) {
        return false;
    }
    size_t mark;
    if (cr_std_arena_get_mark(&arena, &mark) != CR_STD_OK) {
        return false;
    }
    int *alloc1 = cr_std_arena_alloc(&arena, sizeof(*alloc1));
    if (!alloc1) {
        return false;
    }
    return cr_std_arena_reset_to_mark(&arena, mark) == CR_STD_OK && arena.used == mark;
}

b8 cr_std_arena_test_arena_reset_to_mark_large() {
    Arena arena;
    unsigned char memory[CR_STD_KB];
    if (cr_std_arena_init(&arena, memory, sizeof(memory)) != CR_STD_OK) {
        return false;
    }
    return cr_std_arena_reset_to_mark(&arena, 100) != CR_STD_OK;
}

b8 cr_std_arena_test_arena_reset_to_mark_negative() {
    Arena arena;
    unsigned char memory[CR_STD_KB];
    if (cr_std_arena_init(&arena, memory, sizeof(memory)) != CR_STD_OK) {
        return false;
    }
    return cr_std_arena_reset_to_mark(&arena, -2) != CR_STD_OK;
}

b8 cr_std_arena_test_arena_reset_to_mark_null() {
    return cr_std_arena_reset_to_mark(NULL, 0) != CR_STD_OK;
}

b8 cr_std_arena_test_free_arena_normal() {
    Arena *arena = cr_std_arena_new(CR_STD_KB);
    if (!arena) {
        return false;
    }
    return cr_std_arena_free(&arena) == CR_STD_OK && !arena;
}

b8 cr_std_arena_test_free_arena_null() {
    return cr_std_arena_free(NULL) != CR_STD_OK;
}

b8 cr_std_arena_test_remaining_normal() {
    Arena arena;
    unsigned char memory[CR_STD_KB];
    if (cr_std_arena_init(&arena, memory, sizeof(memory)) != CR_STD_OK) {
        return false;
    }
    size_t remaining;
    return cr_std_arena_remaining(&arena, &remaining) == CR_STD_OK && remaining == CR_STD_KB;
}

b8 cr_std_arena_test_remaining_normal_with_alloc() {
    Arena arena;
    unsigned char memory[CR_STD_KB];
    if (cr_std_arena_init(&arena, memory, sizeof(memory)) != CR_STD_OK) {
        return false;
    }
    int *alloc = cr_std_arena_alloc(&arena, sizeof(*alloc));
    if (!alloc) {
        return false;
    }
    size_t remaining;
    return cr_std_arena_remaining(&arena, &remaining) == CR_STD_OK && remaining == (CR_STD_KB - 8);
}

b8 cr_std_arena_test_remaining_null_arena() {
    size_t remaining;
    return cr_std_arena_remaining(NULL, &remaining) != CR_STD_OK;
}

b8 cr_std_arena_test_remaining_null_remaining() {
    Arena arena;
    unsigned char memory[CR_STD_KB];
    if (cr_std_arena_init(&arena, memory, sizeof(memory)) != CR_STD_OK) {
        return false;
    }
    return cr_std_arena_remaining(&arena, NULL) != CR_STD_OK;
}

b8 cr_std_arena_test_get_mark_normal() {
    Arena arena;
    unsigned char memory[CR_STD_KB];
    if (cr_std_arena_init(&arena, memory, sizeof(memory)) != CR_STD_OK) {
        return false;
    }
    size_t mark;
    return cr_std_arena_get_mark(&arena, &mark) == CR_STD_OK;
}

b8 cr_std_arena_test_get_mark_null_arena() {
    size_t mark;
    return cr_std_arena_get_mark(NULL, &mark) != CR_STD_OK;
}

b8 cr_std_arena_test_get_mark_null_mark() {
    Arena arena;
    unsigned char memory[CR_STD_KB];
    if (cr_std_arena_init(&arena, memory, sizeof(memory)) != CR_STD_OK) {
        return false;
    }
    return cr_std_arena_get_mark(&arena, NULL) != CR_STD_OK;
}
