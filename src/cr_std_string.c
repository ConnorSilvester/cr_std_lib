#include "cr_std_string.h"
#include "cr_std_arena.h"
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

StringBuilder *cr_std_string_builder_new(Arena *arena, const char *string) {
    cr_std_string_builder_newc(arena, string, 0);
}

StringBuilder *
cr_std_string_builder_newc(Arena *arena, const char *string, size_t initial_capacity) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_string_builder_newc -> arena* was NULL");
        return NULL;
    }

    if (!string) {
        CR_LOG_ERROR("cr_std_string_builder_newc -> string was NULL");
        return NULL;
    }

    StringBuilder *string_builder = cr_std_arena_alloc(arena, sizeof(*string_builder));
    if (!string_builder) {
        CR_LOG_ERROR("cr_std_string_builder_newc -> failed to allocate StringBuilder");
        return NULL;
    }

    size_t len = strlen(string);
    string_builder->size = len;

    if (initial_capacity > 0) {
        string_builder->capacity = (initial_capacity > len + 1) ? initial_capacity : len + 1;
    } else {
        string_builder->capacity = (len == 0) ? CR_STD_STRING_BUILDER_DEFAULT_CAP : len * 2;
    }

    char *c_str = cr_std_arena_alloc(arena, string_builder->capacity + 1);
    if (!c_str) {
        CR_LOG_ERROR("cr_std_string_builder_new_with_capacity -> failed to allocate buffer");
        return NULL;
    }

    memcpy(c_str, string, len);
    c_str[len] = '\0';
    string_builder->c_str = c_str;

    return string_builder;
}

StringBuilder *cr_std_string_builder_newf(Arena *arena, const char *format, ...) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_string_builder_newf -> arena* was NULL");
        return NULL;
    }

    if (!format) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_builder_newf -> char* input was NULL");
        return NULL;
    }

    StringBuilder *string_builder = cr_std_arena_alloc(arena, sizeof(*string_builder));

    if (!string_builder) {
        cr_std_logger_out(
        CR_STD_LOGGER_LOG_TYPE_ERROR,
        "cr_std_string_builder_newf -> failed to allocate memory for new StringBuilder struct");
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
        return NULL;
    }

    char *c_str = cr_std_arena_alloc(arena, string_builder->capacity + 1);
    if (!c_str) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_builder_newf -> failed to allocate memory for buffer");
        return NULL;
    }

    va_start(args, format);
    vsnprintf(c_str, string_builder->size + 1, format, args);
    va_end(args);

    string_builder->c_str = c_str;

    return string_builder;
}

int cr_std_string_builder_ensure_capacity(Arena *arena,
                                          StringBuilder *string_builder,
                                          size_t additional) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_string_builder_ensure_capacity -> arena* is NULL");
        return 1;
    }

    if (!string_builder) {
        CR_LOG_ERROR("cr_std_string_builder_ensure_capacity -> StringBuilder is NULL");
        return 1;
    }

    size_t needed = string_builder->size + additional + 1;
    if (needed <= string_builder->capacity) {
        return 0;
    }

    size_t new_capacity = string_builder->capacity * 2;
    if (new_capacity < needed) {
        new_capacity = needed;
    }

    char *new_str = cr_std_arena_alloc(arena, new_capacity);
    if (!new_str) {
        cr_std_logger_out(
        CR_STD_LOGGER_LOG_TYPE_ERROR,
        "cr_std_string_builder_ensure_capacity -> failed to allocate memory for new buffer");
        return 1;
    }

    if (string_builder->c_str) {
        memcpy(new_str, string_builder->c_str, string_builder->size + 1);
    } else {
        new_str[0] = '\0';
    }

    string_builder->c_str = new_str;
    string_builder->capacity = new_capacity;
    return 0;
}

