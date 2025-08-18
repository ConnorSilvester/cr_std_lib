#ifndef CR_STD_STRING
#define CR_STD_STRING

#include <stdio.h>

typedef struct Vector Vector;
typedef struct String {
    char *c_str;
    size_t length;
} String;

typedef struct StringBuilder {
    char *c_str;
    size_t size;
    size_t capacity;
} StringBuilder;

#define CR_STD_STRING_TRIM_LEFT -1
#define CR_STD_STRING_TRIM_BOTH 0
#define CR_STD_STRING_TRIM_RIGHT 1
#define CR_STD_STRING_BUILDER_DEFAULT_CAP 256

#define CR_STD_STRING_COLOR_BLACK 30
#define CR_STD_STRING_COLOR_RED 31
#define CR_STD_STRING_COLOR_GREEN 32
#define CR_STD_STRING_COLOR_YELLOW 33
#define CR_STD_STRING_COLOR_BLUE 34
#define CR_STD_STRING_COLOR_MAGENTA 35
#define CR_STD_STRING_COLOR_CYAN 36
#define CR_STD_STRING_COLOR_WHITE 37

/**
 * @brief Creates a new `StringBuilder` struct with the provided string.
 *
 * @param `string` The initial string.
 *
 * @return A pointer to the new `StringBuilder` struct.
 * @return `NULL` if allocation fails.
 */
StringBuilder *cr_std_string_builder_new(const char *string);

/**
 * @brief Creates a new `StringBuilder` struct formatted with the provided format string and arguments if any.
 *
 * @param `format` The format string used to format like `printf` (`%s`, `%d`, etc.).
 * @param `...` The arguments to format according to the format string.
 *
 * @return A pointer to the new `StringBuilder` struct.
 * @return `NULL` if allocation fails.
 */
StringBuilder *cr_std_string_builder_newf(const char *format, ...);

/**
 * @brief Adds a single string to the string builder.
 *
 * @param `string_builder` The `StringBuilder` struct to work on.
 * @param `string` The string to add.
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_string_builder_append_single(StringBuilder *string_builder, const char *string);

/**
 * @brief Adds a string to the string builder.
 *
 * @param `string_builder` The `StringBuilder` struct to work on.
 * @param `format` The format string used to format like `printf` (`%s`, `%d`, etc.).
 *                 Can also be a single string to add, e.g. "hello".
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_string_builder_appendf(StringBuilder *string_builder, const char *format, ...);

/**
 * @brief Adds strings to the string builder.
 *
 * @param `string_builder` A pointer to the `StringBuilder` struct to be updated.
 * @param `...` The strings to append.
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_string_builder_append_null_terminated(StringBuilder *string_builder, ...);

/**
 * @brief Macro to avoid having to add `NULL` to the end of arguments when calling the concat function.
 *
 * @param `string_builder` A pointer to the `StringBuilder` struct to be updated.
 * @param `...` The strings to append.
 */
#define cr_std_string_builder_append(string_builder, ...) cr_std_string_builder_append_null_terminated(string_builder, __VA_ARGS__, NULL)

/**
 * @brief Resets a string builders string, back to nothing.
 *
 * @param `string_builder` The `StringBuilder` struct to work on.
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_string_builder_reset(StringBuilder *string_builder);

/**
 * @brief Resets a string builders string, back to nothing.
 *
 * @param `string_builder` The `StringBuilder` struct to work on.
 *
 * @return A pointer to a new `String` struct.
 * @return `NULL` on failure.
 */
String *cr_std_string_builder_to_string(StringBuilder *string_builder);

/**
 * @brief Free a `StringBuilder` struct, sets pointer to `NULL`.
 *
 * @param `sb_ptr` A pointer to a pointer containing a `StringBuilder` struct.
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_string_builder_free(StringBuilder **sb_ptr);
#define cr_std_string_builder_free_ptr ((int (*)(void **))cr_std_string_builder_free)

/**
 * @brief Creates a new `String` struct with the string provided.
 *
 * @param `string` The initial string
 *
 * @return A pointer to the new `String` struct.
 * @return `NULL` if allocation fails.
 */
String *cr_std_string_new(const char *string);

