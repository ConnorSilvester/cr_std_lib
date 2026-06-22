#ifndef CR_STD_STRING
#define CR_STD_STRING

#include "cr_std_utils.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Vector Vector;
typedef struct Arena Arena;

/**
 * @brief Represents a string.
 *
 * @note Memory is allocated from an arena passed to functions.
 *       Does NOT store the arena - user passes it when needed.
 */
typedef struct String {
    char *c_str;
    size_t length;
} String;

/**
 * @brief A dynamic growing string object, it will realloc on grow.
 *
 * @note Memory is allocated from an arena passed to functions.
 *       Does NOT store the arena - user passes it when needed.
 */
typedef struct StringBuilder {
    char *c_str;
    size_t size;
    size_t capacity;
} StringBuilder;

#define CR_STD_STRING_TRIM_LEFT -1
#define CR_STD_STRING_TRIM_BOTH 0
#define CR_STD_STRING_TRIM_RIGHT 1

#define CR_STD_STRING_EQUAL 1
#define CR_STD_STRING_DIFFERENT 0
#define CR_STD_STRING_ARG_LONGER -1
#define CR_STD_STRING_ARG1_LONGER -2
#define CR_STD_STRING_COMPARE_ERROR -3

#define CR_STD_STRING_BUILDER_DEFAULT_CAP 256

#define CR_STD_STRING_COLOR_BLACK 30
#define CR_STD_STRING_COLOR_RED 31
#define CR_STD_STRING_COLOR_GREEN 32
#define CR_STD_STRING_COLOR_YELLOW 33
#define CR_STD_STRING_COLOR_BLUE 34
#define CR_STD_STRING_COLOR_MAGENTA 35
#define CR_STD_STRING_COLOR_CYAN 36
#define CR_STD_STRING_COLOR_WHITE 37
#define CR_STD_STRING_COLOR_NONE 39

/**
 * @brief Creates a new `StringBuilder` struct with the provided string.
 *
 * @param `arena` The arena to store the memory in
 * @param `string` The initial string.
 *
 * @return A pointer to the new `StringBuilder` struct.
 * @return `NULL` if allocation fails.
 */
StringBuilder *cr_std_string_builder_new(Arena *arena, const char *string);

/**
 * @brief Creates a new `StringBuilder` struct with the provided string, and capacity.
 *
 * @param `arena` The arena to store the memory in
 * @param `string` The initial string.
 * @param `initial_capacity` The initial capacity.
 *
 * @return A pointer to the new `StringBuilder` struct.
 * @return `NULL` if allocation fails.
 */
StringBuilder *
cr_std_string_builder_newc(Arena *arena, const char *string, size_t initial_capacity);

/**
 * @brief Creates a new `StringBuilder` struct formatted with the provided format string and
 * arguments if any.
 *
 * @param `arena` The arena to store the memory in
 * @param `format` The format string used to format like `printf` (`%s`, `%d`, etc.).
 * @param `...` The arguments to format according to the format string.
 *
 * @return A pointer to the new `StringBuilder` struct.
 * @return `NULL` if allocation fails.
 */
StringBuilder *cr_std_string_builder_newf(Arena *arena, const char *format, ...);

/**
 * @brief Reserves additional space in the string builder array if needed.
 *
 * @param `arena` The arena to store the memory in
 * @param `string_builder` The `StringBuilder` struct to work on.
 * @param `additional` The amount of extra space wanted.
 *
 * @return `CR_STD_OK` on success.
 * @return `CR_STD_FAIL` on failure.
 */
b8 cr_std_string_builder_ensure_capacity(Arena *arena,
                                         StringBuilder *string_builder,
                                         size_t additional);

/**
 * @brief Adds a single string to the string builder.
 *
 * @param `arena` The arena to store the memory in
 * @param `string_builder` The `StringBuilder` struct to work on.
 * @param `string` The string to add.
 *
 * @return `CR_STD_OK` on success.
 * @return `CR_STD_FAIL` on failure.
 */
b8 cr_std_string_builder_append_string(Arena *arena,
                                       StringBuilder *string_builder,
                                       const char *string);

