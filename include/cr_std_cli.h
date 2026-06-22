#ifndef CR_STD_CLI
#define CR_STD_CLI

#include "cr_std_utils.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Vector Vector;
typedef struct String String;
typedef struct Arena Arena;

/**
 * @brief Represents an Argument
 * @note `cr_std_arena` is used for memory management
 *
 */
typedef struct ArgumentDefinition {
    String *flag;       // Keyword
    String *help_text;  // Help text (displayed when the wrong parameters are provided)
    Vector *parameters; // Vector <String>
    size_t expected_param_count;
    b8 found;
    b8 error;
} ArgumentDefinition;

/**
 * @brief Creates a new `ArgumentDefinition` struct
 *
 * @param `arena` The arena to store the memory in
 * @param `flag` The flag of the argument that is expected
 * @param `expected_param_count` The number of expected parameters
 *
 * @return A pointer to the new `ArgumentDefinition` struct.
 * @return `NULL` if allocation fails.
 */
ArgumentDefinition *
cr_std_cli_new_argument_definition(Arena *arena, const char *flag, size_t expected_param_count);

/**
 * @brief Parses the argv into the structs provided.
 *
 * @param `arena` The arena to store the memory in
 * @param `argument_definitions` A Vector of ArgumentDefinition to search for
 * @param `argc` int of how many argument count
 * @param `argv` char** of the arguments
 *
 * @return `CR_STD_OK` on success.
 * @return `CR_STD_FAIL` on error.
 */
b8 cr_std_cli_parse_args(Arena *arena, Vector *argument_definitions, int argc, char **argv);

#ifdef __cplusplus
}
#endif
#endif // CR_STD_CLI