/**
 * @brief Creates a new `String` struct formatted with the provided format string and arguments if any.
 *
 * @param `format` The format string used to format like `printf` (`%s`, `%d`, etc.).
 * @param `...` The arguments to format according to the format string.
 *
 * @return A pointer to the new `String` struct.
 * @return `NULL` if allocation fails.
 */
String *cr_std_string_newf(const char *format, ...);

/**
 * @brief Free a `String` struct, sets pointer to `NULL`.
 *
 * @param `string_ptr` A pointer to a pointer containing a `String` struct.
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_string_free(String **string_ptr);
#define cr_std_string_free_ptr ((int (*)(void **))cr_std_string_free)

/**
 * @brief Copy the contents of a `String` struct to another one.
 *
 * @param `src_string` A pointer to the source `String` struct.
 *
 * @return A pointer to the new `String` struct.
 * @return `NULL` if copy fails.
 */
String *cr_std_string_make_copy(String *src_string);
#define cr_std_string_make_copy_ptr ((void *(*)(void *))cr_std_string_make_copy)

/**
 * @brief Concatenates multiple strings and stores the result in an existing `String` struct.
 *
 * @param `string` A pointer to the `String` struct to be updated.
 * @param `...` The strings to concatenate, terminated with a `NULL`.
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_string_concat_null_terminated(String *string, ...);

/**
 * @brief Macro to avoid having to add `NULL` to the end of arguments when calling the concat function.
 *
 * @param `string` A pointer to the `String` struct to be updated.
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
int cr_std_string_compare(String *arg, String *arg1);

/**
 * @brief Compares String and a c str.
 *
 * @param `arg` The first string to compare.
 * @param `arg1` The second raw string to compare.
 *
 * @return `1` if full equality (same characters in the same order).
 * @return `0` if equal in length but different characters.
 * @return `-1` if the first string is longer.
 * @return `-2` if the second string is longer.
 */
int cr_std_string_compare_c_str(String *arg, char *arg1);

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
 * @return `0` if the string is trimmed successfully.
 * @return `1` if the string failed to be trimmed.
 */
int cr_std_string_trim(String *string, int direction);

/**
 * @brief Finds the first index of a character in a given string.
 *
 * @param `string` The string to search through.
 * @param `ch` The character to locate.
 *
 * @return The index of the character in the string.
 * @return `-1` if the character is not found.
 */
int cr_std_string_find_char(String *string, char ch);

/**
 * @brief Finds the first index of a string in a given string.
 *
 * @param `string` The string to search through.
 * @param `phrase` The string to locate.
 *
 * @return The index of the phrase in the string.
 * @return `-1` if the phrase is not found.
 */
int cr_std_string_find_string(String *string, const char *phrase);

/**
 * @brief Finds the n index of a string in a given string.
 *
 * @param `string` The string to search through.
 * @param `phrase` The string to locate.
 *
 * @return The index of the phrase in the string.
 * @return `-1` if the phrase is not found.
 */
int cr_std_string_find_string_n(String *string, const char *phrase, int n);

/**
 * @brief Finds the last index of a string in a given string.
 *
 * @param `string` The string to search through.
 * @param `phrase` The string to locate.
 *
 * @return The index of the phrase in the string.
 * @return `-1` if the phrase is not found.
 */
int cr_std_string_find_string_last(String *string, const char *phrase);

/**
 * @brief Finds the last index of a character in a given string.
 *
 * @param `string` The string to search through.
 * @param `ch` The character to locate.
 *
 * @return The index of the character in the string.
 * @return `-1` if the character is not found.
 */
int cr_std_string_find_char_last(String *string, char ch);

/**
 * @brief Check if a string contains a phrase.
 *
 * @param `string` The string to search through.
 * @param `phrase` The phrase / string to locate.
 *
 * @return `number of occurrences` if the string contains the phrase
 * @return `0` if the string does not contain the phrase
 */
int cr_std_string_contains_string(String *string, const char *phrase);

/**
 * @brief Check if a string contains a char.
 *
 * @param `string` The string to search through.
 * @param `ch` The char to locate.
 *
 * @return `number of occurrences` if the string contains the char.
 * @return `0` if the string does not contain the char.
 */
int cr_std_string_contains_char(String *string, char ch);

/**
 * @brief Calculate the hash code of a string.
 *
 * @param `string` The string to calculate the hash code for.
 *
 * @return `hash` code of the string.
 * @return `-1` if error.
 */