/**
 * @brief Adds a single char to the string builder.
 *
 * @param `arena` The arena to store the memory in
 * @param `string_builder` The `StringBuilder` struct to work on.
 * @param `ch` The char to add.
 *
 * @return `CR_STD_OK` on success.
 * @return `CR_STD_FAIL` on failure.
 */
b8 cr_std_string_builder_append_char(Arena *arena, StringBuilder *string_builder, char ch);

/**
 * @brief Adds a string to the string builder.
 *
 * @param `arena` The arena to store the memory in
 * @param `string_builder` The `StringBuilder` struct to work on.
 * @param `format` The format string used to format like `printf` (`%s`, `%d`, etc.).
 *                 Can also be a single string to add, e.g. "hello".
 *
 * @return `CR_STD_OK` on success.
 * @return `CR_STD_FAIL` on failure.
 */
b8 cr_std_string_builder_appendf(Arena *arena,
                                 StringBuilder *string_builder,
                                 const char *format,
                                 ...);

/**
 * @brief Adds strings to the string builder.
 *
 * @param `arena` The arena to store the memory in
 * @param `string_builder` A pointer to the `StringBuilder` struct to be updated.
 * @param `...` The strings to append.
 *
 * @return `CR_STD_OK` on success.
 * @return `CR_STD_FAIL` on failure.
 */
b8 cr_std_string_builder_append_null_terminated(Arena *arena, StringBuilder *string_builder, ...);

/**
 * @brief Macro to avoid having to add `NULL` to the end of arguments when calling the concat
 * function.
 *
 * @param `arena` The arena to store the memory in
 * @param `string_builder` A pointer to the `StringBuilder` struct to be updated.
 * @param `...` The strings to append.
 */
#define cr_std_string_builder_append(arena, string_builder, ...) \
    cr_std_string_builder_append_null_terminated(arena, string_builder, __VA_ARGS__, NULL)

/**
 * @brief Resets a string builders string, back to nothing.
 *
 * @param `string_builder` The `StringBuilder` struct to work on.
 *
 * @return `CR_STD_OK` on success.
 * @return `CR_STD_FAIL` on failure.
 */
b8 cr_std_string_builder_reset(StringBuilder *string_builder);

/**
 * @brief Resets a string builders string, back to the mark.
 *
 * @param `string_builder` The `StringBuilder` struct to work on.
 * @param `mark` The mark to set back too
 *
 * @return `CR_STD_OK` on success.
 * @return `CR_STD_FAIL` on failure.
 */
b8 cr_std_string_builder_reset_to_mark(StringBuilder *string_builder, size_t mark);

/**
 * @brief Returns the current mark
 *
 * @param `string_builder` The `StringBuilder` struct to work on.
 * @param `mark` output param for the mark
 *
 * @return `CR_STD_OK` on success.
 * @return `CR_STD_FAIL` on failure.
 */
b8 cr_std_string_builder_get_mark(StringBuilder *string_builder, size_t *mark);

/**
 * @brief Resets a string builders string, back to nothing.
 *
 * @param `arena` The arena to store the memory in
 * @param `string_builder` The `StringBuilder` struct to work on.
 *
 * @return A pointer to a new `String` struct.
 * @return `NULL` on failure.
 */
String *cr_std_string_builder_to_string(Arena *arena, StringBuilder *string_builder);

/**
 * @brief Creates a new `String` struct with the string provided.
 *
 * @param `arena` The arena to store the memory in
 * @param `string` The initial string
 *
 * @return A pointer to the new `String` struct.
 * @return `NULL` if allocation fails.
 */
String *cr_std_string_new(Arena *arena, const char *string);

/**
 * @brief Creates a new `String` struct formatted with the provided format string and arguments if
 * any.
 *
 * @param `arena` The arena to store the memory in
 * @param `format` The format string used to format like `printf` (`%s`, `%d`, etc.).
 * @param `...` The arguments to format according to the format string.
 *
 * @return A pointer to the new `String` struct.
 * @return `NULL` if allocation fails.
 */
String *cr_std_string_newf(Arena *arena, const char *format, ...);

/**
 * @brief Copy the contents of a `String` struct to another one.
 *
 * @param `arena` The arena to store the memory in
 * @param `src_string` A pointer to the source `String` struct.
 *
 * @return A pointer to the new `String` struct.
 * @return `NULL` if copy fails.
 */
