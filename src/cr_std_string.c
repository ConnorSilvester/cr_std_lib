#include "cr_std_string.h"
#include "cr_std_logger.h"
#include "cr_std_vector.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wctype.h>

#define CR_STD_STRING_ANSI_COLOR_ESCAPE_SEQ "\033[%dm%s\033[0m"

StringBuilder *cr_std_string_builder_new(const char *string) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_builder_new -> char* input was NULL");
        return NULL;
    }

    StringBuilder *string_builder = (StringBuilder *)malloc(sizeof(StringBuilder));

    if (!string_builder) {
        cr_std_logger_out(
        CR_STD_LOGGER_LOG_TYPE_ERROR,
        "cr_std_string_builder_new -> failed to allocate memory for new String struct");
        return NULL;
    }

    string_builder->size = strlen(string);
    if (string_builder->size == 0) {
        string_builder->capacity = CR_STD_STRING_BUILDER_DEFAULT_CAP;
    } else {
        string_builder->capacity = string_builder->size * 2;
    }

    char *c_str = (char *)malloc(sizeof(char) * (string_builder->capacity + 1));
    if (!c_str) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_builder_new -> failed to allocate memory for buffer");
        free(string_builder);
        return NULL;
    }

    memcpy(c_str, string, string_builder->size);
    c_str[string_builder->size] = '\0';
    string_builder->c_str = c_str;

    return string_builder;
}

StringBuilder *cr_std_string_builder_newf(const char *format, ...) {
    if (!format) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_builder_newf -> char* input was NULL");
        return NULL;
    }

    StringBuilder *string_builder = (StringBuilder *)malloc(sizeof(StringBuilder));

    if (!string_builder) {
        cr_std_logger_out(
        CR_STD_LOGGER_LOG_TYPE_ERROR,
        "cr_std_string_builder_newf -> failed to allocate memory for new String struct");
        return NULL;
    }

    va_list args;
    va_start(args, format);

    string_builder->size = vsnprintf(NULL, 0, format, args);
    if (string_builder->size == 0) {
        string_builder->capacity = CR_STD_STRING_BUILDER_DEFAULT_CAP;
    } else {
        string_builder->capacity = string_builder->size * 2;
    }
    va_end(args);

    if (string_builder->size < 0) {
        cr_std_logger_out(
        CR_STD_LOGGER_LOG_TYPE_ERROR,
        "cr_std_string_builder_newf -> vsnprintf failed during length calculation");
        free(string_builder);
        va_end(args);
        return NULL;
    }

    char *c_str = (char *)malloc(sizeof(char) * string_builder->capacity);
    if (!c_str) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_builder_newf -> failed to allocate memory for buffer");
        free(string_builder);
        va_end(args);
        return NULL;
    }

    va_start(args, format);
    vsnprintf(c_str, string_builder->size + 1, format, args);
    va_end(args);

    string_builder->c_str = c_str;

    return string_builder;
}

int cr_std_string_builder_ensure_capacity(StringBuilder *string_builder, size_t additional) {
    if (!string_builder) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_builder_ensure_capacity -> StringBuilder is NULL");
        return 1;
    }

    if (string_builder->size + additional < string_builder->capacity) {
        return 0;
    }

    string_builder->capacity = (string_builder->size + additional);
    void *temp = realloc(string_builder->c_str, string_builder->capacity);
    if (!temp) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_builder_ensure_capacity -> Failed to reallocate memory");
        return 1;
    }
    string_builder->c_str = temp;

    return 0;
}

int cr_std_string_builder_append_string(StringBuilder *string_builder, const char *string) {
    if (!string_builder) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_builder_append_string -> given string builder is NULL");
        return 1;
    }

    size_t string_to_append_length = strlen(string);
    if (string_builder->size + string_to_append_length >= string_builder->capacity) {
        string_builder->capacity = (string_builder->size + string_to_append_length) * 2;

        void *temp = realloc(string_builder->c_str, string_builder->capacity);
        if (!temp) {
            cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                              "cr_std_string_builder_append_string -> failed to realloc memory for "
                              "StringBuilder->c_str");
            return 1;
        }
        string_builder->c_str = temp;
    }

    memcpy(string_builder->c_str + string_builder->size, string, string_to_append_length);

    string_builder->size += string_to_append_length;
    string_builder->c_str[string_builder->size] = '\0';
    return 0;
}

