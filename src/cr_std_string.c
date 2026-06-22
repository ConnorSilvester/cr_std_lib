#include "cr_std_string.h"
#include "cr_std_arena.h"
#include "cr_std_logger.h"
#include "cr_std_utils.h"
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
    return cr_std_string_builder_newc(arena, string, 0);
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

b8 cr_std_string_builder_ensure_capacity(Arena *arena,
                                         StringBuilder *string_builder,
                                         size_t additional) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_string_builder_ensure_capacity -> arena* is NULL");
        return CR_STD_FAIL;
    }

    if (!string_builder) {
        CR_LOG_ERROR("cr_std_string_builder_ensure_capacity -> StringBuilder is NULL");
        return CR_STD_FAIL;
    }

    size_t needed = string_builder->size + additional + 1;
    if (needed <= string_builder->capacity) {
        return CR_STD_OK;
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
        return CR_STD_FAIL;
    }

    if (string_builder->c_str) {
        memcpy(new_str, string_builder->c_str, string_builder->size + 1);
    } else {
        new_str[0] = '\0';
    }

    string_builder->c_str = new_str;
    string_builder->capacity = new_capacity;
    return CR_STD_OK;
}

b8 cr_std_string_builder_append_string(Arena *arena,
                                       StringBuilder *string_builder,
                                       const char *string) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_string_builder_append_string -> arena* was NULL");
        return CR_STD_FAIL;
    }

    if (!string_builder) {
        CR_LOG_ERROR("cr_std_string_builder_append_string -> given string builder is NULL");
        return CR_STD_FAIL;
    }

    if (!string) {
        CR_LOG_ERROR("cr_std_string_builder_append_string -> string is NULL");
        return CR_STD_FAIL;
    }

    size_t len = strlen(string);
    if (len == 0) {
        return CR_STD_OK;
    }

    if (cr_std_string_builder_ensure_capacity(arena, string_builder, len) != CR_STD_OK) {
        return CR_STD_FAIL;
    }

    memcpy(string_builder->c_str + string_builder->size, string, len);
    string_builder->size += len;
    string_builder->c_str[string_builder->size] = '\0';

    return CR_STD_OK;
}

b8 cr_std_string_builder_append_char(Arena *arena, StringBuilder *string_builder, char ch) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_string_builder_append_char -> arena* was NULL");
        return CR_STD_FAIL;
    }

    if (!string_builder) {
        CR_LOG_ERROR("cr_std_string_builder_append_char -> given string builder is NULL");
        return CR_STD_FAIL;
    }

    if (cr_std_string_builder_ensure_capacity(arena, string_builder, 1) != CR_STD_OK) {
        CR_LOG_ERROR("cr_std_string_builder_append_char -> failed to ensure capacity");
        return CR_STD_FAIL;
    }

    string_builder->c_str[string_builder->size] = ch;
    string_builder->size++;
    string_builder->c_str[string_builder->size] = '\0';

    return CR_STD_OK;
}

b8 cr_std_string_builder_appendf(Arena *arena,
                                 StringBuilder *string_builder,
                                 const char *format,
                                 ...) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_string_builder_appendf -> arena* was NULL");
        return CR_STD_FAIL;
    }

    if (!string_builder) {
        CR_LOG_ERROR("cr_std_string_builder_appendf -> given string builder is NULL");
        return CR_STD_FAIL;
    }

    va_list args;
    va_start(args, format);
    int len = vsnprintf(NULL, 0, format, args);
    va_end(args);

    if (len < 0) {
        CR_LOG_ERROR("cr_std_string_builder_appendf -> vsnprintf failed");
        return CR_STD_FAIL;
    }

    if (cr_std_string_builder_ensure_capacity(arena, string_builder, len) != CR_STD_OK) {
        CR_LOG_ERROR("cr_std_string_builder_appendf -> failed to ensure capacity");
        return CR_STD_FAIL;
    }

    va_start(args, format);
    vsnprintf(string_builder->c_str + string_builder->size, len + 1, format, args);
    va_end(args);

    string_builder->size += len;
    return CR_STD_OK;
}

