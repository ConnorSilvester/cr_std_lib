#include "cr_std_arena.h"
#include "cr_std_logger.h"
#include <stdlib.h>

Arena *cr_std_arena_new(size_t capacity) {
    Arena *arena = malloc(sizeof(*arena));
    if (!arena) {
        CR_LOG_ERROR("cr_std_arena_new -> malloc call for arena failed");
        return NULL;
    }

    if (capacity <= 0) {
        capacity = CR_STD_ARENA_DEFAULT_CAPACITY;
        CR_LOG_WARNING_FMT("cr_std_arena_new -> capacity <= 0, using default %zu bytes",
                           CR_STD_ARENA_DEFAULT_CAPACITY);
    }

    // Overflow
    if (capacity > SIZE_MAX / 2) {
        CR_LOG_ERROR_FMT("cr_std_arena_new -> capacity too large: %zu", capacity);
        capacity = CR_STD_ARENA_DEFAULT_CAPACITY;
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

b8 cr_std_arena_init(Arena *arena, void *memory, size_t capacity) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_arena_init -> arena* is NULL");
        return CR_STD_FAIL;
    }

    if (!memory) {
        CR_LOG_ERROR("cr_std_arena_init -> memory* is NULL");
        return CR_STD_FAIL;
    }

    // Overflow
    if (capacity > SIZE_MAX / 2) {
        CR_LOG_ERROR_FMT("cr_std_arena_init -> capacity too large: %zu", capacity);
        return CR_STD_FAIL;
    }

    arena->memory = (unsigned char *)memory;
    arena->capacity = capacity;
    arena->used = 0;
    return CR_STD_OK;
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

    if (size > SIZE_MAX / 2) {
        CR_LOG_ERROR_FMT("cr_std_arena_alloc -> size too large: %zu", size);
        return NULL;
    }

    size_t aligned = (size + 7) & ~7;

    if (arena->used + aligned > arena->capacity) {
        size_t remaining;
        cr_std_arena_remaining(arena, &remaining);
        CR_LOG_ERROR_FMT("cr_std_arena_alloc -> memory overflow, requested %zu : available %zu",
                         size, remaining);
        return NULL; // Overflow
    }

    void *ptr = arena->memory + arena->used;
    arena->used += aligned;
    return ptr;
}

b8 cr_std_arena_reset(Arena *arena) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_arena_reset -> arena* is NULL");
        return CR_STD_FAIL;
    }
    arena->used = 0;
    return CR_STD_OK;
}

b8 cr_std_arena_reset_to_mark(Arena *arena, size_t mark) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_arena_reset_to_mark -> arena is NULL");
        return CR_STD_FAIL;
    }

    if (mark > arena->used) {
        CR_LOG_ERROR_FMT("cr_std_arena_reset_to_mark -> mark %zu > current used %zu", mark,
                         arena->used);
        return CR_STD_FAIL;
    }

    arena->used = mark;
    return CR_STD_OK;
}

b8 cr_std_arena_free(Arena **arena_ptr) {
    if (arena_ptr && *arena_ptr) {
        if ((*arena_ptr)->memory) {
            free((*arena_ptr)->memory);
            (*arena_ptr)->memory = NULL;
        }

        free(*arena_ptr);
        *arena_ptr = NULL;
        return CR_STD_OK;
    }

    CR_LOG_ERROR("cr_std_arena_free -> tried to free a NULL Arena*");
    return CR_STD_FAIL;
}

b8 cr_std_arena_remaining(Arena *arena, size_t *remaining) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_arena_remaining -> arena* is NULL");
        return CR_STD_FAIL;
    }

    if (!remaining) {
        CR_LOG_ERROR("cr_std_arena_remaining -> remaining* is NULL");
        return CR_STD_FAIL;
    }

    *remaining = arena->capacity - arena->used;
    return CR_STD_OK;
}

b8 cr_std_arena_get_mark(Arena *arena, size_t *mark) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_arena_get_mark -> arena is NULL");
        return CR_STD_FAIL;
    }

    if (!mark) {
        CR_LOG_ERROR("cr_std_arena_get_mark -> mark is NULL");
        return CR_STD_FAIL;
    }

    *mark = arena->used;
    return CR_STD_OK;
}