int cr_std_string_builder_append_char(StringBuilder *string_builder, char ch) {
    if (!string_builder) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_builder_append_char -> given string builder is NULL");
        return 1;
    }

    if (string_builder->size + 1 >= string_builder->capacity) {
        string_builder->capacity = (string_builder->size + 1) * 2;

        void *temp = realloc(string_builder->c_str, string_builder->capacity);
        if (!temp) {
            cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                              "cr_std_string_builder_append_char -> failed to realloc memory");
            return 1;
        }
        string_builder->c_str = temp;
    }

    string_builder->c_str[string_builder->size] = ch;
    string_builder->size += 1;
    string_builder->c_str[string_builder->size] = '\0';

    return 0;
}

int cr_std_string_builder_appendf(StringBuilder *string_builder, const char *format, ...) {
    if (!string_builder) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_builder_appendf -> given string builder is NULL");
        return 1;
    }

    va_list args;
    va_start(args, format);
    size_t string_to_append_length = vsnprintf(NULL, 0, format, args);
    va_end(args);

    char *buffer = (char *)malloc(string_to_append_length + 1);
    if (!buffer) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_builder_appendf -> memory allocation failed for buffer");
        return 1;
    }

    va_start(args, format);
    vsnprintf(buffer, string_to_append_length + 1, format, args);
    va_end(args);

    buffer[string_to_append_length] = '\0';
    int result = cr_std_string_builder_append_string(string_builder, buffer);

    free(buffer);
    return result;
}

int cr_std_string_builder_append_null_terminated(StringBuilder *string_builder, ...) {
    if (!string_builder) {
        cr_std_logger_out(
        CR_STD_LOGGER_LOG_TYPE_ERROR,
        "cr_std_string_builder_append_null_terminated -> string builder pointer is NULL");
        return 1;
    }

    va_list args;
    va_start(args, string_builder);

    char *current_string;
    while ((current_string = va_arg(args, char *)) != NULL) {
        int result = cr_std_string_builder_append_string(string_builder, current_string);
        if (result != 0) {
            return result;
        }
    }
    va_end(args);
    return 0;
}

int cr_std_string_builder_reset(StringBuilder *string_builder) {
    if (!string_builder) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_builder_reset -> given string builder is NULL");
        return 1;
    }
    string_builder->c_str[0] = '\0';
    string_builder->size = 0;
    return 0;
}

String *cr_std_string_builder_to_string(StringBuilder *string_builder) {
    if (!string_builder) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_builder_to_string -> given string builder is NULL");
        return NULL;
    }
    return cr_std_string_new(string_builder->c_str);
}

int cr_std_string_builder_free(StringBuilder **sb_ptr) {
    if (sb_ptr && *sb_ptr) {
        if ((*sb_ptr)->c_str) {
            free((*sb_ptr)->c_str);
            (*sb_ptr)->c_str = NULL;
        }

        free(*sb_ptr);
        *sb_ptr = NULL;
        return 0;
    }
    cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_WARNING,
                      "cr_std_string_builder_free -> tried to free a NULL StringBuilder*");
    return 1;
}

String *cr_std_string_new(const char *string) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_new -> char* input was NULL");
        return NULL;
    }

    String *new_string = (String *)malloc(sizeof(String));

    if (!new_string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_new -> failed to allocate memory for new String struct");
        return NULL;
    }

    new_string->length = strlen(string);

    char *c_str = (char *)malloc(sizeof(char) * (new_string->length + 1));
    if (!c_str) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_new -> failed to allocate memory for buffer");
        cr_std_string_free(&new_string);
        return NULL;
    }

    memcpy(c_str, string, new_string->length);
    c_str[new_string->length] = '\0';
    new_string->c_str = c_str;

    return new_string;
}

