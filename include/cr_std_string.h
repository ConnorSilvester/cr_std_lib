#ifndef CR_STD_STRING
#define CR_STD_STRING

#include <stdio.h>

typedef struct vector_t vector_t;
typedef struct string_t {
    char *c_str;
    size_t length;
} string_t;

typedef struct string_builder_t {
    char *c_str;
    size_t size;
    size_t capacity;
} string_builder_t;

#define CR_STD_STRING_TRIM_LEFT -1
#define CR_STD_STRING_TRIM_BOTH 0
#define CR_STD_STRING_TRIM_RIGHT 1
#define CR_STD_STRING_BUILDER_DEFAULT_CAP 256

/**
 * @brief Creates a new `string_builder_t` struct with the provided string.
 *
 * @param `string` The initial string.
 *
 * @return A pointer to the new `string_builder_t` struct.
 * @return `NULL` if allocation fails.
 */
string_builder_t *cr_std_string_builder_new(const char *string);

/**
 * @brief Creates a new `string_builder_t` struct formatted with the provided format string and arguments if any.
 *
 * @param `format` The format string used to format like `printf` (`%s`, `%d`, etc.).
 * @param `...` The arguments to format according to the format string.
 *
 * @return A pointer to the new `string_builder_t` struct.
 * @return `NULL` if allocation fails.
 */
string_builder_t *cr_std_string_builder_newf(const char *format, ...);

/**
 * @brief Adds a single string to the string builder.
 *
 * @param `string_builder` The `string_builder_t` struct to work on.
 * @param `string` The string to add.
 *
 * @return `1` on success.
 * @return `0` on failure.
 */
int cr_std_string_builder_append_single(string_builder_t *string_builder, const char *string);

/**
 * @brief Adds a string to the string builder.
 *
 * @param `string_builder` The `string_builder_t` struct to work on.
 * @param `format` The format string used to format like `printf` (`%s`, `%d`, etc.).
 *                 Can also be a single string to add, e.g. "hello".
 *
 * @return `1` on success.
 * @return `0` on failure.
 */
int cr_std_string_builder_appendf(string_builder_t *string_builder, const char *format, ...);

/**
 * @brief Adds strings to the string builder.
 *
 * @param `string_builder` A pointer to the `string_builder_t` struct to be updated.
 * @param `...` The strings to append.
 *
 * @return `1` on success.
 * @return `0` on failure.
 */
int cr_std_string_builder_append_null_terminated(string_builder_t *string_builder, ...);

/**
 * @brief Macro to avoid having to add `NULL` to the end of arguments when calling the concat function.
 *
 * @param `string_builder` A pointer to the `string_builder_t` struct to be updated.
 * @param `...` The strings to append.
 */
#define cr_std_string_builder_append(string_builder, ...) cr_std_string_builder_append_null_terminated(string_builder, __VA_ARGS__, NULL)

/**
 * @brief Resets a string builders string, back to nothing.
 *
 * @param `string_builder` The `string_builder_t` struct to work on.
 *
 * @return `1` on success.
 * @return `0` on failure.
 */
int cr_std_string_builder_reset(string_builder_t *string_builder);

/**
 * @brief Resets a string builders string, back to nothing.
 *
 * @param `string_builder` The `string_builder_t` struct to work on.
 *
 * @return A pointer to a new `string_t` struct.
 * @return `NULL` on failure.
 */
string_t *cr_std_string_builder_to_string(string_builder_t *string_builder);

/**
 * @brief Free a `string_builder_t` struct, sets pointer to `NULL`.
 *
 * @param `sb_ptr` A pointer to a pointer containing a `string_builder_t` struct.
 *
 * @return `1` on success.
 * @return `0` on failure.
 */
int cr_std_string_builder_free(string_builder_t **sb_ptr);
#define cr_std_string_builder_free_ptr ((int (*)(void **))cr_std_string_builder_free)

