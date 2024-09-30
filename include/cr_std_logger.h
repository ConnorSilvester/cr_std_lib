#ifndef LOGGER_H
#define LOGGER_H
#include <stdio.h>

#define CR_STD_LOGGER_LOG_TYPE_INFO 0
#define CR_STD_LOGGER_LOG_TYPE_WARNING 1
#define CR_STD_LOGGER_LOG_TYPE_ERROR 2

void cr_std_logger_out(int log_type, const char *log_message, ...);

#endif // LOGGER_H