String *cr_std_string_newf(const char *format, ...) {
    if (!format) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_newf -> char* input was NULL");
        return NULL;
    }

    String *string = (String *)malloc(sizeof(String));

    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_newf -> failed to allocate memory for new String struct");
        return NULL;
    }

    va_list args;
    va_start(args, format);

    string->length = vsnprintf(NULL, 0, format, args);
    va_end(args);

    if (string->length < 0) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_newf -> vsnprintf failed during length calculation");
        cr_std_string_free(&string);
        va_end(args);
        return NULL;
    }

    char *c_str = (char *)malloc(sizeof(char) * (string->length + 1));
    if (!c_str) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_newf -> failed to allocate memory for buffer");
        cr_std_string_free(&string);
        va_end(args);
        return NULL;
    }

    va_start(args, format);
    vsnprintf(c_str, string->length + 1, format, args);
    va_end(args);

    string->c_str = c_str;

    return string;
}

int cr_std_string_free(String **string_ptr) {
    if (string_ptr && *string_ptr) {
        if ((*string_ptr)->c_str) {
            free((*string_ptr)->c_str);
            (*string_ptr)->c_str = NULL;
        }

        free(*string_ptr);
        *string_ptr = NULL;
        return 0;
    }
    cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_WARNING,
                      "cr_std_string_free -> tried to free a NULL String*");
    return 1;
}

String *cr_std_string_make_copy(String *src_string) {
    if (!src_string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_make_copy -> source string pointer is NULL");
        return NULL;
    }
    if (!src_string->c_str) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_make_copy -> source string's c_str pointer is NULL");
        return NULL;
    }

    String *new_string = cr_std_string_new(src_string->c_str);
    if (!new_string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_make_copy -> failed to allocate memory for new string");
        return NULL;
    }

    return new_string;
}

int cr_std_string_concat_null_terminated(String *string, ...) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_concat_null_terminated -> string pointer is NULL");
        return 1;
    }

    va_list args;
    va_start(args, string);

    size_t total_length = string->length;
    char *current_string;
    while ((current_string = va_arg(args, char *)) != NULL) {
        total_length += strlen(current_string);
    }
    va_end(args);

    char *new_c_str = (char *)malloc(sizeof(char) * (total_length + 1));
    if (!new_c_str) {
        cr_std_logger_out(
        CR_STD_LOGGER_LOG_TYPE_ERROR,
        "cr_std_string_concat_null_terminated -> Failed to allocate memory for new String struct");
        return 1;
    }

    size_t c_str_current_pos = 0;
    memcpy(new_c_str, string->c_str, string->length);
    c_str_current_pos += string->length;

    va_start(args, string);
    while ((current_string = va_arg(args, char *)) != NULL) {
        size_t current_length = strlen(current_string);
        memcpy(new_c_str + c_str_current_pos, current_string, current_length);
        c_str_current_pos += current_length;
    }
    va_end(args);

    new_c_str[c_str_current_pos] = '\0';

    free(string->c_str);
    string->c_str = new_c_str;
    string->length = total_length;

    return 0;
}

int cr_std_string_compare(String *arg, String *arg1) {
    if (!arg || !arg1) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_compare -> string pointer is NULL");
        return CR_STD_STRING_COMPARE_ERROR;
    }

    // First string is longer
    if (arg->length > arg1->length) {
        return -1;
    }
    // Second string is longer
    if (arg->length < arg1->length) {
        return -2;
    }
    for (int i = 0; i < arg->length; i++) {
        if (arg->c_str[i] != arg1->c_str[i]) {
            // Partial equality ( length is equal but not chars )
            return 0;
        }
    }
    // Fully equal
    return 1;
}

int cr_std_string_compare_c_str(String *arg, const char *arg1) {
    // First string is longer
    size_t arg1_length = strlen(arg1);
    if (arg->length > arg1_length) {
        return -1;
    }
    // Second string is longer
    if (arg->length < arg1_length) {
        return -2;
    }
    for (int i = 0; i < arg->length; i++) {
        if (arg->c_str[i] != arg1[i]) {
            // Partial equality ( length is equal but not chars )
            return 0;
        }
    }
    // Fully equal
    return 1;
}

