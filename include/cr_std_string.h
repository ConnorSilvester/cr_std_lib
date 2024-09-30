#ifndef CR_STD_STRING
#define CR_STD_STRING

#include <stdio.h>

typedef struct vector_t vector_t;
typedef struct string_t {
    char *c_str;
    size_t length;
} string_t;

#define CR_STD_STRING_TRIM_LEFT -1
#define CR_STD_STRING_TRIM_BOTH 0
#define CR_STD_STRING_TRIM_RIGHT 1

/**
 * @brief Creates a new `string_t` struct formatted with the provided format string and arguments if any.
 *
 * @param `format` The format string used to format like `printf` (`%s`, `%d`, etc.).
 * @param `...` The arguments to format according to the format string.
 *
 * @return A pointer to the new `string_t` struct, or `NULL` if allocation fails.
 */
string_t *cr_std_string_new(const char *format, ...);

/**
 * @brief Free a `string_t` struct.
 *
 * @param `string` A pointer to the `string_t` struct.
 *
 * @return `1` on success, `0` on failure.
 */
int cr_std_string_free(string_t *string);

/**
 * @brief Concatenates multiple strings and stores the result in an existing `string_t` struct.
 *
 * @param `string` A pointer to the `string_t` struct to be updated.
 * @param `...` The strings to concatenate, terminated with a `NULL`.
 *
 * @return `1` on success, `0` on failure.
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
 * @param `str` The string to trim.
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
int cr_std_string_trim(string_t *str, int direction);

/**
 * @brief Checks to see if a given char is considered white space.
 *
 * @param `c` The char to check.
 *
 * @return `1` if it is white space.
 * @return `0` if is is not white space.
 */
int cr_std_string_is_char_white_space(char c);

/**
 * @brief Finds the first index of a character in a given string.
 *
 * @param `str` The string to search through.
 * @param `ch` The character to locate.
 *
 * @return The index of the character in the string.
 * @return `-1` if the character is not found.
 */
int cr_std_string_find_char(string_t *str, char ch);

/**
 * @brief Finds the last index of a character in a given string.
 *
 * @param `str` The string to search through.
 * @param `ch` The character to locate.
 *
 * @return The index of the character in the string.
 * @return `-1` if the character is not found.
 */
int cr_std_string_find_char_last(string_t *str, char ch);

/**
 * @brief Check if a string contains a phrase.
 *
 * @param `str` The string to search through.
 * @param `phrase` The phrase / string to locate.
 *
 * @return `1` if the string contains the phrase
 * @return `0` if the string does not contain the phrase
 */
int cr_std_string_contains_string(string_t *str, char *phrase);

/**
 * @brief Check if a string contains a char.
 *
 * @param `str` The string to search through.
 * @param `ch` The char to locate.
 *
 * @return `1` if the string contains the char
 * @return `0` if the string does not contain the char
 */
int cr_std_string_contains_char(string_t *str, char ch);

/**
 * @brief Calculate the hash code of a string.
 *
 * @param `str` The string to calculate the hash code for.
 *
 * @return The hash code of the string.
 */
unsigned long cr_std_string_hash_code(string_t *str);

/**
 * @brief Splits a string into a `vector_t`.
 *
 * @param `str` The string to split.
 * @param `delimiter` The character to split the string by.
 *
 * @return A pointer to a new `vector_t` of the split strings.
 */
vector_t *cr_std_string_split(string_t *str, char delimiter);

#endif // CR_STD_STRING