/**
 * @brief Creates a new `string_t` struct with the string provided.
 *
 * @param `string` The initial string
 *
 * @return A pointer to the new `string_t` struct.
 * @return `NULL` if allocation fails.
 */
string_t *cr_std_string_new(const char *string);

/**
 * @brief Creates a new `string_t` struct formatted with the provided format string and arguments if any.
 *
 * @param `format` The format string used to format like `printf` (`%s`, `%d`, etc.).
 * @param `...` The arguments to format according to the format string.
 *
 * @return A pointer to the new `string_t` struct.
 * @return `NULL` if allocation fails.
 */
string_t *cr_std_string_newf(const char *format, ...);

/**
 * @brief Free a `string_t` struct, sets pointer to `NULL`.
 *
 * @param `string_ptr` A pointer to a pointer containing a `string_t` struct.
 *
 * @return `1` on success.
 * @return `0` on failure.
 */
int cr_std_string_free(string_t **string_ptr);
#define cr_std_string_free_ptr ((int (*)(void **))cr_std_string_free)

/**
 * @brief Copy the contents of a `string_t` struct to another one.
 *
 * @param `src_string` A pointer to the source `string_t` struct.
 *
 * @return A pointer to the new `string_t` struct.
 * @return `NULL` if copy fails.
 */
string_t *cr_std_string_make_copy(string_t *src_string);
#define cr_std_string_make_copy_ptr ((void *(*)(void *))cr_std_string_make_copy)

/**
 * @brief Concatenates multiple strings and stores the result in an existing `string_t` struct.
 *
 * @param `string` A pointer to the `string_t` struct to be updated.
 * @param `...` The strings to concatenate, terminated with a `NULL`.
 *
 * @return `1` on success.
 * @return `0` on failure.
 */
int cr_std_string_concat_null_terminated(string_t *string, ...);

/**
 * @brief Macro to avoid having to add `NULL` to the end of arguments when calling the concat function.
 *
 * @param `string` A pointer to the `string_t` struct to be updated.
 * @param `...` The strings to concatenate.
 */
#define cr_std_string_concat(string, ...) cr_std_string_concat_null_terminated(string, __VA_ARGS__, NULL)

/**
 * @brief Compares two strings.
 *
 * @param `arg` The first string to compare.
 * @param `arg1` The second string to compare.
 *
 * @return `1` if full equality (same characters in the same order).
 * @return `0` if equal in length but different characters.
 * @return `-1` if the first string is longer.
 * @return `-2` if the second string is longer.
 */
int cr_std_string_compare(string_t *arg, string_t *arg1);

/**
 * @brief Trims a string of white space in both directions of the string.
 *
 * @param `string` The string to trim.
 * @param `direction` Which dir you want to cut or both
 *
 * Defined as `CR_STD_STRING_TRIM_LEFT`
 *
 * Defined as `CR_STD_STRING_TRIM_RIGHT`
 *
 * Defined as `CR_STD_STRING_TRIM_BOTH`
 *
 * @return `1` if the string is trimmed successfully.
 * @return `0` if the string failed to be trimmed.
 */
int cr_std_string_trim(string_t *string, int direction);

/**
 * @brief Finds the first index of a character in a given string.
 *
 * @param `string` The string to search through.
 * @param `ch` The character to locate.
 *
 * @return The index of the character in the string.
 * @return `-1` if the character is not found.
 */
int cr_std_string_find_char(string_t *string, char ch);

/**
 * @brief Finds the last index of a character in a given string.
 *
 * @param `string` The string to search through.
 * @param `ch` The character to locate.
 *
 * @return The index of the character in the string.
 * @return `-1` if the character is not found.
 */
int cr_std_string_find_char_last(string_t *string, char ch);

/**
 * @brief Check if a string contains a phrase.
 *
 * @param `string` The string to search through.
 * @param `phrase` The phrase / string to locate.
 *
 * @return `number of occurrences` if the string contains the phrase
 * @return `0` if the string does not contain the phrase
 */