int cr_std_string_trim(String *string, int direction) {
    if (string == NULL || string->c_str == NULL || string->length == 0) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_trim -> tried to trim an invalid string");
        return 1;
    }

    int start = 0;
    int end = string->length - 1;

    if (direction == CR_STD_STRING_TRIM_LEFT || direction == CR_STD_STRING_TRIM_BOTH) {
        while (start <= end && iswspace(string->c_str[start])) {
            start++;
        }
    }

    if (direction == CR_STD_STRING_TRIM_RIGHT || direction == CR_STD_STRING_TRIM_BOTH) {
        while (end >= start && iswspace(string->c_str[end])) {
            end--;
        }
    }

    int new_length = end - start + 1;
    if (new_length == 0) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_WARNING,
                          "cr_std_string_trim -> new string is empty");
    }
    char *new_str = malloc(sizeof(char) * (new_length + 1));
    if (new_str == NULL) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_trim -> malloc failed to allocate memory for new string");
        return 1;
    }

    memcpy(new_str, string->c_str + start, new_length);
    new_str[new_length] = '\0';

    free(string->c_str);
    string->c_str = new_str;
    string->length = new_length;

    return 0;
}

int cr_std_string_find_char(String *string, char ch) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_find_char -> string pointer is NULL");
        return -1;
    }
    for (int i = 0; i <= string->length; i++) {
        if (string->c_str[i] == ch) {
            return i;
        }
    }
    return -1;
}

int cr_std_string_find_char_n(String *string, char ch, int n) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_find_char_n -> string pointer is NULL");
        return -1;
    }
    if (n <= 0) {
        n = 1;
    }

    int current_n = 0;
    int last_char_index = -1;
    for (int i = 0; i <= string->length; i++) {
        if (string->c_str[i] == ch) {
            last_char_index = i;
            current_n++;
        }

        if (current_n == n) {
            return last_char_index;
        }
    }
    return -1;
}

int cr_std_string_find_char_last(String *string, char ch) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_find_char_last -> string pointer is NULL");
        return -1;
    }
    for (int i = string->length; i >= 0; i--) {
        if (string->c_str[i] == ch) {
            return i;
        }
    }
    return -1;
}

int cr_std_string_find_string(String *string, const char *phrase) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_find_string -> string pointer is NULL");
        return 0;
    }

    int phrase_length = strlen(phrase);
    if (phrase_length > string->length || phrase_length < 1) {
        return 0;
    }

    for (int string_index = 0; string_index <= string->length - phrase_length; string_index++) {
        int found = 1;
        for (int phrase_index = 0; phrase_index < phrase_length; phrase_index++) {
            if (string->c_str[string_index + phrase_index] != phrase[phrase_index]) {
                found = 0;
                break;
            }
        }
        if (found) {
            return string_index;
        }
    }
    return -1;
}

int cr_std_string_find_string_n(String *string, const char *phrase, int n) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_find_string_n -> string pointer is NULL");
        return 0;
    }
    if (n <= 0) {
        n = 1;
    }

    int phrase_length = strlen(phrase);
    if (phrase_length > string->length || phrase_length < 1) {
        return 0;
    }

    int current_n = 0;
    int last_string_index = -1;
    for (int string_index = 0; string_index <= string->length - phrase_length; string_index++) {
        int found = 1;
        for (int phrase_index = 0; phrase_index < phrase_length; phrase_index++) {
            if (string->c_str[string_index + phrase_index] != phrase[phrase_index]) {
                found = 0;
                break;
            }
        }
        if (found) {
            last_string_index = string_index;
            current_n++;
        }
        if (current_n == n) {
            return last_string_index;
        }
    }
    return -1;
}

int cr_std_string_find_string_last(String *string, const char *phrase) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_find_string_last -> string pointer is NULL");
        return 0;
    }

    int phrase_length = strlen(phrase);
    if (phrase_length > string->length || phrase_length < 1) {
        return 0;
    }

    int last_string_index = -1;
    for (int string_index = 0; string_index <= string->length - phrase_length; string_index++) {
        int found = 1;
        for (int phrase_index = 0; phrase_index < phrase_length; phrase_index++) {
            if (string->c_str[string_index + phrase_index] != phrase[phrase_index]) {
                found = 0;
                break;
            }
        }
        if (found) {
            last_string_index = string_index;
        }
    }
    return last_string_index;
}