unsigned long cr_std_string_hash_code(String *string);

/**
 * @brief Splits a string into a `Vector`.
 * @brief Removes white space, use cr_std_string_split_hard if you require white space.
 *
 * @param `string` The string to split.
 * @param `delimiter` The character to split the string by.
 *
 * @return A pointer to a new `Vector` of the split strings.
 * @return `NULL` if error.
 */
Vector *cr_std_string_split(String *string, char delimiter);

/**
 * @brief Splits a string into a `Vector` (includes white space / empty strings).
 * @brief Includes any white space and includes empty string.
 *
 * @param `string` The string to split.
 * @param `delimiter` The character to split the string by.
 *
 * @return A pointer to a new `Vector` of the split strings.
 * @return `NULL` if error.
 */
Vector *cr_std_string_split_hard(String *string, char delimiter);

/**
 * @Convert a string to upper case.
 *
 * @param `string` The string to work on.
 *
 * @return `0` if the string is changed.
 * @return `1` if error.
 */
int cr_std_string_to_upper(String *string);

/**
 * @Convert a string to lower case.
 *
 * @param `string` The string to work on.
 *
 * @return `0` if the string is changed.
 * @return `1` if error.
 */
int cr_std_string_to_lower(String *string);

/**
 * @Convert a string to title case.
 *
 * @param `string` The string to work on.
 *
 * @return `0` if the string is changed.
 * @return `1` if error.
 */
int cr_std_string_to_title(String *string);

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
int cr_std_string_replace_string(String *string, const char *from, const char *to);

/**
 * @brief Removes all characters that are not numbers.
 *
 * @param `string` The `String` to work on.
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_string_remove_non_numeric(String *string);

/**
 * @brief Removes all characters that are considered numbers.
 *
 * @param `string` The `String` to work on.
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_string_remove_numeric(String *string);

/**
 * @brief Returns the numerical representation of a string
 *
 * @param `string` The `String` to work on.
 *
 * @return `int` The numerical representation of the string
 */
int cr_std_string_to_int(String *string);

/**
 * @brief Returns the string representation of a number.
 *
 * @param `number` The number to convert.
 *
 * @return `String` A pointer to a `String` struct representing the number.
 */
String *cr_std_string_from_int(int number);

/**
 * @brief Returns a sub-string of a given string and index.
 *
 * @param `string` The original string.
 * @param `start_index` The index to start the sub-string.
 * @param `end_index` The index to end the sub-string.
 *
 * @return `String` A pointer to a `String` struct representing the sub-string.
 */
String *cr_std_string_sub_string(String *string, int start_index, int end_index);

/**
 * @brief Returns the string representation of a `Vector` struct.
 *
 * @param `vector` The vector you want to join together, that contains pointers to `String` structs.
 * @param `delimiter` A string / phrase to separate the elements in the vector with.
 *
 * @return `String` A pointer to a `String` struct the joined vector.
 */
String *cr_std_string_from_string_ptr_vector(Vector *vector, const char *delimiter);

/**
 * @brief Returns the string representation of a `Vector` struct.
 *
 * @param `vector` The vector you want to join together, that contains char pointers.
 * @param `delimiter` A string / phrase to separate the elements in the vector with.
 *
 * @return `String` A pointer to a `String` struct the joined vector.
 */
String *cr_std_string_from_char_ptr_vector(Vector *vector, const char *delimiter);

/**
 * @brief Colors an entire string to one color
 *
 * @param `string` The `String` to work from.
 * @param `color_code` The color code from ANSI escape codes, see `CR_STD_COLOR`.
 *
 * @return `String` A pointer to the new colored string.
 * @return `NULL` If something failed.
 */
String *cr_std_string_color_string(String *string, int color_code);

/**
 * @brief Colors a specific phrase within a string
 *
 * @param `string` The `String` to work from.
 * @param `phrase` The phrase to color
 * @param `color_code` The color code from ANSI escape codes, see `CR_STD_COLOR`.
 *
 * @return `String` A pointer to the new colored string.
 * @return `NULL` If something failed.
 */
String *cr_std_string_color_phrase(String *string, const char *phrase, int color_code);
#endif // CR_STD_STRING