b8 cr_std_string_builder_append_null_terminated(Arena *arena, StringBuilder *string_builder, ...) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_string_builder_append_null_terminated -> arena* was NULL");
        return CR_STD_FAIL;
    }

    if (!string_builder) {
        cr_std_logger_out(
        CR_STD_LOGGER_LOG_TYPE_ERROR,
        "cr_std_string_builder_append_null_terminated -> string builder pointer is NULL");
        return CR_STD_FAIL;
    }

    va_list args;
    va_start(args, string_builder);

    char *current_string;
    while ((current_string = va_arg(args, char *)) != NULL) {
        if (cr_std_string_builder_append_string(arena, string_builder, current_string) !=
            CR_STD_OK) {
            va_end(args);
            return CR_STD_FAIL;
        }
    }
    va_end(args);
    return CR_STD_OK;
}

b8 cr_std_string_builder_reset(StringBuilder *string_builder) {
    if (!string_builder) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_builder_reset -> given string builder is NULL");
        return CR_STD_FAIL;
    }

    if (string_builder->c_str) {
        string_builder->c_str[0] = '\0';
        string_builder->size = 0;
    }
    return CR_STD_OK;
}

b8 cr_std_string_builder_reset_to_mark(StringBuilder *string_builder, size_t mark) {
    if (!string_builder) {
        CR_LOG_ERROR("cr_std_string_builder_reset_to_mark -> given string builder is NULL");
        return CR_STD_FAIL;
    }

    if (mark > string_builder->size) {
        CR_LOG_ERROR_FMT("cr_std_string_builder_reset_to_mark -> mark %zu > size %zu", mark,
                         string_builder->size);
        return CR_STD_FAIL;
    }

    if (string_builder->c_str) {
        string_builder->size = mark;
        string_builder->c_str[mark] = '\0';
        return CR_STD_OK;
    }

    CR_LOG_ERROR("cr_std_string_builder_reset_to_mark -> c_str is NULL");
    return CR_STD_FAIL;
}

b8 cr_std_string_builder_get_mark(StringBuilder *string_builder, size_t *mark) {
    if (!string_builder) {
        CR_LOG_ERROR("cr_std_string_builder_get_mark -> given string builder* is NULL");
        return CR_STD_FAIL;
    }

    if (!mark) {
        CR_LOG_ERROR("cr_std_string_builder_get_mark -> given mark* is NULL");
        return CR_STD_FAIL;
    }

    *mark = string_builder->size;
    return CR_STD_OK;
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
        CR_LOG_ERROR("cr_std_string_new -> arena allocation failed for String struct");
        return NULL;
    }

    size_t len = strlen(string);
    char *c_str = cr_std_arena_alloc(arena, len + 1);
    if (!c_str) {
        CR_LOG_ERROR("cr_std_string_new -> arena allocation failed for string data");
        return NULL;
    }

    memcpy(c_str, string, len + 1);
    new_string->c_str = c_str;
    new_string->length = len;

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

b8 cr_std_string_concat_null_terminated(Arena *arena, String *string, ...) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_string_concat_null_terminated -> arena* was NULL");
        return CR_STD_FAIL;
    }

    if (!string) {
        CR_LOG_ERROR("cr_std_string_concat_null_terminated -> string* is NULL");
        return CR_STD_FAIL;
    }

    va_list args;
    va_start(args, string);

    size_t num_strings = 0;
    const char *current_string;

    size_t total_length = string->length;
    while ((current_string = va_arg(args, const char *)) != NULL) {
        total_length += strlen(current_string);
        num_strings++;
    }
    va_end(args);

    if (num_strings == 0) {
        return CR_STD_OK;
    }

    char *new_c_str = cr_std_arena_alloc(arena, total_length + 1);
    if (!new_c_str) {
        CR_LOG_ERROR("cr_std_string_concat_null_terminated -> allocation failed");
        return CR_STD_FAIL;
    }

    size_t pos = 0;
    memcpy(new_c_str, string->c_str, string->length);
    pos += string->length;

    va_start(args, string);
    while ((current_string = va_arg(args, const char *)) != NULL) {
        size_t len = strlen(current_string);
        memcpy(new_c_str + pos, current_string, len);
        pos += len;
    }
    va_end(args);

    new_c_str[pos] = '\0';

    string->c_str = new_c_str;
    string->length = total_length;

    return CR_STD_OK;
}

i32 cr_std_string_compare(String *arg, String *arg1) {
    if (!arg || !arg1) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_compare -> string pointer is NULL");
        return CR_STD_STRING_COMPARE_ERROR;
    }

    // First string is longer
    if (arg->length > arg1->length) {
        return CR_STD_STRING_ARG_LONGER;
    }
    // Second string is longer
    if (arg->length < arg1->length) {
        return CR_STD_STRING_ARG1_LONGER;
    }
    for (int i = 0; i < arg->length; i++) {
        if (arg->c_str[i] != arg1->c_str[i]) {
            // Partial equality ( length is equal but not chars )
            return CR_STD_STRING_DIFFERENT;
        }
    }
    // Fully equal
    return CR_STD_STRING_EQUAL;
}