int cr_std_string_contains_string(String *string, const char *phrase) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_contains_string -> string pointer is NULL");
        return 0;
    }

    int phrase_length = strlen(phrase);
    if (phrase_length > string->length || phrase_length < 1) {
        return 0;
    }

    int occurrences = 0;
    for (int string_index = 0; string_index <= string->length - phrase_length; string_index++) {
        int found = 1;
        for (int phrase_index = 0; phrase_index < phrase_length; phrase_index++) {
            if (string->c_str[string_index + phrase_index] != phrase[phrase_index]) {
                found = 0;
                break;
            }
        }
        if (found) {
            occurrences++;
        }
    }
    return occurrences;
}

int cr_std_string_contains_char(String *string, char ch) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_contains_char -> string pointer is NULL");
        return 0;
    }

    int occurrences = 0;
    for (int i = 0; i <= string->length; i++) {
        if (string->c_str[i] == ch) {
            occurrences++;
        }
    }
    return occurrences;
}

int cr_std_string_starts_with_string(String *string, const char *prefix) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_starts_with_string -> string pointer is NULL");
        return 0;
    }

    int prefix_length = strlen(prefix);
    if (prefix_length > string->length || prefix_length < 1) {
        return 0;
    }

    for (int i = 0; i < prefix_length; i++) {
        if (string->c_str[i] != prefix[i]) {
            return 0;
        }
    }

    return 1;
}

int cr_std_string_ends_with_string(String *string, const char *suffix) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_ends_with_string -> string pointer is NULL");
        return 0;
    }

    int suffix_length = strlen(suffix);
    if (suffix_length > string->length || suffix_length < 1) {
        return 0;
    }

    int start_index = string->length - suffix_length;

    for (int i = 0; i < suffix_length; i++) {
        if (string->c_str[start_index + i] != suffix[i]) {
            return 0;
        }
    }

    return 1;
}

int cr_std_string_starts_with_char(String *string, char prefix) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_starts_with_char -> string pointer is NULL");
        return 0;
    }

    if (string->length < 1) {
        return 0;
    }

    return (string->c_str[0] == prefix) ? 1 : 0;
}

int cr_std_string_ends_with_char(String *string, char suffix) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_ends_with_char -> string pointer is NULL");
        return 0;
    }

    if (string->length < 1) {
        return 0;
    }

    return (string->c_str[string->length - 1] == suffix) ? 1 : 0;
}

char cr_std_string_char_at(String *string, int index) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_char_at -> string pointer is NULL");
        return ' ';
    }
    if (!string->c_str) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_char_at -> string->c_str pointer is NULL");
        return ' ';
    }

    if (index < 0) {
        index = 0;
    }

    if (index >= string->length) {
        index = string->length - 1;
    }

    return string->c_str[index];
}

unsigned long cr_std_string_hash_code(String *string) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_hash_code -> string pointer is NULL");
        return -1;
    }
    unsigned long hash = 5381; // Initial hash value

    for (size_t i = 0; i < string->length; i++) {
        hash = ((hash << 5) + hash) + (int)(string->c_str[i]); // hash * 33 + char
    }

    return hash;
}

Vector *cr_std_string_split(String *string, char delimiter) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_split -> string pointer is NULL");
        return NULL;
    }
    Vector *vector = cr_std_vector_new(String *);
    vector->free_function = cr_std_string_free_ptr;

    char *buffer = (char *)malloc(string->length + 1);
    if (!buffer) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_split -> memory allocation failed");
        cr_std_vector_free(&vector);
        return NULL;
    }

    int buffer_index = 0;
    for (size_t i = 0; i < string->length; i++) {
        if (string->c_str[i] == delimiter || i == string->length - 1) {
            if (string->c_str[i] != delimiter) {
                buffer[buffer_index] = string->c_str[i];
                buffer_index++;
            }

            buffer[buffer_index] = '\0';

            // Avoid adding empty strings to the vector
            if (buffer_index > 0) {
                String *string = cr_std_string_new(buffer);
                cr_std_vector_push_back(vector, string);
            }

            buffer_index = 0;
        } else {
            buffer[buffer_index] = string->c_str[i];
            buffer_index++;
        }
    }
    return vector;
}

