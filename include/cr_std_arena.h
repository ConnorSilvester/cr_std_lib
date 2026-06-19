#ifndef CR_STD_ARENA
#define CR_STD_ARENA

#include "cr_std_utils.h"
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Arena {
    unsigned char *memory;
    size_t capacity;
    size_t used;
} Arena;

#define CR_STD_ARENA_DEFAULT_CAPACITY (1 * CR_STD_MB)

/**
 * @brief Allocates a new Arena struct on the heap
 *
 * @param `capacity` number of bytes to allocate
 *
 * @return `Arena *` on success.
 * @return `NULL` on error.
 */
Arena *cr_std_arena_new(size_t capacity);

/**
 * @brief Used to init an Arena struct using a user specified memory location.
 *
 * @param `arena` A pointer to the Arena struct
 * @param `memory` A pointer to the user specified memory location
 * @param `capacity` number of bytes the user has allocated allocated
 *
 * @return `CR_STD_OK` on success.
 * @return `CR_STD_FAIL` on error.
 */
b8 cr_std_arena_init(Arena *arena, void *memory, size_t capacity);

/**
 * @brief Used to allocate memory to a given arena
 *
 * @param `arena` A pointer to the Arena struct
 * @param `size` number of bytes to allocate
 *
 * @return `void *` to the memory location on success.
 * @return `NULL` on error.
 */
void *cr_std_arena_alloc(Arena *arena, size_t size);

/**
 * @brief Used to reset a Arena struct, does not free memory
 *
 * @param `arena` A pointer to the Arena struct
 *
 * @return `CR_STD_OK` on success.
 * @return `CR_STD_FAIL` on error.
 */
b8 cr_std_arena_reset(Arena *arena);

/**
 * @brief Used to reset a Arena struct, does not free memory
 *
 * @param `arena` A pointer to the Arena struct
 * @param `mark` The pos to reset too
 *
 * @return `CR_STD_OK` on success.
 * @return `CR_STD_FAIL` on error.
 */
b8 cr_std_arena_reset_to_mark(Arena *arena, size_t mark);

/**
 * @brief Used to free an Arena struct
 *
 * @param `arena` A pointer to the Arena struct
 *
 * @warning Do `NOT` call this on arenas initialized with `cr_std_arena_init()`
 *          Stack/static arenas are freed automatically when they go out of scope
 *
 * @return `CR_STD_OK` on success.
 * @return `CR_STD_FAIL` on error.
 */
b8 cr_std_arena_free(Arena **arena_ptr);

/**
 * @brief Used to query the remaining bytes of the Arena struct
 *
 * @param `arena` A pointer to the Arena struct
 * @param `remaining` Output parameter for remaining bytes
 *
 * @return `CR_STD_OK` on success.
 * @return `CR_STD_FAIL` on error.
 */
b8 cr_std_arena_remaining(Arena *arena, size_t *remaining);

/**
 * @brief Used to query the current mark in the arena
 *
 * @param `arena` A pointer to the Arena struct
 * @param `mark` Output parameter for the mark position
 *
 * @return `CR_STD_OK` on success.
 * @return `CR_STD_FAIL` on error.
 */
b8 cr_std_arena_get_mark(Arena *arena, size_t *mark);

#ifdef __cplusplus
}
#endif
#endif // CR_STD_ARENA
