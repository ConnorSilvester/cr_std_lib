#include "cr_std_string.h"
#include "cr_std_logger.h"
#include "cr_std_vector.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

string_t *cr_std_string_new(const char *format, ...) {
    string_t *string = (string_t *)malloc(sizeof(string_t));

    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_string_new -> failed to allocate memory for new string_t struct");
        return NULL;
    }

    va_list args;
    va_start(args, format);

    string->length = vsnprintf(NULL, 0, format, args);

    if (string->length < 0) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_string_new -> vsnprintf failed during length calculation");
        cr_std_string_free(string);
        va_end(args);
        return NULL;
    }

    char *c_str = (char *)malloc(sizeof(char) * (string->length + 1));
    if (!c_str) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_string_new -> failed to allocate memory for buffer");
        cr_std_string_free(string);
        return NULL;
    }

    va_start(args, format);
    vsnprintf(c_str, string->length + 1, format, args);
    va_end(args);

    string->c_str = c_str;

    return string;
}

int cr_std_string_free(string_t *string) {
    if (string) {
        if (string->c_str) {
            free(string->c_str);
            string->c_str = NULL;
        }

        free(string);
        string = NULL;

        return 1;
    }
    return 0;
}

int cr_std_string_concat_null_terminated(string_t *string, ...) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_WARNING, "cr_std_string_concat_null_terminated -> String pointer is NULL");
        return 0;
    }

    va_list args;
    va_start(args, string);

    size_t total_length = string->length;
    char *current_char;
    while ((current_char = va_arg(args, char *)) != NULL) {
        size_t index = 0;
        while (current_char[index] != '\0') {
            index++;
        }
        total_length += index;
    }
    va_end(args);

    char *c_str = (char *)malloc(sizeof(char) * (total_length + 1));
    if (!c_str) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_string_concat_null_terminated -> Failed to allocate memory for new string_t struct");
        return 0;
    }

    size_t c_str_current_pos = 0;
    for (size_t i = 0; i < string->length; i++) {
        c_str[c_str_current_pos] = string->c_str[i];
        c_str_current_pos++;
    }

    va_start(args, string);
    while ((current_char = va_arg(args, char *)) != NULL) {
        size_t index = 0;
        while (current_char[index] != '\0') {
            c_str[c_str_current_pos] = current_char[index];
            c_str_current_pos++;
            index++;
        }
    }
    va_end(args);

    c_str[c_str_current_pos] = '\0';

    free(string->c_str);
    string->c_str = c_str;
    string->length = total_length;

    return 1;
}

int cr_std_string_compare(string_t *arg, string_t *arg1) {
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

int cr_std_string_trim(string_t *str, int direction) {
    if (str == NULL || str->c_str == NULL || str->length == 0) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_string_trim -> tried to trim an invalid string");
        return 0;
    }

    int start = 0;
    int end = str->length - 1;

    if (direction == CR_STD_STRING_TRIM_LEFT || direction == CR_STD_STRING_TRIM_BOTH) {
        while (start <= end && cr_std_string_is_char_white_space(str->c_str[start])) {
            start++;
        }
    }

    if (direction == CR_STD_STRING_TRIM_RIGHT || direction == CR_STD_STRING_TRIM_BOTH) {
        while (end >= start && cr_std_string_is_char_white_space(str->c_str[end])) {
            end--;
        }
    }

    int new_length = end - start + 1;
    if (new_length == 0) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_WARNING, "cr_std_string_trim -> new string is empty");
    }
    char *new_str = malloc(sizeof(char) * (new_length + 1));
    if (new_str == NULL) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_string_trim -> malloc failed to allocate memory for new string");
        return 0;
    }

    for (int i = 0; i < new_length; i++) {
        new_str[i] = str->c_str[start + i];
    }
    new_str[new_length] = '\0';

    free(str->c_str);
    str->c_str = new_str;
    str->length = new_length;

    return 1;
}

int cr_std_string_is_char_white_space(char c) {
    return c == ' ' ||
           c == '\n' ||
           c == '\t' ||
           c == '\r' ||
           c == '\v' ||
           c == '\f';
}

int cr_std_string_find_char(string_t *str, char ch) {
    for (int i = 0; i <= str->length; i++) {
        if (str->c_str[i] == ch) {
            return i;
        }
    }
    return -1;
}

int cr_std_string_find_char_last(string_t *str, char ch) {
    for (int i = str->length; i >= 0; i--) {
        if (str->c_str[i] == ch) {
            return i;
        }
    }
    return -1;
}

int cr_std_string_contains_string(string_t *str, char *phrase) {
    int phrase_length = 0;
    while (phrase[phrase_length] != '\0') {
        phrase_length++;
    }

    if (phrase_length > str->length) {
        return 0;
    }

    for (int string_index = 0; string_index <= str->length - phrase_length; string_index++) {
        int found = 1;
        for (int phrase_index = 0; phrase_index < phrase_length; phrase_index++) {
            if (str->c_str[string_index + phrase_index] != phrase[phrase_index]) {
                found = 0;
                break;
            }
        }
        if (found) {
            return 1;
        }
    }
    return 0;
}

int cr_std_string_contains_char(string_t *str, char ch) {
    for (int i = 0; i <= str->length; i++) {
        if (str->c_str[i] == ch) {
            return 1;
        }
    }
    return 0;
}

unsigned long cr_std_string_hash_code(string_t *str) {
    unsigned long hash = 5381; // Initial hash value

    for (size_t i = 0; i < str->length; i++) {
        hash = ((hash << 5) + hash) + (int)(str->c_str[i]); // hash * 33 + char
    }

    return hash;
}

vector_t *cr_std_string_split(string_t *str, char delimiter) {
    vector_t *vector = cr_std_vector_new(sizeof(string_t *));
    char buffer[str->length + 1];
    int buffer_index = 0;
    for (size_t i = 0; i < str->length; i++) {
        if (str->c_str[i] == delimiter || i == str->length - 1) {
            if (str->c_str[i] != delimiter) {
                buffer[buffer_index] = str->c_str[i];
                buffer_index++;
            }

            buffer[buffer_index] = '\0';

            // Avoid adding empty strings to the vector
            if (buffer_index > 0) {
                string_t *string = cr_std_string_new(buffer);
                cr_std_vector_push_back(vector, string);
            }

            buffer_index = 0;
        } else {
            buffer[buffer_index] = str->c_str[i];
            buffer_index++;
        }
    }
    return vector;
}