String *cr_std_string_make_copy(Arena *arena, String *src_string);
// #define cr_std_string_make_copy_ptr ((void *(*)(void *))cr_std_string_make_copy)

/**
 * @brief Concatenates multiple strings and stores the result in an existing `String` struct.
 *
 * @param `arena` The arena to store the memory in
 * @param `string` A pointer to the `String` struct to be updated.
 * @param `...` The strings to concatenate, terminated with a `NULL`.
 *
 * @return `CR_STD_OK` on success.
 * @return `CR_STD_FAIL` on failure.
 */
b8 cr_std_string_concat_null_terminated(Arena *arena, String *string, ...);

/**
 * @brief Macro to avoid having to add `NULL` to the end of arguments when calling the concat
 * function.
 *
 * @param `arena` The arena to store the memory in
 * @param `string` A pointer to the `String` struct to be updated.
 * @param `...` The strings to concatenate.
 */
#define cr_std_string_concat(arena, string, ...) \
    cr_std_string_concat_null_terminated(arena, string, __VA_ARGS__, NULL)

/**
 * @brief Compares two strings.
 *
 * @param `arg` The first string to compare.
 * @param `arg1` The second string to compare.
 *
 * @return `CR_STD_STRING_EQUAL` if full equality (same characters in the same order).
 * @return `CR_STD_STRING_DIFFERENT` if equal in length but different characters.
 * @return `CR_STD_STRING_ARG_LONGER` if the first string is longer.
 * @return `CR_STD_STRING_ARG1_LONGER` if the second string is longer.
 * @return `CR_STD_STRING_COMPARE_ERROR` in case of errors.
 */
i32 cr_std_string_compare(String *arg, String *arg1);

/**
 * @brief Compares String and a c str.
 *
 * @param `arg` The first string to compare.
 * @param `arg1` The second raw string to compare.
 *
 * @return `CR_STD_STRING_EQUAL` if full equality (same characters in the same order).
 * @return `CR_STD_STRING_DIFFERENT` if equal in length but different characters.
 * @return `CR_STD_STRING_ARG_LONGER` if the first string is longer.
 * @return `CR_STD_STRING_ARG1_LONGER` if the second string is longer.
 * @return `CR_STD_STRING_COMPARE_ERROR` in case of errors.
 */
i32 cr_std_string_compare_c_str(String *arg, const char *arg1);

/**
 * @brief Trims a string of white space in both directions of the string.
 *
 * @param `string` The string to trim.
 * @param `direction` Which dir you want to cut or both
 *
 * Defined as `CR_STD_STRING_TRIM_LEFT`
 * Defined as `CR_STD_STRING_TRIM_RIGHT`
 * Defined as `CR_STD_STRING_TRIM_BOTH`
 *
 * @return `CR_STD_OK` if the string is trimmed successfully.
 * @return `CR_STD_FAIL` if the string failed to be trimmed.
 */
b8 cr_std_string_trim(String *string, int direction);

/**
 * @brief Finds the first index of a character in a given string.
 *
 * @param `string` The string to search through.
 * @param `ch` The character to locate.
 *
 * @return The index of the character in the string.
 * @return `-1` if the character is not found.
 */
i32 cr_std_string_find_char(String *string, char ch);

/**
 * @brief Finds the n index of a character in a given string.
 *
 * @param `string` The string to search through.
 * @param `ch` The character to locate.
 * @param `n` The n index to find
 *
 * @return The index of the n character in the string.
 * @return `-1` if the character is not found.
 */
i32 cr_std_string_find_char_n(String *string, char ch, int n);

/**
 * @brief Finds the last index of a character in a given string.
 *
 * @param `string` The string to search through.
 * @param `ch` The character to locate.
 *
 * @return The index of the character in the string.
 * @return `-1` if the character is not found.
 */
i32 cr_std_string_find_char_last(String *string, char ch);

/**
 * @brief Finds the first index of a string in a given string.
 *
 * @param `string` The string to search through.
 * @param `phrase` The string to locate.
 *
 * @return The index of the phrase in the string.
 * @return `-1` if the phrase is not found.
 */
i32 cr_std_string_find_string(String *string, const char *phrase);