int cr_std_string_builder_append_string(Arena *arena,
                                        StringBuilder *string_builder,
                                        const char *string) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_string_builder_append_string -> arena* was NULL");
        return 1;
    }

    if (!string_builder) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_builder_append_string -> given string builder is NULL");
        return 1;
    }

    size_t string_to_append_length = strlen(string);
    if (string_builder->size + string_to_append_length >= string_builder->capacity) {

        size_t new_capacity = (string_builder->size + string_to_append_length) * 2;
        void *temp = cr_std_arena_alloc(arena, new_capacity);
        if (!temp) {
            cr_std_logger_out(
            CR_STD_LOGGER_LOG_TYPE_ERROR,
            "cr_std_string_builder_append_string -> failed to allocate memory for "
            "StringBuilder->c_str");
            return 1;
        }
        if (string_builder->c_str) {
            memcpy(temp, string_builder->c_str, string_builder->size + 1);
        }

        string_builder->c_str = temp;
        string_builder->capacity = new_capacity;
    }

    memcpy(string_builder->c_str + string_builder->size, string, string_to_append_length);

    string_builder->size += string_to_append_length;
    string_builder->c_str[string_builder->size] = '\0';
    return 0;
}

int cr_std_string_builder_append_char(Arena *arena, StringBuilder *string_builder, char ch) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_string_builder_append_char -> arena* was NULL");
        return 1;
    }

    if (!string_builder) {
        CR_LOG_ERROR("cr_std_string_builder_append_char -> given string builder is NULL");
        return 1;
    }

    if (string_builder->size + 1 >= string_builder->capacity) {
        size_t new_capacity = (string_builder->size + 1) * 2;
        char *temp = cr_std_arena_alloc(arena, new_capacity);
        if (!temp) {
            CR_LOG_ERROR(
            "cr_std_string_builder_append_char -> failed to allocate memory from arena");
            return 1;
        }

        if (string_builder->c_str) {
            memcpy(temp, string_builder->c_str, string_builder->size + 1);
        }

        string_builder->c_str = temp;
        string_builder->capacity = new_capacity;
    }

    string_builder->c_str[string_builder->size] = ch;
    string_builder->size += 1;
    string_builder->c_str[string_builder->size] = '\0';

    return 0;
}

int cr_std_string_builder_appendf(Arena *arena,
                                  StringBuilder *string_builder,
                                  const char *format,
                                  ...) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_string_builder_appendf -> arena* was NULL");
        return 1;
    }

    if (!string_builder) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_builder_appendf -> given string builder is NULL");
        return 1;
    }

    va_list args;
    va_start(args, format);
    size_t string_to_append_length = vsnprintf(NULL, 0, format, args);
    va_end(args);

    char *buffer = cr_std_arena_alloc(arena, string_to_append_length + 1);
    if (!buffer) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_builder_appendf -> memory allocation failed for buffer");
        return 1;
    }

    va_start(args, format);
    vsnprintf(buffer, string_to_append_length + 1, format, args);
    va_end(args);

    buffer[string_to_append_length] = '\0';
    return cr_std_string_builder_append_string(arena, string_builder, buffer);
}

int cr_std_string_builder_append_null_terminated(Arena *arena, StringBuilder *string_builder, ...) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_string_builder_append_null_terminated -> arena* was NULL");
        return 1;
    }

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
        int result = cr_std_string_builder_append_string(arena, string_builder, current_string);
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

    if (string_builder->c_str) {
        string_builder->c_str[0] = '\0';
        string_builder->size = 0;
    }
    return 0;
}

String *cr_std_string_builder_to_string(Arena *arena, StringBuilder *string_builder) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_string_builder_to_string -> arena* was NULL");
        return NULL;
    }

    if (!string_builder) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_builder_to_string -> given string builder is NULL");
        return NULL;
    }
    return cr_std_string_new(arena, string_builder->c_str);
}

