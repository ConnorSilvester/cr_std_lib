#include "cr_std_cli.h"
#include "cr_std_arena.h"
#include "cr_std_logger.h"
#include "cr_std_string.h"
#include "cr_std_vector.h"
#include <stdlib.h>
#include <string.h>

b8 is_flag(Vector *argument_definitions, const char *arg) {
    ArgumentDefinition **args = cr_std_vector_get_all(argument_definitions, ArgumentDefinition);
    for (size_t i = 0; i < argument_definitions->size; i++) {
        if (cr_std_string_compare_c_str(args[i]->flag, arg) == CR_STD_STRING_EQUAL) {
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
        CR_LOG_ERROR(
        "cr_std_cli_new_argument_definition -> failed to allocate memory for ArgumentDefinition");
        return NULL;
    }

    arg->parameters = cr_std_vector_new(arena);
    arg->flag = cr_std_string_new(arena, flag);

    if (!arg->parameters) {
        CR_LOG_ERROR("cr_std_cli_new_argument_definition -> failed to create parameters vector");
        return NULL;
    }

    if (!arg->flag) {
        CR_LOG_ERROR("cr_std_cli_new_argument_definition -> failed to create flag string");
        return NULL;
    }

    if (expected_param_count > SIZE_MAX / 2) {
        expected_param_count = 0;
    }

    arg->help_text = NULL;
    arg->expected_param_count = expected_param_count;
    arg->found = false;
    arg->error = false;

    return arg;
}

b8 cr_std_cli_parse_args(Arena *arena, Vector *argument_definitions, int argc, char **argv) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_cli_parse_args -> arena* was NULL");
        return CR_STD_FAIL;
    }

    if (!argument_definitions) {
        CR_LOG_ERROR("cr_std_cli_parse_args -> argument_definitions is NULL");
        return CR_STD_FAIL;
    }

    if (!argv) {
        CR_LOG_ERROR("cr_std_cli_parse_args -> argv is NULL");
        return CR_STD_FAIL;
    }

    if (argc <= 1) {
        CR_LOG_WARNING("cr_std_cli_parse_args -> Nothing to parse");
        return CR_STD_OK;
    }

    b8 dealt_with[argc];
    memset(dealt_with, false, sizeof(dealt_with));
    b8 return_value = CR_STD_OK;

    ArgumentDefinition **args = cr_std_vector_get_all(argument_definitions, ArgumentDefinition);
    for (size_t index = 1; index < argc; index++) {
        for (size_t arg_index = 0; arg_index < argument_definitions->size; arg_index++) {
            if (dealt_with[index]) {
                continue;
            }

            ArgumentDefinition *current_arg = args[arg_index];
            if (cr_std_string_compare_c_str(current_arg->flag, argv[index]) ==
                CR_STD_STRING_EQUAL) {
                CR_LOG_INFO_FMT("Found Keyword : %s", current_arg->flag->c_str);
                dealt_with[index] = true;
                current_arg->found = true;

                size_t index_offset = index + 1;
                size_t sub_index = index_offset;
                while (sub_index < argc &&
                       sub_index < index_offset + current_arg->expected_param_count) {
                    if (is_flag(argument_definitions, argv[sub_index])) {
                        break;
                    }
                    CR_LOG_INFO_FMT("Found param for flag %s : '%s'", current_arg->flag->c_str,
                                    argv[sub_index]);
                    String *sub_param = cr_std_string_new(arena, argv[sub_index]);
                    if (!sub_param) {
                        current_arg->error = true;
                        return_value = CR_STD_FAIL;
                    }
                    if (cr_std_vector_push_back(arena, args[arg_index]->parameters, sub_param) !=
                        CR_STD_OK) {
                        current_arg->error = true;
                        return_value = CR_STD_FAIL;
                    }
                    dealt_with[sub_index] = true;
                    sub_index++;
                }
                if (sub_index != index_offset + current_arg->expected_param_count) {
                    CR_LOG_ERROR_FMT(
                    "Incorrect amount of params for flag '%s' : Expected %zu Found %zu",
                    current_arg->flag->c_str, current_arg->expected_param_count,
                    sub_index - index_offset);
                    current_arg->error = true;
                    return_value = CR_STD_FAIL;
                }
            }
        }
    }

    for (size_t i = 1; i < argc; i++) {
        if (dealt_with[i] == false) {
            CR_LOG_WARNING_FMT("Unknown argument : %s", argv[i]);
        }
    }
    return return_value;
}