/**
 * @brief Finds the n index of a string in a given string.
 *
 * @param `string` The string to search through.
 * @param `phrase` The string to locate.
 * @param `n` The n index to find
 *
 * @return The index of the n phrase in the string.
 * @return `-1` if the phrase is not found.
 */
i32 cr_std_string_find_string_n(String *string, const char *phrase, int n);

/**
 * @brief Finds the last index of a string in a given string.
 *
 * @param `string` The string to search through.
 * @param `phrase` The string to locate.
 *
 * @return The index of the phrase in the string.
 * @return `-1` if the phrase is not found.
 */
i32 cr_std_string_find_string_last(String *string, const char *phrase);

/**
 * @brief Check if a string contains a phrase.
 *
 * @param `string` The string to search through.
 * @param `phrase` The phrase / string to locate.
 *
 * @return `number of occurrences` if the string contains the phrase
 * @return `0` if the string does not contain the phrase
 */
i32 cr_std_string_contains_string(String *string, const char *phrase);

/**
 * @brief Check if a string contains a char.
 *
 * @param `string` The string to search through.
 * @param `ch` The char to locate.
 *
 * @return `number of occurrences` if the string contains the char.
 * @return `0` if the string does not contain the char.
 */
i32 cr_std_string_contains_char(String *string, char ch);

/**
 * @brief Check if a string has a prefix
 *
 * @param `string` The string to check.
 * @param `prefix` The prefix to check.
 *
 * @return `true` if the string has the prefix
 * @return `false` if the string does not have the prefix
 */
b8 cr_std_string_starts_with_string(String *string, const char *prefix);

/**
 * @brief Check if a string has a suffix
 *
 * @param `string` The string to check.
 * @param `suffix` The suffix to check.
 *
 * @return `true` if the string has the suffix
 * @return `false` if the string does not have the suffix
 */
b8 cr_std_string_ends_with_string(String *string, const char *suffix);

/**
 * @brief Check if a string has a prefix
 *
 * @param `string` The string to check.
 * @param `ch` The prefix to check.
 *
 * @return `true` if the string has the prefix
 * @return `false` if the string does not have the prefix
 */
b8 cr_std_string_starts_with_char(String *string, char prefix);

/**
 * @brief Check if a string has a suffix
 *
 * @param `string` The string to check.
 * @param `ch` The suffix to check.
 *
 * @return `true` if the string has the suffix
 * @return `false` if the string does not have the suffix
 */
b8 cr_std_string_ends_with_char(String *string, char suffix);

/**
 * @brief Fetch char at index
 *
 * @param `string` The string to work on.
 * @param `index` The index to fetch.
 *
 * @return `char` char at index
 * @return ' ' if error
 */
char cr_std_string_char_at(String *string, int index);

/**
 * @brief Calculate the hash code of a string.
 *
 * @param `string` The string to calculate the hash code for.
 * @param `hash` The output param to store the hash.
 *
 * @return `CR_STD_OK` on success.
 * @return `CR_STD_FAIL` on failure.
 */
b8 cr_std_string_hash_code(String *string, u64 *hash);

/**
 * @brief Splits a string into a `Vector`.
 * @brief Removes white space, use cr_std_string_split_hard if you require white space.
 *
 * @param `arena` The arena to store the memory in
 * @param `string` The string to split.
 * @param `delimiter` The character to split the string by.
 *
 * @return A pointer to a new `Vector` of the split strings.
 * @return `NULL` if error.
 */
Vector *cr_std_string_split(Arena *arena, String *string, char delimiter);

/**
 * @brief Splits a string into a `Vector` (includes white space / empty strings).
 * @brief Includes any white space and includes empty string.
 *
 * @param `arena` The arena to store the memory in
 * @param `string` The string to split.
 * @param `delimiter` The character to split the string by.
 *
 * @return A pointer to a new `Vector` of the split strings.
 * @return `NULL` if error.
 */
Vector *cr_std_string_split_hard(Arena *arena, String *string, char delimiter);

/**
 * @Convert a string to upper case.
 *
 * @param `string` The string to work on.
 *
 * @return `CR_STD_OK` if the string is changed.
 * @return `CR_STD_FAIL` if error.
 */
b8 cr_std_string_to_upper(String *string);