String *cr_std_string_new(Arena *arena, const char *string) {
    if (!string) {
        CR_LOG_ERROR("cr_std_string_new -> char* was NULL");
        return NULL;
    }

    if (!arena) {
        CR_LOG_ERROR("cr_std_string_new -> arena* was NULL");
        return NULL;
    }

    String *new_string = cr_std_arena_alloc(arena, sizeof(*new_string));
    if (!new_string) {
        CR_LOG_ERROR("cr_std_string_new -> arena* allocation failed for String struct");
        return NULL;
    }

    new_string->length = strlen(string);

    char *c_str = cr_std_arena_alloc(arena, new_string->length + 1);
    if (!c_str) {
        CR_LOG_ERROR("cr_std_string_new -> arena* allocation failed for c_str");
        return NULL;
    }

    memcpy(c_str, string, new_string->length);
    c_str[new_string->length] = '\0';
    new_string->c_str = c_str;

    return new_string;
}

String *cr_std_string_newf(Arena *arena, const char *format, ...) {
    if (!format) {
        CR_LOG_ERROR("cr_std_string_newf -> char* was NULL");
        return NULL;
    }

    if (!arena) {
        CR_LOG_ERROR("cr_std_string_newf -> arena* was NULL");
        return NULL;
    }

    String *new_string = cr_std_arena_alloc(arena, sizeof(*new_string));
    if (!new_string) {
        CR_LOG_ERROR("cr_std_string_newf -> arena* allocation failed for String struct");
        return NULL;
    }

    va_list args;
    va_start(args, format);

    new_string->length = vsnprintf(NULL, 0, format, args);
    va_end(args);

    if (new_string->length < 0) {
        CR_LOG_ERROR("cr_std_string_newf -> vsnprintf failed during length calculation");
        return NULL;
    }

    char *c_str = cr_std_arena_alloc(arena, new_string->length + 1);
    if (!c_str) {
        CR_LOG_ERROR("cr_std_string_newf -> arena* allocation failed for c_str");
        return NULL;
    }

    va_start(args, format);
    vsnprintf(c_str, new_string->length + 1, format, args);
    va_end(args);

    new_string->c_str = c_str;

    return new_string;
}

String *cr_std_string_make_copy(Arena *arena, String *src_string) {
    if (!src_string) {
        CR_LOG_ERROR("cr_std_string_make_copy -> src_string* was NULL");
        return NULL;
    }

    if (!arena) {
        CR_LOG_ERROR("cr_std_string_make_copy -> arena* was NULL");
        return NULL;
    }

    return cr_std_string_new(arena, src_string->c_str);
}

