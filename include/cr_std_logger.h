#ifndef CR_STD_LOGGER_H
#define CR_STD_LOGGER_H
#include <stdio.h>

#define CR_STD_LOGGER_LOG_TYPE_INFO 0
#define CR_STD_LOGGER_LOG_TYPE_WARNING 1
#define CR_STD_LOGGER_LOG_TYPE_ERROR 2


/**
 * @brief Used to log to the console
 *
 * @param `log_type` One of the defined log types.
 * @param `log_message` The message to print to the console.
 */
void cr_std_logger_out(int log_type, const char *log_message);

/**
 * @brief Used to log to the console
 *
 * @param `log_type` One of the defined log types.
 * @param `formatted_str` The message to print to the console, is formatted.
 * @param `...` This can be any amount of args to add into the format of formatted_str.
 */
void cr_std_logger_outf(int log_type, const char *formatted_str, ...);

#endif // CR_STD_LOGGER_H
