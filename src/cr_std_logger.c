#include "cr_std_logger.h"
#include <stdarg.h>
#include <time.h>

static int cr_std_logger_current_log_level = CR_STD_LOGGER_LOG_LEVEL_ALL;

void cr_std_logger_get_current_time(char *buffer, size_t buffer_size) {
    time_t raw_time;
    struct tm *time_info;

    time(&raw_time);
    time_info = localtime(&raw_time);

    // Format the time as HH:MM:SS
    strftime(buffer, buffer_size, "%H:%M:%S", time_info);
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
}

void cr_std_logger_outf(int log_type, const char *formatted_str, ...) {
    if (log_type < cr_std_logger_current_log_level) {
        return;
    }

    va_list args;
    va_start(args, formatted_str);

    int length_of_string = vsnprintf(NULL, 0, formatted_str, args);
    char result[length_of_string];

    va_start(args, formatted_str);
    vsnprintf(result, length_of_string + 1, formatted_str, args);
    va_end(args);

    cr_std_logger_out(log_type, result);
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