i32 cr_std_string_compare_c_str(String *arg, const char *arg1) {
    if (!arg || !arg1) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_compare_c_str -> pointer is NULL");
        return CR_STD_STRING_COMPARE_ERROR;
    }

    // First string is longer
    size_t arg1_length = strlen(arg1);
    if (arg->length > arg1_length) {
        return CR_STD_STRING_ARG_LONGER;
    }
    // Second string is longer
    if (arg->length < arg1_length) {
        return CR_STD_STRING_ARG1_LONGER;
    }
    for (int i = 0; i < arg->length; i++) {
        if (arg->c_str[i] != arg1[i]) {
            // Partial equality ( length is equal but not chars )
            return CR_STD_STRING_DIFFERENT;
        }
    }
    // Fully equal
    return CR_STD_STRING_EQUAL;
}

b8 cr_std_string_trim(String *string, int direction) {
    if (!string || !string->c_str || string->length == 0) {
        CR_LOG_ERROR("cr_std_string_trim -> invalid string");
        return CR_STD_FAIL;
    }

    size_t start = 0;
    size_t end = string->length - 1;

    // Trim left
    if (direction == CR_STD_STRING_TRIM_LEFT || direction == CR_STD_STRING_TRIM_BOTH) {
        while (start <= end && isspace((unsigned char)string->c_str[start])) {
            start++;
        }
    }

    // Trim right
    if (direction == CR_STD_STRING_TRIM_RIGHT || direction == CR_STD_STRING_TRIM_BOTH) {
        while (end > start && isspace((unsigned char)string->c_str[end])) {
            end--;
        }
    }

    size_t new_length = end - start + 1;
    if (start > 0 && start < string->length) {
        memmove(string->c_str, string->c_str + start, new_length);
    }

    string->c_str[new_length] = '\0';
    string->length = new_length;

    return CR_STD_OK;
}

i32 cr_std_string_find_char(String *string, char ch) {
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

i32 cr_std_string_find_char_n(String *string, char ch, int n) {
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

i32 cr_std_string_find_char_last(String *string, char ch) {
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

i32 cr_std_string_find_string(String *string, const char *phrase) {
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

i32 cr_std_string_find_string_n(String *string, const char *phrase, int n) {
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

i32 cr_std_string_find_string_last(String *string, const char *phrase) {
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

i32 cr_std_string_contains_string(String *string, const char *phrase) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_contains_string -> string pointer is NULL");
        return 0;
    }

    if (string->length == 0) {
        return 0;
    }

    if (!phrase || phrase[0] == '\0') {
        return 0;
    }

    int occurrences = 0;
    const char *pos = string->c_str;
    size_t phrase_len = strlen(phrase);

    if (phrase_len > string->length) {
        return 0;
    }

    while ((pos = strstr(pos, phrase)) != NULL) {
        occurrences++;
        pos += phrase_len;
    }

    return occurrences;
}

i32 cr_std_string_contains_char(String *string, char ch) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_contains_char -> string pointer is NULL");
        return 0;
    }

    if (string->length == 0) {
        return 0;
    }

    int occurrences = 0;
    const char *pos = string->c_str;

    while ((pos = strchr(pos, ch)) != NULL) {
        occurrences++;
        pos++;
    }

    return occurrences;
}

b8 cr_std_string_starts_with_string(String *string, const char *prefix) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_starts_with_string -> string pointer is NULL");
        return false;
    }

    int prefix_length = strlen(prefix);
    if (prefix_length > string->length || prefix_length < 1) {
        return false;
    }

    for (int i = 0; i < prefix_length; i++) {
        if (string->c_str[i] != prefix[i]) {
            return false;
        }
    }

    return true;
}

b8 cr_std_string_ends_with_string(String *string, const char *suffix) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_ends_with_string -> string pointer is NULL");
        return false;
    }

    int suffix_length = strlen(suffix);
    if (suffix_length > string->length || suffix_length < 1) {
        return false;
    }

    int start_index = string->length - suffix_length;

    for (int i = 0; i < suffix_length; i++) {
        if (string->c_str[start_index + i] != suffix[i]) {
            return false;
        }
    }

    return true;
}

