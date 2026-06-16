#include "cr_std_arena.h"
#include "cr_std_logger.h"
#include <stdlib.h>

Arena *cr_std_arena_new(size_t capacity) {
    Arena *arena = malloc(sizeof(Arena));
    if (!arena) {
        CR_LOG_ERROR("cr_std_arena_new -> malloc call for arena failed");
        return NULL;
    }

    if (capacity <= 0) {
        capacity = CR_STD_ARENA_DEFAULT_CAPACITY;
        CR_LOG_WARNING_FMT("cr_std_arena_new -> capacity <= 0, using default %zu bytes",
                           CR_STD_ARENA_DEFAULT_CAPACITY);
    }

    arena->memory = malloc(capacity);
    if (!arena->memory) {
        free(arena);
        CR_LOG_ERROR("cr_std_arena_new -> malloc call for memory failed");
        return NULL;
    }

    arena->capacity = capacity;
    arena->used = 0;
    return arena;
}

int cr_std_arena_init(Arena *arena, void *memory, size_t capacity) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_arena_init -> arena* is NULL");
        return 1;
    }

    if (!memory) {
        CR_LOG_ERROR("cr_std_arena_init -> memory* is NULL");
        return 1;
    }

    arena->memory = (unsigned char *)memory;
    arena->capacity = capacity;
    arena->used = 0;
    return 0;
}

void *cr_std_arena_alloc(Arena *arena, size_t size) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_arena_alloc -> arena* is NULL");
        return NULL;
    }

    if (size == 0) {
        CR_LOG_WARNING("cr_std_arena_alloc -> requested 0 bytes");
        return NULL;
    }

    size_t aligned = (size + 7) & ~7;

    if (arena->used + aligned > arena->capacity) {
        CR_LOG_ERROR_FMT("cr_std_arena_alloc -> memory overflow, requested %zu : available %zu",
                         size, cr_std_arena_remaining(arena));
        return NULL; // Overflow
    }

    void *ptr = arena->memory + arena->used;
    arena->used += aligned;
    return ptr;
}

int cr_std_arena_reset(Arena *arena) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_arena_reset -> arena* is NULL");
        return 1;
    }
    arena->used = 0;
    return 0;
}

int cr_std_arena_free(Arena **arena_ptr) {
    if (arena_ptr && *arena_ptr) {
        if ((*arena_ptr)->memory) {
            free((*arena_ptr)->memory);
            (*arena_ptr)->memory = NULL;
        }

        free(*arena_ptr);
        *arena_ptr = NULL;
        return 0;
    }

    CR_LOG_ERROR("cr_std_arena_free -> tried to free a NULL Arena*");
    return 1;
}

size_t cr_std_arena_remaining(Arena *arena) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_arena_remaining -> arena* is NULL");
        return 0;
    }

    return arena->capacity - arena->used;
}
