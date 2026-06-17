#include "cr_std_arena.h"
#include "cr_std_cli.h"
#include "cr_std_csv.h"
#include "cr_std_filesystem.h"
#include "cr_std_logger.h"
#include "cr_std_string.h"
#include "cr_std_testing.h"
#include "cr_std_utils.h"
#include "cr_std_vector.h"
#include <stdio.h>
#include <stdlib.h>

// If you want to test the library see the end of the README.md file
int main(int argc, char **argv) {
    // Heap
    Arena *arena = cr_std_arena_new(1 * CR_STD_KB);

    // Stack
    Arena stack_arena;
    unsigned char stack_arena_memory[1 * CR_STD_KB];
    cr_std_arena_init(&stack_arena, &stack_arena_memory, sizeof(stack_arena_memory));

    // Heap
    String *string = cr_std_string_newf(arena, "Hello, %s", "World");
    cr_std_string_concat(arena, string, ", This", " Has", " Been", " Concatenated");

    // Stack
    String *stack_string = cr_std_string_new(&stack_arena, "Hello, World");
    printf("%s\n", string->c_str);
    printf("%s\n", stack_string->c_str);

    // Do not call free for the stack_arena
    cr_std_arena_free(&arena);
    return 0;
}