/**
 * @Convert a string to lower case.
 *
 * @param `string` The string to work on.
 *
 * @return `CR_STD_OK` if the string is changed.
 * @return `CR_STD_FAIL` if error.
 */
b8 cr_std_string_to_lower(String *string);

/**
 * @Convert a string to title case.
 *
 * @param `string` The string to work on.
 *
 * @return `CR_STD_OK` if the string is changed.
 * @return `CR_STD_FAIL` if error.
 */
b8 cr_std_string_to_title(String *string);

/**
 * @brief Replaces all occurrences of `from` with `to` in a given string.
 * @note Operation will be done in-place if possible, if not new memory will be allocated in the
 * arena
 *
 * @param `arena` The arena to store the memory in
 * @param `string` The string to work on.
 * @param `from` The string you want to replace.
 * @param `to` The string you want to replace `from` with.
 *
 * @return `number of replaced words`.
 * @return `0` if nothing is replaced.
 */
i32 cr_std_string_replace_string(Arena *arena, String *string, const char *from, const char *to);

/**
 * @brief Removes all characters that are not numbers.
 *
 * @param `string` The `String` to work on.
 *
 * @return `CR_STD_OK` on success.
 * @return `CR_STD_FAIL` on failure.
 */
b8 cr_std_string_remove_non_numeric(String *string);

/**
 * @brief Removes all characters that are considered numbers.
 *
 * @param `string` The `String` to work on.
 *
 * @return `CR_STD_OK` on success.
 * @return `CR_STD_FAIL` on failure.
 */
b8 cr_std_string_remove_numeric(String *string);

/**
 * @brief Returns the numerical representation of a string
 *
 * @param `string` The `String` to work on.
 * @param `number` The result.
 *
 * @return `CR_STD_OK` on success.
 * @return `CR_STD_FAIL` on failure.
 */
b8 cr_std_string_to_int(String *string, i32 *number);

/**
 * @brief Returns the string representation of a number.
 *
 * @param `arena` The arena to store the memory in
 * @param `number` The number to convert.
 *
 * @return `String` A pointer to a `String` struct representing the number.
 */
String *cr_std_string_from_int(Arena *arena, int number);

/**
 * @brief Returns a sub-string of a given string and index.
 *
 * @param `arena` The arena to store the memory in
 * @param `string` The original string.
 * @param `start_index` The index to start the sub-string.
 * @param `end_index` The index to end the sub-string.
 *
 * @return `String` A pointer to a `String` struct representing the sub-string.
 */
String *cr_std_string_sub_string(Arena *arena, String *string, int start_index, int end_index);

/**
 * @brief Colors an entire string to one color
 *
 * @param `arena` The arena to store the memory in
 * @param `string` The `String` to work from.
 * @param `color_code` The color code from ANSI escape codes, see `CR_STD_STRING_COLOR`.
 *
 * @return `String` A pointer to the new colored string.
 * @return `NULL` If something failed.
 */
String *cr_std_string_color_string(Arena *arena, String *string, int color_code);

/**
 * @brief Colors a specific phrase within a string
 *
 * @param `arena` The arena to store the memory in
 * @param `string` The `String` to work from.
 * @param `phrase` The phrase to color
 * @param `color_code` The color code from ANSI escape codes, see `CR_STD_STRING_COLOR`.
 *
 * @return `String` A pointer to the new colored string.
 * @return `NULL` If something failed.
 */
String *
cr_std_string_color_phrase(Arena *arena, String *string, const char *phrase, int color_code);

/**
 * @brief Strips the colors from a string
 *
 * @param `string` The `String` to work from.
 *
 * @return `CR_STD_OK` on success
 * @return `CR_STD_FAIL` on failure
 */
b8 cr_std_string_color_strip(String *string);

/**
 * @brief Repeats a string n times.
 *
 * @param `arena` The arena to store the memory in
 * @param `string` The `String` to repeat.
 * @param `n` The number of times to repeat.
 *
 * @return `String` A pointer to the new full string.
 * @return `String` A pointer to an empty string.
 */
String *cr_std_string_repeat(Arena *arena, const char *string, size_t n);

#ifdef __cplusplus
}
#endif
#endif // CR_STD_STRING