int cr_std_string_contains_string(string_t *string, char *phrase);

/**
 * @brief Check if a string contains a char.
 *
 * @param `string` The string to search through.
 * @param `ch` The char to locate.
 *
 * @return `number of occurrences` if the string contains the char.
 * @return `0` if the string does not contain the char.
 */
int cr_std_string_contains_char(string_t *string, char ch);

/**
 * @brief Calculate the hash code of a string.
 *
 * @param `string` The string to calculate the hash code for.
 *
 * @return `hash` code of the string.
 * @return `-1` if error.
 */
unsigned long cr_std_string_hash_code(string_t *string);

/**
 * @brief Splits a string into a `vector_t`.
 *
 * @param `string` The string to split.
 * @param `delimiter` The character to split the string by.
 *
 * @return A pointer to a new `vector_t` of the split strings.
 * @return `NULL` if error.
 */
vector_t *cr_std_string_split(string_t *string, char delimiter);

/**
 * @Convert a string to upper case.
 *
 * @param `string` The string to work on.
 *
 * @return `1` if the string is changed.
 * @return `0` if error.
 */
int cr_std_string_to_upper(string_t *string);

/**
 * @Convert a string to lower case.
 *
 * @param `string` The string to work on.
 *
 * @return `1` if the string is changed.
 * @return `0` if error.
 */
int cr_std_string_to_lower(string_t *string);

/**
 * @brief Replaces all occurrences of `from` with `to` in a given string.
 *
 * @param `string` The string to work on.
 * @param `from` The string you want to replace.
 * @param `to` The string you want to replace `from` with.
 *
 * @return `number of replaced words`.
 * @return `0` if nothing is replaced.
 */
int cr_std_string_replace_string(string_t *string, char *from, char *to);

/**
 * @brief Removes all characters that are not numbers.
 *
 * @param `string` The `string_t` to work on.
 *
 * @return `1` on success.
 * @return `0` on failure.
 */
int cr_std_string_remove_non_numeric(string_t *string);

/**
 * @brief Removes all characters that are considered numbers.
 *
 * @param `string` The `string_t` to work on.
 *
 * @return `1` on success.
 * @return `0` on failure.
 */
int cr_std_string_remove_numeric(string_t *string);

/**
 * @brief Returns the numerical representation of a string
 *
 * @param `string` The `string_t` to work on.
 *
 * @return `long int` The numerical representation of the string
 */
long int cr_std_string_to_int(string_t *string);

/**
 * @brief Returns the string representation of a number.
 *
 * @param `number` The number to convert.
 *
 * @return `string_t` A pointer to a `string_t` struct representing the number.
 */
string_t *cr_std_string_from_int(int number);


/**
 * @brief Returns a sub-string of a given string and index.
 *
 * @param `string` The original string.
 * @param `start_index` The index to start the sub-string.
 * @param `end_index` The index to end the sub-string.
 *
 * @return `string_t` A pointer to a `string_t` struct representing the sub-string.
 */
string_t *cr_std_string_sub_string(string_t* string, int start_index, int end_index);

/**
 * @brief Returns the string representation of a `vector_t` struct.
 *
 * @param `vector` The vector you want to join together, that contains pointers to `string_t` structs.
 * @param `delimiter` A string / phrase to separate the elements in the vector with.
 *
 * @return `string_t` A pointer to a `string_t` struct the joined vector.
 */
string_t *cr_std_string_from_string_ptr_vector(vector_t *vector, const char *delimiter);

/**
 * @brief Returns the string representation of a `vector_t` struct.
 *
 * @param `vector` The vector you want to join together, that contains char pointers.
 * @param `delimiter` A string / phrase to separate the elements in the vector with.
 *
 * @return `string_t` A pointer to a `string_t` struct the joined vector.
 */
string_t *cr_std_string_from_char_ptr_vector(vector_t *vector, const char *delimiter);
#endif // CR_STD_STRING