Vector *cr_std_string_split_hard(String *string, char delimiter) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_split_hard -> string pointer is NULL");
        return NULL;
    }
    Vector *vector = cr_std_vector_new(String *);
    vector->free_function = cr_std_string_free_ptr;

    char *buffer = (char *)malloc(string->length + 1);
    if (!buffer) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_split_hard -> memory allocation failed");
        cr_std_vector_free(&vector);
        return NULL;
    }

    int buffer_index = 0;
    for (size_t i = 0; i < string->length; i++) {
        if (string->c_str[i] == delimiter || i == string->length - 1) {
            if (string->c_str[i] != delimiter) {
                buffer[buffer_index] = string->c_str[i];
                buffer_index++;
            }

            buffer[buffer_index] = '\0';

            String *string = cr_std_string_new(buffer);
            cr_std_vector_push_back(vector, string);

            buffer_index = 0;
        } else {
            buffer[buffer_index] = string->c_str[i];
            buffer_index++;
        }
    }
    return vector;
}

int cr_std_string_to_upper(String *string) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_to_upper -> string pointer is NULL");
        return 1;
    }
    for (int i = 0; i < string->length; i++) {
        string->c_str[i] = toupper(string->c_str[i]);
    }
    return 0;
}

int cr_std_string_to_lower(String *string) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_to_lower -> string pointer is NULL");
        return 1;
    }
    for (int i = 0; i < string->length; i++) {
        string->c_str[i] = tolower(string->c_str[i]);
    }
    return 0;
}

int cr_std_string_to_title(String *string) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_to_title -> string pointer is NULL");
        return 1;
    }
    if (string->length < 1) {
        return 1;
    }

    bool new_word = true;
    for (size_t i = 0; i < string->length; i++) {
        char c = string->c_str[i];
        if (isspace(c) || ispunct(c)) {
            new_word = true;
        } else if (new_word) {
            string->c_str[i] = toupper(c);
            new_word = false;
        } else {
            string->c_str[i] = tolower(c);
        }
    }

    return 0;
}

int cr_std_string_replace_string(String *string, const char *from, const char *to) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_replace_string -> string pointer is NULL");
        return 0;
    }

    int occurrences = cr_std_string_contains_string(string, from);
    if (occurrences == 0) {
        return 0;
    }

    int from_length = strlen(from);
    int to_length = strlen(to);

    int new_length = string->length + (to_length - from_length) * occurrences;
    char *new_string = malloc(sizeof(char) * (new_length + 1));
    if (!new_string) {
        cr_std_logger_out(
        CR_STD_LOGGER_LOG_TYPE_ERROR,
        "cr_std_string_replace_string -> malloc allocation failed for new_string");
        return 0;
    }

    // src = start of string
    char *src = string->c_str;
    char *dest = new_string;
    char *current_pos_in_string = strstr(src, from);

    int successfully_changed_words = 0;
    while (current_pos_in_string != NULL) {
        // Copy everything before the from match
        int bytes_before_match = current_pos_in_string - src;
        memcpy(dest, src, bytes_before_match);
        dest += bytes_before_match;

        // Copy the to string in place of from
        memcpy(dest, to, to_length);
        dest += to_length;

        // Move the source pointer past the from string
        src = current_pos_in_string + from_length;
        current_pos_in_string = strstr(src, from);
        successfully_changed_words++;
    }

    // Copy rest of the string
    strcpy(dest, src);

    free(string->c_str);
    string->c_str = new_string;
    string->length = new_length;

    return successfully_changed_words;
}

int cr_std_string_remove_non_numeric(String *string) {
    if (!string || !string->c_str) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_remove_non_numeric -> string pointer is NULL");
        return 1;
    }

    size_t digit_count = 0;
    for (size_t i = 0; i < string->length; ++i) {
        if (isdigit((unsigned char)string->c_str[i])) {
            digit_count++;
        }
    }

    char *filtered_str = (char *)malloc(digit_count + 1);
    if (!filtered_str) {
        cr_std_logger_out(
        CR_STD_LOGGER_LOG_TYPE_ERROR,
        "cr_std_string_remove_non_numeric -> malloc allocation failed for filtered_str");
        return 1;
    }

    size_t filtered_str_index = 0;
    for (size_t i = 0; i < string->length; ++i) {
        if (isdigit((unsigned char)string->c_str[i])) {
            filtered_str[filtered_str_index] = string->c_str[i];
            filtered_str_index++;
        }
    }
    filtered_str[filtered_str_index] = '\0';

    free(string->c_str);
    string->c_str = filtered_str;
    string->length = digit_count;
    return 0;
}