int cr_std_string_concat_null_terminated(Arena *arena, String *string, ...) {
    if (!string) {
        CR_LOG_ERROR("cr_std_string_concat_null_terminated -> string pointer is NULL");
        return 1;
    }

    if (!arena) {
        CR_LOG_ERROR("cr_std_string_concat_null_terminated -> arena* was NULL");
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

    char *new_c_str = cr_std_arena_alloc(arena, total_length + 1);
    if (!new_c_str) {
        CR_LOG_ERROR("cr_std_string_concat_null_terminated -> arena* allocation failed for c_str");
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

int cr_std_string_trim(Arena *arena, String *string, int direction) {
    if (string == NULL || string->c_str == NULL || string->length == 0) {
        CR_LOG_ERROR("cr_std_string_trim -> tried to trim an invalid string");
        return 1;
    }

    if (!arena) {
        CR_LOG_ERROR("cr_std_string_trim -> arena* was NULL");
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
    char *new_str = cr_std_arena_alloc(arena, new_length + 1);
    if (!new_str) {
        CR_LOG_ERROR("cr_std_string_trim -> arena* allocation failed for c_str");
        return 1;
    }

    memcpy(new_str, string->c_str + start, new_length);
    new_str[new_length] = '\0';

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

Vector *cr_std_string_split(Arena *arena, String *string, char delimiter) {
    if (!string) {
        CR_LOG_ERROR("cr_std_string_split -> string pointer is NULL");
        return NULL;
    }

    if (!arena) {
        CR_LOG_ERROR("cr_std_string_split -> arena* was NULL");
        return NULL;
    }

    Vector *vector = cr_std_vector_new(arena);

    char *buffer = cr_std_arena_alloc(arena, string->length + 1);
    if (!buffer) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_split -> memory allocation failed");
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
                String *new_string = cr_std_string_new(arena, buffer);
                cr_std_vector_push_back(arena, vector, new_string);
            }

            buffer_index = 0;
        } else {
            buffer[buffer_index] = string->c_str[i];
            buffer_index++;
        }
    }
    return vector;
}

Vector *cr_std_string_split_hard(Arena *arena, String *string, char delimiter) {
    if (!string) {
        CR_LOG_ERROR("cr_std_string_split_hard -> string pointer is NULL");
        return NULL;
    }

    if (!arena) {
        CR_LOG_ERROR("cr_std_string_split_hard -> arena* was NULL");
        return NULL;
    }

    Vector *vector = cr_std_vector_new(arena);

    char *buffer = cr_std_arena_alloc(arena, string->length + 1);
    if (!buffer) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_split_hard -> memory allocation failed");
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

            String *new_string = cr_std_string_new(arena, buffer);
            cr_std_vector_push_back(arena, vector, new_string);

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

int cr_std_string_replace_string(Arena *arena, String *string, const char *from, const char *to) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_replace_string -> string pointer is NULL");
        return 0;
    }

    if (!arena) {
        CR_LOG_ERROR("cr_std_string_replace_string -> arena* was NULL");
        return 0;
    }

    int occurrences = cr_std_string_contains_string(string, from);
    if (occurrences == 0) {
        return 0;
    }

    int from_length = strlen(from);
    int to_length = strlen(to);

    int new_length = string->length + (to_length - from_length) * occurrences;
    char *new_string = cr_std_arena_alloc(arena, new_length + 1);
    if (!new_string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_replace_string -> arena allocation failed for new_string");
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

    string->c_str = new_string;
    string->length = new_length;

    return successfully_changed_words;
}

int cr_std_string_remove_non_numeric(Arena *arena, String *string) {
    if (!string || !string->c_str) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_remove_non_numeric -> string pointer is NULL");
        return 1;
    }

    if (!arena) {
        CR_LOG_ERROR("cr_std_string_remove_non_numeric -> arena* was NULL");
        return 1;
    }

    size_t digit_count = 0;
    for (size_t i = 0; i < string->length; ++i) {
        if (isdigit((unsigned char)string->c_str[i])) {
            digit_count++;
        }
    }

    char *filtered_str = cr_std_arena_alloc(arena, digit_count + 1);
    if (!filtered_str) {
        cr_std_logger_out(
        CR_STD_LOGGER_LOG_TYPE_ERROR,
        "cr_std_string_remove_non_numeric -> arena allocation failed for filtered_str");
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

    string->c_str = filtered_str;
    string->length = digit_count;
    return 0;
}

int cr_std_string_remove_numeric(Arena *arena, String *string) {
    if (!string || !string->c_str) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_remove_numeric -> string pointer is NULL");
        return 1;
    }

    if (!arena) {
        CR_LOG_ERROR("cr_std_string_replace_string -> arena* was NULL");
        return 1;
    }

    size_t new_char_count = 0;
    for (size_t i = 0; i < string->length; ++i) {
        if (!isdigit((unsigned char)string->c_str[i])) {
            new_char_count++;
        }
    }

    char *filtered_str = cr_std_arena_alloc(arena, new_char_count + 1);
    if (!filtered_str) {
        cr_std_logger_out(
        CR_STD_LOGGER_LOG_TYPE_ERROR,
        "cr_std_string_remove_numeric -> arena allocation failed for filtered_str");
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

    string->c_str = filtered_str;
    string->length = new_char_count;
    return 0;
}

int cr_std_string_to_int(String *string) {
    if (!string) {
        CR_LOG_ERROR("cr_std_string_to_int -> string pointer is NULL");
        return 0;
    }

    int result = 0;
    int found_digit = 0;

    for (size_t i = 0; i < string->length; i++) {
        char c = string->c_str[i];

        if (isdigit((unsigned char)c)) {
            int digit = c - '0';

            result = result * 10 + digit;
            found_digit = 1;
        }
    }

    return found_digit ? result : 0;
}

String *cr_std_string_from_int(Arena *arena, int number) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_string_from_int -> arena* was NULL");
        return NULL;
    }

    char str_buffer[50];
    int result = snprintf(str_buffer, sizeof(str_buffer), "%d", number);

    if (result < 0 || result >= sizeof(str_buffer)) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_from_int -> snprintf failed");
        return cr_std_string_new(arena, "");
    }

    String *string = cr_std_string_new(arena, str_buffer);
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_from_int -> failed to create String");
        return cr_std_string_new(arena, "");
    }
    return string;
}