b8 cr_std_string_starts_with_char(String *string, char prefix) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_starts_with_char -> string pointer is NULL");
        return false;
    }

    if (string->length < 1) {
        return false;
    }

    return (string->c_str[0] == prefix) ? true : false;
}

b8 cr_std_string_ends_with_char(String *string, char suffix) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_ends_with_char -> string pointer is NULL");
        return false;
    }

    if (string->length < 1) {
        return false;
    }

    return (string->c_str[string->length - 1] == suffix) ? true : false;
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

b8 cr_std_string_hash_code(String *string, u64 *hash) {
    if (!string || !hash) {
        CR_LOG_ERROR("cr_std_string_hash_code -> NULL parameter");
        return CR_STD_FAIL;
    }

    u64 result = 5381;
    for (size_t i = 0; i < string->length; i++) {
        result = ((result << 5) + result) + (u8)string->c_str[i];
    }
    *hash = result;
    return CR_STD_OK;
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

    Arena *temp_arena = cr_std_arena_new(string->length + 128);
    char *buffer = cr_std_arena_alloc(temp_arena, string->length + 1);
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
    cr_std_arena_free(&temp_arena);
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

    Arena *temp_arena = cr_std_arena_new(string->length + 128);
    char *buffer = cr_std_arena_alloc(temp_arena, string->length + 1);
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
    cr_std_arena_free(&temp_arena);
    return vector;
}

b8 cr_std_string_to_upper(String *string) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_to_upper -> string pointer is NULL");
        return CR_STD_FAIL;
    }
    for (int i = 0; i < string->length; i++) {
        string->c_str[i] = toupper(string->c_str[i]);
    }
    return CR_STD_OK;
}

b8 cr_std_string_to_lower(String *string) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_to_lower -> string pointer is NULL");
        return CR_STD_FAIL;
    }
    for (int i = 0; i < string->length; i++) {
        string->c_str[i] = tolower(string->c_str[i]);
    }
    return CR_STD_OK;
}

b8 cr_std_string_to_title(String *string) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_to_title -> string pointer is NULL");
        return CR_STD_FAIL;
    }
    if (string->length < 1) {
        return CR_STD_FAIL;
    }

    b8 new_word = true;
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

    return CR_STD_OK;
}

i32 cr_std_string_replace_string_inplace(String *string, const char *from, const char *to) {
    if (!string || !string->c_str || !from || !to) {
        CR_LOG_ERROR("cr_std_string_replace_string_inplace -> NULL parameter");
        return 0;
    }

    if (string->length == 0 || from[0] == '\0') {
        return 0;
    }

    size_t from_len = strlen(from);
    size_t to_len = strlen(to);

    if (to_len > from_len) {
        return 0;
    }

    int occurrences = 0;
    const char *pos = string->c_str;
    while ((pos = strstr(pos, from)) != NULL) {
        occurrences++;
        pos += from_len;
    }

    if (occurrences == 0) return 0;

    char *src = string->c_str;
    char *dest = string->c_str;
    size_t len_diff = from_len - to_len;
    size_t new_len = string->length - (len_diff * occurrences);

    while (*src) {
        if (strncmp(src, from, from_len) == 0) {
            memcpy(dest, to, to_len);
            dest += to_len;
            src += from_len;
        } else {
            *dest++ = *src++;
        }
    }

    *dest = '\0';
    string->length = new_len;

    return occurrences;
}