int cr_std_string_remove_numeric(String *string) {
    if (!string || !string->c_str) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_remove_numeric -> string pointer is NULL");
        return 1;
    }

    size_t new_char_count = 0;
    for (size_t i = 0; i < string->length; ++i) {
        if (!isdigit((unsigned char)string->c_str[i])) {
            new_char_count++;
        }
    }

    char *filtered_str = (char *)malloc(new_char_count + 1);
    if (!filtered_str) {
        cr_std_logger_out(
        CR_STD_LOGGER_LOG_TYPE_ERROR,
        "cr_std_string_remove_numeric -> malloc allocation failed for filtered_str");
        return 1;
    }

    size_t filtered_str_index = 0;
    for (size_t i = 0; i < string->length; ++i) {
        if (!isdigit((unsigned char)string->c_str[i])) {
            filtered_str[filtered_str_index] = string->c_str[i];
            filtered_str_index++;
        }
    }
    filtered_str[filtered_str_index] = '\0';

    free(string->c_str);
    string->c_str = filtered_str;
    string->length = new_char_count;
    return 0;
}

int cr_std_string_to_int(String *string) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_to_int -> string pointer is NULL");
        return 0;
    }
    String *string_copy = cr_std_string_make_copy(string);
    cr_std_string_remove_non_numeric(string_copy);
    int result = strtol(string_copy->c_str, NULL, 10);
    cr_std_string_free(&string_copy);
    return result;
}

String *cr_std_string_from_int(int number) {
    char str_buffer[50];
    int result = snprintf(str_buffer, sizeof(str_buffer), "%d", number);

    if (result < 0 || result >= sizeof(str_buffer)) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_from_int -> snprintf failed");
        return NULL;
    }

    String *string = cr_std_string_new(str_buffer);
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_from_int -> failed to create String");
        return NULL;
    }
    return string;
}

String *cr_std_string_sub_string(String *string, int start_index, int end_index) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_WARNING,
                          "cr_std_string_sub_string -> given string is NULL");
        return cr_std_string_new("");
    }

    if (string->length == 0) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_WARNING,
                          "cr_std_string_sub_string -> given string is empty");
        return cr_std_string_new("");
    }

    if (start_index < 0) {
        start_index = 0;
    }

    if (end_index < start_index) {
        return cr_std_string_new("");
    }

    if (end_index > string->length) {
        end_index = string->length;
    }

    size_t sub_length = end_index - start_index;

    char *sub_data = (char *)malloc(sub_length + 1);
    if (!sub_data) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_sub_string -> failed to allocate memory for substring");
        return cr_std_string_new("");
    }

    strncpy(sub_data, string->c_str + start_index, sub_length);
    sub_data[sub_length] = '\0';
    String *new_string = cr_std_string_new(sub_data);

    free(sub_data);
    return new_string;
}

String *cr_std_string_from_string_ptr_vector(Vector *vector, const char *delimiter) {
    if (!vector) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_from_string_ptr_vector -> given vector is NULL");
        return NULL;
    }

    if (!cr_std_vector_contains_pointer(vector) || vector->type_size != sizeof(String *)) {
        cr_std_logger_out(
        CR_STD_LOGGER_LOG_TYPE_ERROR,
        "cr_std_string_from_string_ptr_vector -> given vector does not contain String pointers");
        return NULL;
    }

    StringBuilder *sb = cr_std_string_builder_new("");

    for (int i = 0; i < vector->size; i++) {
        String *current_string = cr_std_vector_get_at(vector, String, i);
        if (!current_string) {
            cr_std_logger_outf(
            CR_STD_LOGGER_LOG_TYPE_WARNING,
            "cr_std_string_from_string_ptr_vector -> element at index %zu is NULL", i);
            continue; // Skip NULL elements.
        }

        cr_std_string_builder_append(sb, current_string->c_str);
        if (i < vector->size - 1) {
            cr_std_string_builder_append(sb, delimiter);
        }
    }

    String *final_string = cr_std_string_builder_to_string(sb);
    cr_std_string_builder_free(&sb);

    if (!final_string) {
        cr_std_logger_out(
        CR_STD_LOGGER_LOG_TYPE_ERROR,
        "cr_std_string_from_string_ptr_vector -> failed to convert string builder to string");
    }

    return final_string;
}

