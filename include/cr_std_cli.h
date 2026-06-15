#ifndef CR_STD_CLI
#define CR_STD_CLI

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Vector Vector;
typedef struct String String;

/**
 * @brief Represents an Argument
 *
 * Memory ownership:
 * - The struct owns the flag, help_text and parameters vector.
 * - Freeing the struct using 'cr_std_cli_free_argument_definition()' will free all memory.
 */
typedef struct ArgumentDefinition {
    String *flag;       // Keyword
    String *help_text;  // Help text (displayed when the wrong parameters are provided)
    Vector *parameters; // Vector <String>
    int expected_param_count;
    int found;
    int error;
} ArgumentDefinition;

/**
 * @brief Parses the argv into the structs provided.
 *
 * @param `argument_definitions` A Vector of ArgumentDefinition to search for
 * @param `argc` int of how many argument count
 * @param `argv` char** of the arguments
 *
 * @return `0` on success.
 * @return `1` on error.
 */
int cr_std_cli_parse_args(Vector *argument_definitions, int argc, char **argv);

/**
 * @brief Creates a new `ArgumentDefinition` struct
 *
 * @return A pointer to the new `CSVFile` struct.
 * @return `NULL` if allocation fails.
 */
ArgumentDefinition *cr_std_cli_new_argument_definition(const char *flag, int expected_param_count);

/**
 * @brief Used to free a ArgumentDefinition struct.
 *
 * Frees all elements inside
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_cli_free_argument_definition(ArgumentDefinition **arg_ptr);

#ifdef __cplusplus
}
#endif
#endif // CR_STD_CLI
