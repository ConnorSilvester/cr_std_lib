#include "cr_std_logger.h"
#include "cr_std_filesystem.h"
#include "cr_std_string.h"
#include "cr_std_vector.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int cr_std_logger_current_log_level = CR_STD_LOGGER_LOG_LEVEL_ALL;
static bool cr_std_logger_record_log_flag = false;
static Vector *cr_std_logger_log_history = NULL;

void cr_std_logger_get_current_time(char *buffer, size_t buffer_size) {
    time_t raw_time;
    struct tm *time_info;

    time(&raw_time);
    time_info = localtime(&raw_time);

    // Format the time as HH:MM:SS
    strftime(buffer, buffer_size, "%H:%M:%S", time_info);
}

void cr_std_logger_init_history() {
    if (!cr_std_logger_log_history) {
        cr_std_logger_log_history = cr_std_vector_new(String *);
        cr_std_logger_log_history->free_function = cr_std_string_free_ptr;
    }
}

void cr_std_logger_record_log(int log_type, const char *log_message) {
    if (cr_std_logger_record_log_flag) {
        cr_std_logger_init_history();

        char time_str[9]; // HH:MM:SS + null terminator
        cr_std_logger_get_current_time(time_str, sizeof(time_str));

        String *log = NULL;
        switch (log_type) {
        case CR_STD_LOGGER_LOG_TYPE_INFO:
            log = cr_std_string_newf("[%s] [INFO] %s\n", time_str, log_message);
            break;
        case CR_STD_LOGGER_LOG_TYPE_WARNING:
            log = cr_std_string_newf("[%s] \033[33m[WARNING]\033[0m %s\n", time_str, log_message);
            break;
        case CR_STD_LOGGER_LOG_TYPE_ERROR:
            log = cr_std_string_newf("[%s] \033[31m[ERROR]\033[0m %s\n", time_str, log_message);
            break;
        default:
            break;
        }
        if (log) {
            cr_std_vector_push_back(cr_std_logger_log_history, log);
        }
    }
}

void cr_std_logger_out(int log_type, const char *log_message) {
    if (log_type < cr_std_logger_current_log_level) {
        return;
    }

    char time_str[9]; // HH:MM:SS + null terminator
    cr_std_logger_get_current_time(time_str, sizeof(time_str));

    switch (log_type) {
    case CR_STD_LOGGER_LOG_TYPE_INFO:
        printf("[%s] [INFO] %s\n", time_str, log_message);
        break;
    case CR_STD_LOGGER_LOG_TYPE_WARNING:
        printf("[%s] \033[33m[WARNING]\033[0m %s\n", time_str, log_message);
        break;
    case CR_STD_LOGGER_LOG_TYPE_ERROR:
        printf("[%s] \033[31m[ERROR]\033[0m %s\n", time_str, log_message);
        break;
    default:
        break;
    }

    cr_std_logger_record_log(log_type, log_message);
}

void cr_std_logger_outf(int log_type, const char *formatted_str, ...) {
    va_list args;
    va_start(args, formatted_str);

    int length_of_string = vsnprintf(NULL, 0, formatted_str, args);
    va_end(args);

    char *result = (char *)malloc(length_of_string + 1);
    if (result == NULL) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "memory allocation failed in cr_std_logger_outf");
        return;
    }

    va_start(args, formatted_str);
    vsnprintf(result, length_of_string + 1, formatted_str, args);
    va_end(args);

    cr_std_logger_out(log_type, result);

    free(result);
}

int cr_std_logger_set_log_level(int log_level) {
    if (log_level < 0) {
        cr_std_logger_current_log_level = 0;
        return 1;
    }
    if (log_level > CR_STD_LOGGER_LOG_LEVEL_NONE) {
        cr_std_logger_current_log_level = CR_STD_LOGGER_LOG_LEVEL_NONE;
        return 1;
    }
    cr_std_logger_current_log_level = log_level;
    return 0;
}

void cr_std_logger_start_recording() {
    cr_std_logger_record_log_flag = true;
}

void cr_std_logger_stop_recording() {
    cr_std_logger_record_log_flag = false;
}

Vector *cr_std_logger_get_history() {
    return cr_std_logger_log_history;
}

void cr_std_logger_print_history() {
    cr_std_logger_init_history();
    for (int i = 0; i < cr_std_logger_log_history->size; i++) {
        String *current_log = cr_std_vector_get_at(cr_std_logger_log_history, String, i);
        printf("%s", current_log->c_str);
    }
}

void cr_std_logger_clear_history() {
    cr_std_vector_free(&cr_std_logger_log_history);
    cr_std_logger_log_history = NULL;
}

int cr_std_logger_write_history_to_file(const char *filepath) {
    cr_std_logger_init_history();
    int result = 0;
    if (cr_std_logger_log_history->size > 0) {
        StringBuilder *sb = cr_std_string_builder_new("");
        for (int i = 0; i < cr_std_logger_log_history->size; i++) {
            String *current_log = cr_std_vector_get_at(cr_std_logger_log_history, String, i);
            cr_std_string_builder_append_string(sb, current_log->c_str);
        }
        String *output = cr_std_string_builder_to_string(sb);

        // String Color
        cr_std_string_replace_string(output, "\033[33m", "");
        cr_std_string_replace_string(output, "\033[31m", "");
        cr_std_string_replace_string(output, "\033[0m", "");

        result = cr_std_filesystem_write_to_file(filepath, output->c_str);
        cr_std_string_builder_free(&sb);
        cr_std_string_free(&output);
    } else {
        result = cr_std_filesystem_write_to_file(filepath, "");
    }
    return result;
}
