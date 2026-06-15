#include "cr_std_cli.h"
#include "cr_std_logger.h"
#include "cr_std_string.h"
#include "cr_std_vector.h"
#include <stdlib.h>

int is_flag(Vector *expected_args, char *arg) {
    for (int i = 0; i < expected_args->size; i++) {
        ArgumentDefinition *current_arg =
        cr_std_vector_get_at(expected_args, ArgumentDefinition, i);
        if (cr_std_string_compare_c_str(current_arg->flag, arg) == 1) {
            return 1;
        }
    }
    return 0;
}

ArgumentDefinition *cr_std_cli_new_argument_definition(const char *flag, int expected_param_count) {
    ArgumentDefinition *arg = malloc(sizeof(ArgumentDefinition));

    if (!arg) {
        cr_std_logger_out(
        CR_STD_LOGGER_LOG_TYPE_ERROR,
        "cr_std_cli_new_argument_definition -> Failed to allocate memory for ArgumentDefinition");
        return NULL;
    }

    arg->parameters = cr_std_vector_new(String *);
    arg->parameters->free_function = cr_std_string_free_ptr;
    arg->flag = cr_std_string_new(flag);
    arg->expected_param_count = expected_param_count;

    return arg;
}

int cr_std_cli_free_argument_definition(ArgumentDefinition **arg_ptr) {
    if (!arg_ptr || !*arg_ptr) {
        CR_LOG_ERROR("cr_std_cli_free_argument_definition -> arg_ptr is NULL");
        return 1;
    }

    ArgumentDefinition *arg = *arg_ptr;
    if (arg->parameters) {
        cr_std_vector_free(&(arg->parameters));
        arg->parameters = NULL;
    }

    if (arg->flag) {
        cr_std_string_free(&(arg->flag));
        arg->flag = NULL;
    }

    if (arg->help_text) {
        cr_std_string_free(&(arg->help_text));
        arg->help_text = NULL;
    }

    free(arg);
    *arg_ptr = NULL;

    return 0;
}

int cr_std_cli_parse_args(Vector *argument_definitions, int argc, char **argv) {
    int dealt_with[argc] = {};
    int return_value = 0;

    for (int index = 1; index < argc; index++) {
        for (int arg_index = 0; arg_index < argument_definitions->size; arg_index++) {
            if (dealt_with[index]) {
                continue;
            }

            ArgumentDefinition *current_arg =
            cr_std_vector_get_at(argument_definitions, ArgumentDefinition, arg_index);
            if (cr_std_string_compare_c_str(current_arg->flag, argv[index]) == 1) {
                CR_LOG_INFO_FMT("Found Keyword : %s", current_arg->flag->c_str);
                dealt_with[index] = 1;
                current_arg->found = 1;

                int index_offset = index + 1;
                int sub_index = index_offset;
                while (sub_index < argc &&
                       sub_index < index_offset + current_arg->expected_param_count) {
                    if (is_flag(argument_definitions, argv[sub_index])) {
                        break;
                    }
                    CR_LOG_INFO_FMT("Found param for flag %s : '%s'", current_arg->flag->c_str,
                                    argv[sub_index]);
                    dealt_with[sub_index] = 1;
                    sub_index++;
                }
                if (sub_index != index_offset + current_arg->expected_param_count) {
                    CR_LOG_ERROR_FMT(
                    "Incorrect amount of params for flag '%s' : Expected %d Found %d",
                    current_arg->flag->c_str, current_arg->expected_param_count,
                    sub_index - index_offset);
                    return_value = 1;
                    current_arg->error = 1;
                }
            }
        }
    }

    for (int i = 1; i < argc; i++) {
        if (dealt_with[i] == 0) {
            CR_LOG_WARNING_FMT("Unknown argument : %s", argv[i]);
        }
    }
    return return_value;
}