String *cr_std_string_from_char_ptr_vector(Vector *vector, const char *delimiter) {
    if (!vector) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_from_char_ptr_vector -> given vector is NULL");
        return NULL;
    }

    if (!cr_std_vector_contains_pointer(vector) || vector->type_size != sizeof(char *)) {
        cr_std_logger_out(
        CR_STD_LOGGER_LOG_TYPE_ERROR,
        "cr_std_string_from_char_ptr_vector -> given vector does not contain char pointers");
        return NULL;
    }

    StringBuilder *sb = cr_std_string_builder_new("");

    for (int i = 0; i < vector->size; i++) {
        char *current_string = cr_std_vector_get_at(vector, char, i);
        if (!current_string) {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_WARNING,
                               "cr_std_string_from_char_ptr_vector -> element at index %zu is NULL",
                               i);
            continue; // Skip NULL elements.
        }

        cr_std_string_builder_append(sb, current_string);
        if (i < vector->size - 1) {
            cr_std_string_builder_append(sb, delimiter);
        }
    }

    String *final_string = cr_std_string_builder_to_string(sb);
    cr_std_string_builder_free(&sb);

    if (!final_string) {
        cr_std_logger_out(
        CR_STD_LOGGER_LOG_TYPE_ERROR,
        "cr_std_string_from_char_ptr_vector -> failed to convert string builder to string");
    }

    return final_string;
}

String *cr_std_string_color_string(String *string, int color_code) {
    if (!string || !string->c_str) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_color_string -> string is NULL");
        return NULL;
    }
    if (color_code < 30 || color_code >= 40) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_WARNING,
                          "cr_std_string_color_string -> invalid color code");
        color_code = CR_STD_STRING_COLOR_NONE;
    }
    return cr_std_string_newf(CR_STD_STRING_ANSI_COLOR_ESCAPE_SEQ, color_code, string->c_str);
}

String *cr_std_string_color_phrase(String *string, const char *phrase, int color_code) {
    if (!string || !string->c_str) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_color_phrase -> string is NULL");
        return NULL;
    }
    if (color_code < 30 || color_code >= 40) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_WARNING,
                          "cr_std_string_color_phrase -> invalid color code");
        color_code = CR_STD_STRING_COLOR_NONE;
    }
    String *str_copy = cr_std_string_make_copy(string);
    String *phrase_colored =
    cr_std_string_newf(CR_STD_STRING_ANSI_COLOR_ESCAPE_SEQ, color_code, phrase);
    cr_std_string_replace_string(str_copy, phrase, phrase_colored->c_str);
    cr_std_string_free(&phrase_colored);
    return str_copy;
}

int cr_std_string_color_strip(String *string) {
    if (!string || !string->c_str) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_color_strip -> string is NULL");
        return 1;
    }

    for (int i = 30; i < 40; i++) {
        char color_code[16];
        snprintf(color_code, sizeof(color_code), "\033[%dm", i);
        cr_std_string_replace_string(string, color_code, "");
    }

    cr_std_string_replace_string(string, "\033[0m", "");
    return 0;
}

String *cr_std_string_repeat(const char *string, size_t n) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_string_repeat -> string is NULL");
        return NULL;
    }

    if (n == 0) {
        return cr_std_string_new("");
    }

    size_t len = strlen(string);
    if (len > SIZE_MAX / n) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_repeat -> string is too large (n might be negative)");
        return cr_std_string_new("");
    }
    size_t total_length = len * n;

    StringBuilder *sb = cr_std_string_builder_new("");
    cr_std_string_builder_ensure_capacity(sb, total_length);
    for (size_t i = 0; i < n; i++) {
        cr_std_string_builder_append_string(sb, string);
    }

    String *result = cr_std_string_builder_to_string(sb);
    cr_std_string_builder_free(&sb);
    return result;
}
