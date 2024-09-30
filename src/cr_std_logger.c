#include "cr_std_logger.h"
#include <stdarg.h>
#include <time.h>

void cr_std_logger_get_current_time(char *buffer, size_t buffer_size) {
    time_t raw_time;
    struct tm *time_info;

    time(&raw_time);
    time_info = localtime(&raw_time);

    // Format the time as HH:MM:SS
    strftime(buffer, buffer_size, "%H:%M:%S", time_info);
}

void cr_std_logger_out(int log_type, const char *formatted_str, ...) {
    char time_str[9]; // HH:MM:SS + null terminator
    cr_std_logger_get_current_time(time_str, sizeof(time_str));

    va_list args;
    va_start(args, formatted_str);

    int length_of_string = vsnprintf(NULL, 0, formatted_str, args);
    char result[length_of_string];

    va_start(args, formatted_str);
    vsnprintf(result, length_of_string + 1, formatted_str, args);
    va_end(args);

    switch (log_type) {
    case CR_STD_LOGGER_LOG_TYPE_INFO:
        printf("[%s] [INFO] %s\n", time_str, result);
        break;
    case CR_STD_LOGGER_LOG_TYPE_WARNING:
        printf("[%s] [WARNING] %s\n", time_str, result);
        break;
    case CR_STD_LOGGER_LOG_TYPE_ERROR:
        printf("[%s] [ERROR] %s\n", time_str, result);
        break;
    default:
        break;
    }
}