i32 cr_std_string_replace_string(Arena *arena, String *string, const char *from, const char *to) {
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

    if (to_length <= from_length) {
        return cr_std_string_replace_string_inplace(string, from, to);
    }

    if (!arena) {
        CR_LOG_ERROR("cr_std_string_replace_string -> arena* was NULL");
        return 0;
    }

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

b8 cr_std_string_remove_non_numeric(String *string) {
    if (!string || !string->c_str) {
        CR_LOG_ERROR("cr_std_string_remove_non_numeric -> NULL parameter");
        return CR_STD_FAIL;
    }

    if (string->length == 0) {
        return CR_STD_OK;
    }

    char *src = string->c_str;
    char *dest = string->c_str;
    size_t new_len = 0;

    while (*src) {
        if (isdigit((unsigned char)*src)) {
            *dest = *src;
            dest++;
            new_len++;
        }
        src++;
    }

    *dest = '\0';
    string->length = new_len;

    return CR_STD_OK;
}

b8 cr_std_string_remove_numeric(String *string) {
    if (!string || !string->c_str) {
        CR_LOG_ERROR("cr_std_string_remove_numeric -> NULL parameter");
        return CR_STD_FAIL;
    }

    if (string->length == 0) {
        return CR_STD_OK;
    }

    char *write = string->c_str;
    const char *read = string->c_str;
    size_t new_len = 0;

    while (*read) {
        if (!isdigit((unsigned char)*read)) {
            *write++ = *read;
            new_len++;
        }
        read++;
    }

    *write = '\0';
    string->length = new_len;

    return CR_STD_OK;
}

b8 cr_std_string_to_int(String *string, i32 *number) {
    if (!string) {
        CR_LOG_ERROR("cr_std_string_to_int -> string pointer is NULL");
        return CR_STD_FAIL;
    }

    i32 result = 0;
    b8 found_digit = false;

    for (size_t i = 0; i < string->length; i++) {
        char c = string->c_str[i];

        if (isdigit((unsigned char)c)) {
            i32 digit = c - '0';

            if (result > (INT32_MAX - digit) / 10) {
                CR_LOG_ERROR("cr_std_string_to_int -> overflow");
                return CR_STD_FAIL;
            }

            result = result * 10 + digit;
            found_digit = true;
        }
    }

    if (!found_digit) {
        return CR_STD_FAIL;
    }

    *number = result;
    return CR_STD_OK;
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
        return cr_std_string_new(arena, "0");
    }

    String *string = cr_std_string_new(arena, str_buffer);
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_from_int -> failed to create String");
        return cr_std_string_new(arena, "0");
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
        color_code = CR_STD_STRING_COLOR_NONE;
    }

    size_t phrase_len = strlen(phrase);
    size_t needed = phrase_len + 32;

    if (needed < 256) {
        char colored_phrase[256];
        snprintf(colored_phrase, sizeof(colored_phrase), CR_STD_STRING_ANSI_COLOR_ESCAPE_SEQ,
                 color_code, phrase);

        String *str_copy = cr_std_string_make_copy(arena, string);
        if (str_copy) {
            cr_std_string_replace_string(arena, str_copy, phrase, colored_phrase);
        }
        return str_copy;
    } else {
        Arena *temp_arena = cr_std_arena_new(needed + 16);
        char *colored_phrase = cr_std_arena_alloc(temp_arena, needed + 1);
        snprintf(colored_phrase, needed + 1, CR_STD_STRING_ANSI_COLOR_ESCAPE_SEQ, color_code,
                 phrase);

        String *str_copy = cr_std_string_make_copy(arena, string);
        cr_std_string_replace_string(arena, str_copy, phrase, colored_phrase);
        cr_std_arena_free(&temp_arena);
        return str_copy;
    }
}

b8 cr_std_string_color_strip(String *string) {
    if (!string || !string->c_str) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_string_color_strip -> string is NULL");
        return CR_STD_FAIL;
    }

    for (int i = 30; i < 40; i++) {
        char color_code[16];
        snprintf(color_code, sizeof(color_code), "\033[%dm", i);
        cr_std_string_replace_string(NULL, string, color_code, "");
    }

    if (cr_std_string_replace_string(NULL, string, "\033[0m", "") > 0) {
        return CR_STD_OK;
    }

    return CR_STD_FAIL;
}

String *cr_std_string_repeat(Arena *arena, const char *string, size_t n) {
    if (!arena || !string) {
        CR_LOG_ERROR("cr_std_string_repeat -> NULL parameter");
        return NULL;
    }

    if (n <= 0 || string[0] == '\0') {
        return cr_std_string_new(arena, "");
    }

    size_t len = strlen(string);

    if (len > SIZE_MAX / n) {
        CR_LOG_ERROR_FMT("cr_std_string_repeat -> size overflow: %zu * %zu", len, n);
        return cr_std_string_new(arena, "");
    }

    size_t total_len = len * n;
    char *buffer = cr_std_arena_alloc(arena, total_len + 1);
    if (!buffer) {
        CR_LOG_ERROR("cr_std_string_repeat -> allocation failed");
        return cr_std_string_new(arena, "");
    }

    char *pos = buffer;
    for (size_t i = 0; i < n; i++) {
        memcpy(pos, string, len);
        pos += len;
    }
    *pos = '\0';

    String *result = cr_std_arena_alloc(arena, sizeof(String));
    if (!result) {
        CR_LOG_ERROR("cr_std_string_repeat -> failed to allocate String struct");
        return cr_std_string_new(arena, "");
    }

    result->c_str = buffer;
    result->length = total_len;
    return result;
}