String *cr_std_string_sub_string(Arena *arena, String *string, int start_index, int end_index) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_string_sub_string -> arena* was NULL");
        return NULL;
    }

    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_WARNING,
                          "cr_std_string_sub_string -> given string is NULL");
        return cr_std_string_new(arena, "");
    }

    if (string->length == 0) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_WARNING,
                          "cr_std_string_sub_string -> given string is empty");
        return cr_std_string_new(arena, "");
    }

    if (start_index < 0) {
        start_index = 0;
    }

    if (end_index < start_index) {
        return cr_std_string_new(arena, "");
    }

    if (end_index > string->length) {
        end_index = string->length;
    }

    size_t sub_length = end_index - start_index;

    char *sub_data = cr_std_arena_alloc(arena, sub_length + 1);
    if (!sub_data) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_sub_string -> failed to allocate memory for substring");
        return cr_std_string_new(arena, "");
    }

    strncpy(sub_data, string->c_str + start_index, sub_length);
    sub_data[sub_length] = '\0';
    return cr_std_string_new(arena, sub_data);
}

String *cr_std_string_color_string(Arena *arena, String *string, int color_code) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_string_color_string -> arena* was NULL");
        return NULL;
    }

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
    return cr_std_string_newf(arena, CR_STD_STRING_ANSI_COLOR_ESCAPE_SEQ, color_code,
                              string->c_str);
}

String *
cr_std_string_color_phrase(Arena *arena, String *string, const char *phrase, int color_code) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_string_color_phrase -> arena* was NULL");
        return NULL;
    }

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

    String *str_copy = cr_std_string_make_copy(arena, string);
    String *phrase_colored =
    cr_std_string_newf(arena, CR_STD_STRING_ANSI_COLOR_ESCAPE_SEQ, color_code, phrase);
    cr_std_string_replace_string(arena, str_copy, phrase, phrase_colored->c_str);
    return str_copy;
}

int cr_std_string_color_strip(Arena *arena, String *string) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_string_color_strip -> arena* was NULL");
        return 1;
    }

    if (!string || !string->c_str) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_color_strip -> string is NULL");
        return 1;
    }

    for (int i = 30; i < 40; i++) {
        char color_code[16];
        snprintf(color_code, sizeof(color_code), "\033[%dm", i);
        cr_std_string_replace_string(arena, string, color_code, "");
    }

    cr_std_string_replace_string(arena, string, "\033[0m", "");
    return 0;
}

String *cr_std_string_repeat(Arena *arena, const char *string, size_t n) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_string_repeat -> arena* was NULL");
        return NULL;
    }

    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_string_repeat -> string is NULL");
        return cr_std_string_new(arena, "");
    }

    if (n == 0) {
        return cr_std_string_new(arena, "");
    }

    size_t len = strlen(string);
    if (len > SIZE_MAX / n) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_repeat -> string is too large (n might be negative)");
        return cr_std_string_new(arena, "");
    }
    size_t total_length = len * n;

    StringBuilder *sb = cr_std_string_builder_new(arena, "");
    cr_std_string_builder_ensure_capacity(arena, sb, total_length);
    for (size_t i = 0; i < n; i++) {
        cr_std_string_builder_append_string(arena, sb, string);
    }

    return cr_std_string_builder_to_string(arena, sb);
}
