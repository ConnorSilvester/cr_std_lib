#include "cr_std_cli.h"
#include "cr_std_arena.h"
#include "cr_std_logger.h"
#include "cr_std_string.h"
#include "cr_std_vector.h"
#include <stdlib.h>
#include <string.h>

b8 is_flag(Vector *expected_args, const char *arg) {
    for (int i = 0; i < expected_args->size; i++) {
        ArgumentDefinition *current_arg =
        cr_std_vector_get_at(expected_args, ArgumentDefinition, i);
        if (cr_std_string_compare_c_str(current_arg->flag, arg) == 1) {
            return true;
        }
    }
    return false;
}

ArgumentDefinition *
cr_std_cli_new_argument_definition(Arena *arena, const char *flag, size_t expected_param_count) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_cli_new_argument_definition -> arena* was NULL");
        return NULL;
    }

    if (!flag) {
        CR_LOG_ERROR("cr_std_cli_new_argument_definition -> flag* was NULL");
        return NULL;
    }

    ArgumentDefinition *arg = cr_std_arena_alloc(arena, sizeof(*arg));

    if (!arg) {
        cr_std_logger_out(
        CR_STD_LOGGER_LOG_TYPE_ERROR,
        "cr_std_cli_new_argument_definition -> failed to allocate memory for ArgumentDefinition");
        return NULL;
    }

    arg->parameters = cr_std_vector_new(arena);
    arg->flag = cr_std_string_new(arena, flag);
    arg->help_text = NULL;
    arg->expected_param_count = expected_param_count;
    arg->found = false;
    arg->error = false;

    return arg;
}

b8 cr_std_cli_parse_args(Vector *argument_definitions, int argc, char **argv) {
    b8 dealt_with[argc];
    memset(dealt_with, false, sizeof(dealt_with));
    b8 return_value = CR_STD_OK;

    if (!argument_definitions) {
        CR_LOG_ERROR("cr_std_cli_parse_args -> argument_definitions is NULL");
        return CR_STD_FAIL;
    }

    for (int index = 1; index < argc; index++) {
        for (int arg_index = 0; arg_index < argument_definitions->size; arg_index++) {
            if (dealt_with[index]) {
                continue;
            }

            ArgumentDefinition *current_arg =
            cr_std_vector_get_at(argument_definitions, ArgumentDefinition, arg_index);
            if (cr_std_string_compare_c_str(current_arg->flag, argv[index]) == 1) {
                CR_LOG_INFO_FMT("Found Keyword : %s", current_arg->flag->c_str);
                dealt_with[index] = true;
                current_arg->found = true;

                int index_offset = index + 1;
                int sub_index = index_offset;
                while (sub_index < argc &&
                       sub_index < index_offset + current_arg->expected_param_count) {
                    if (is_flag(argument_definitions, argv[sub_index])) {
                        break;
                    }
                    CR_LOG_INFO_FMT("Found param for flag %s : '%s'", current_arg->flag->c_str,
                                    argv[sub_index]);
                    dealt_with[sub_index] = true;
                    sub_index++;
                }
                if (sub_index != index_offset + current_arg->expected_param_count) {
                    CR_LOG_ERROR_FMT(
                    "Incorrect amount of params for flag '%s' : Expected %zu Found %d",
                    current_arg->flag->c_str, current_arg->expected_param_count,
                    sub_index - index_offset);
                    current_arg->error = true;
                    return_value = CR_STD_FAIL;
                }
            }
        }
    }

    for (int i = 1; i < argc; i++) {
        if (dealt_with[i] == false) {
            CR_LOG_WARNING_FMT("Unknown argument : %s", argv[i]);
        }
    }
    return return_value;
}
