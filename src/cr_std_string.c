#include "cr_std_string.h"
#include "cr_std_logger.h"
#include "cr_std_vector.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wctype.h>

string_t *cr_std_string_new(const char *format, ...) {
    if (!format) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_string_new -> char* input was NULL");
        return NULL;
    }

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
        va_end(args);
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
    cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_WARNING, "cr_std_string_free -> tried to free a NULL string_t*");
    return 0;
}

string_t *cr_std_string_make_copy(string_t *src_string) {
    if (!src_string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_string_make_copy -> string pointer is NULL");
        return NULL;
    }
    if (!src_string->c_str) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_string_make_copy -> string->c_str pointer is NULL");
        return NULL;
    }
    return cr_std_string_new(src_string->c_str);
}

int cr_std_string_concat_null_terminated(string_t *string, ...) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_string_concat_null_terminated -> string pointer is NULL");
        return 0;
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
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_string_concat_null_terminated -> Failed to allocate memory for new string_t struct");
        return 0;
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

int cr_std_string_trim(string_t *string, int direction) {
    if (string == NULL || string->c_str == NULL || string->length == 0) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_string_trim -> tried to trim an invalid string");
        return 0;
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
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_WARNING, "cr_std_string_trim -> new string is empty");
    }
    char *new_str = malloc(sizeof(char) * (new_length + 1));
    if (new_str == NULL) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_string_trim -> malloc failed to allocate memory for new string");
        return 0;
    }

    memcpy(new_str, string->c_str + start, new_length);
    new_str[new_length] = '\0';

    free(string->c_str);
    string->c_str = new_str;
    string->length = new_length;

    return 1;
}

int cr_std_string_find_char(string_t *string, char ch) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_string_find_char -> string pointer is NULL");
        return -1;
    }
    for (int i = 0; i <= string->length; i++) {
        if (string->c_str[i] == ch) {
            return i;
        }
    }
    return -1;
}

int cr_std_string_find_char_last(string_t *string, char ch) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_string_find_char_last -> string pointer is NULL");
        return -1;
    }
    for (int i = string->length; i >= 0; i--) {
        if (string->c_str[i] == ch) {
            return i;
        }
    }
    return -1;
}

int cr_std_string_contains_string(string_t *string, char *phrase) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_string_contains_string -> string pointer is NULL");
        return 0;
    }

    int phrase_length = strlen(phrase);
    if (phrase_length > string->length) {
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

int cr_std_string_contains_char(string_t *string, char ch) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_string_contains_char -> string pointer is NULL");
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

unsigned long cr_std_string_hash_code(string_t *string) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_string_hash_code -> string pointer is NULL");
        return 0;
    }
    unsigned long hash = 5381; // Initial hash value

    for (size_t i = 0; i < string->length; i++) {
        hash = ((hash << 5) + hash) + (int)(string->c_str[i]); // hash * 33 + char
    }

    return hash;
}

vector_t *cr_std_string_split(string_t *string, char delimiter) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_string_split -> string pointer is NULL");
        return NULL;
    }
    vector_t *vector = cr_std_vector_new(sizeof(string_t *));
    char buffer[string->length + 1];
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
                string_t *string = cr_std_string_new(buffer);
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

int cr_std_string_to_upper(string_t *string) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_string_to_upper -> string pointer is NULL");
        return 0;
    }
    for (int i = 0; i < string->length; i++) {
        string->c_str[i] = toupper(string->c_str[i]);
    }
    return 1;
}

int cr_std_string_to_lower(string_t *string) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_string_to_lower -> string pointer is NULL");
        return 0;
    }
    for (int i = 0; i < string->length; i++) {
        string->c_str[i] = tolower(string->c_str[i]);
    }
    return 1;
}

int cr_std_string_replace_string(string_t *string, char *from, char *to) {
    if (!string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_string_replace_string -> string pointer is NULL");
        return 0;
    }

    int occurrences = cr_std_string_contains_string(string, from);
    if (occurrences == 0) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_WARNING, "cr_std_string_replace_string -> string does not contain phrase");
        return 0;
    }

    int from_length = strlen(from);
    int to_length = strlen(to);

    int new_length = string->length + (to_length - from_length) * occurrences;
    char *new_string = malloc(sizeof(char) * (new_length + 1));
    if (!new_string) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_string_replace_string -> malloc allocation failed for new_string");
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
