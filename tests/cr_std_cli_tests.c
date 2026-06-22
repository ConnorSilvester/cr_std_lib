#include "cr_std_cli_tests.h"
#include "cr_std_arena.h"
#include "cr_std_cli.h"
#include "cr_std_string.h"
#include "cr_std_testing.h"
#include "cr_std_utils.h"
#include "cr_std_vector.h"
#include <stdio.h>
#include <stdlib.h>

Arena *g_arena_cli_arena = NULL;

void cr_std_cli_test_all() {
    printf("%s\n", "Running CLI Tests:");

    g_arena_cli_arena = cr_std_arena_new(CR_STD_MB);
    Vector *tests = cr_std_vector_new(g_arena_cli_arena);

    // clang-format off
    // New Arena
    cr_std_vector_push_back(g_arena_cli_arena, tests, cr_std_testing_new_test(g_arena_cli_arena, "Make Arg Def -> Normal", cr_std_cli_test_new_arg_def_normal));
    cr_std_vector_push_back(g_arena_cli_arena, tests, cr_std_testing_new_test(g_arena_cli_arena, "Make Arg Def -> Negative", cr_std_cli_test_new_arg_def_negative));
    cr_std_vector_push_back(g_arena_cli_arena, tests, cr_std_testing_new_test(g_arena_cli_arena, "Make Arg Def -> Null Arena", cr_std_cli_test_new_arg_def_null_arena));
    cr_std_vector_push_back(g_arena_cli_arena, tests, cr_std_testing_new_test(g_arena_cli_arena, "Make Arg Def -> Null Flag", cr_std_cli_test_new_arg_def_null_flag));

    // Parse Args
    cr_std_vector_push_back(g_arena_cli_arena, tests, cr_std_testing_new_test(g_arena_cli_arena, "Parse Args -> Normal", cr_std_cli_test_parse_args_normal));
    cr_std_vector_push_back(g_arena_cli_arena, tests, cr_std_testing_new_test(g_arena_cli_arena, "Parse Args -> Normal Found", cr_std_cli_test_parse_args_normal_found));
    cr_std_vector_push_back(g_arena_cli_arena, tests, cr_std_testing_new_test(g_arena_cli_arena, "Parse Args -> Normal Found Sub", cr_std_cli_test_parse_args_normal_found_sub));
    cr_std_vector_push_back(g_arena_cli_arena, tests, cr_std_testing_new_test(g_arena_cli_arena, "Parse Args -> Error Flag", cr_std_cli_test_parse_args_error_flag));
    cr_std_vector_push_back(g_arena_cli_arena, tests, cr_std_testing_new_test(g_arena_cli_arena, "Parse Args -> 0 Argc", cr_std_cli_test_parse_args_0_argc));
    cr_std_vector_push_back(g_arena_cli_arena, tests, cr_std_testing_new_test(g_arena_cli_arena, "Parse Args -> Negative Argc", cr_std_cli_test_parse_args_negative_argc));
    cr_std_vector_push_back(g_arena_cli_arena, tests, cr_std_testing_new_test(g_arena_cli_arena, "Parse Args -> Null Arena", cr_std_cli_test_parse_args_null_arena));
    cr_std_vector_push_back(g_arena_cli_arena, tests, cr_std_testing_new_test(g_arena_cli_arena, "Parse Args -> Null Vec", cr_std_cli_test_parse_args_null_vec));
    cr_std_vector_push_back(g_arena_cli_arena, tests, cr_std_testing_new_test(g_arena_cli_arena, "Parse Args -> Null Argv", cr_std_cli_test_parse_args_null_argv));

    // clang-format on
    cr_std_testing_run_tests(g_arena_cli_arena, tests);
    cr_std_arena_free(&g_arena_cli_arena);
}

b8 cr_std_cli_test_new_arg_def_normal() {
    size_t parameter_count = 2;
    const char *flag = "-b";
    ArgumentDefinition *arg =
    cr_std_cli_new_argument_definition(g_arena_cli_arena, flag, parameter_count);
    if (!arg) {
        return false;
    }
    if (cr_std_string_compare_c_str(arg->flag, flag) != CR_STD_STRING_EQUAL) {
        return false;
    }
    if (!arg->parameters) {
        return false;
    }
    if (arg->expected_param_count != parameter_count) {
        return false;
    }
    if (arg->found) {
        return false;
    }
    if (arg->error) {
        return false;
    }
    return true;
}

b8 cr_std_cli_test_new_arg_def_negative() {
    size_t parameter_count = -2;
    const char *flag = "-b";
    ArgumentDefinition *arg =
    cr_std_cli_new_argument_definition(g_arena_cli_arena, flag, parameter_count);
    if (!arg) {
        return false;
    }
    if (cr_std_string_compare_c_str(arg->flag, flag) != CR_STD_STRING_EQUAL) {
        return false;
    }
    if (!arg->parameters) {
        return false;
    }
    if (arg->expected_param_count != 0) {
        return false;
    }
    if (arg->found) {
        return false;
    }
    if (arg->error) {
        return false;
    }
    return true;
}

b8 cr_std_cli_test_new_arg_def_null_arena() {
    size_t parameter_count = 2;
    const char *flag = "-b";
    ArgumentDefinition *arg = cr_std_cli_new_argument_definition(NULL, flag, parameter_count);
    if (!arg) {
        return true;
    }
    return false;
}

b8 cr_std_cli_test_new_arg_def_null_flag() {
    size_t parameter_count = 2;
    ArgumentDefinition *arg =
    cr_std_cli_new_argument_definition(g_arena_cli_arena, NULL, parameter_count);
    if (!arg) {
        return true;
    }
    return false;
}

b8 cr_std_cli_test_parse_args_normal() {
    int argc = 2;
    char *argv[] = { "program_name", "-a" };
    size_t parameter_count = 0;
    const char *flag = "-b";
    Vector *vec = cr_std_vector_new(g_arena_cli_arena);
    if (!vec) {
        return false;
    }
    ArgumentDefinition *arg =
    cr_std_cli_new_argument_definition(g_arena_cli_arena, flag, parameter_count);
    if (!arg) {
        return false;
    }
    if (cr_std_vector_push_back(g_arena_cli_arena, vec, arg) != CR_STD_OK) {
        return false;
    }
    if (cr_std_cli_parse_args(g_arena_cli_arena, vec, argc, argv) != CR_STD_OK) {
        return false;
    }
    if (arg->found != false) {
        return false;
    }
    return true;
}

b8 cr_std_cli_test_parse_args_normal_found() {
    int argc = 2;
    char *argv[] = { "program_name", "-a" };
    size_t parameter_count = 0;
    const char *flag = "-a";
    Vector *vec = cr_std_vector_new(g_arena_cli_arena);
    if (!vec) {
        return false;
    }
    ArgumentDefinition *arg =
    cr_std_cli_new_argument_definition(g_arena_cli_arena, flag, parameter_count);
    if (!arg) {
        return false;
    }
    if (cr_std_vector_push_back(g_arena_cli_arena, vec, arg) != CR_STD_OK) {
        return false;
    }
    if (cr_std_cli_parse_args(g_arena_cli_arena, vec, argc, argv) != CR_STD_OK) {
        return false;
    }
    if (arg->found != true) {
        return false;
    }
    return true;
}

b8 cr_std_cli_test_parse_args_normal_found_sub() {
    int argc = 4;
    char *argv[] = { "program_name", "-a", ".data", ".data/path" };
    size_t parameter_count = 2;
    const char *flag = "-a";
    Vector *vec = cr_std_vector_new(g_arena_cli_arena);
    if (!vec) {
        return false;
    }
    ArgumentDefinition *arg =
    cr_std_cli_new_argument_definition(g_arena_cli_arena, flag, parameter_count);
    if (!arg) {
        return false;
    }
    if (cr_std_vector_push_back(g_arena_cli_arena, vec, arg) != CR_STD_OK) {
        return false;
    }
    if (cr_std_cli_parse_args(g_arena_cli_arena, vec, argc, argv) != CR_STD_OK) {
        return false;
    }
    if (arg->found != true) {
        return false;
    }
    if (arg->parameters->size != parameter_count) {
        return false;
    }
    return true;
}

b8 cr_std_cli_test_parse_args_error_flag() {
    int argc = 4;
    char *argv[] = { "program_name", "-a", ".data", ".data/path" };
    size_t parameter_count = 4;
    const char *flag = "-a";
    Vector *vec = cr_std_vector_new(g_arena_cli_arena);
    if (!vec) {
        return false;
    }
    ArgumentDefinition *arg =
    cr_std_cli_new_argument_definition(g_arena_cli_arena, flag, parameter_count);
    if (!arg) {
        return false;
    }
    if (cr_std_vector_push_back(g_arena_cli_arena, vec, arg) != CR_STD_OK) {
        return false;
    }
    if (cr_std_cli_parse_args(g_arena_cli_arena, vec, argc, argv) != CR_STD_FAIL) {
        return false;
    }
    if (arg->found != true) {
        return false;
    }
    if (arg->error != true) {
        return false;
    }
    return true;
}

b8 cr_std_cli_test_parse_args_0_argc() {
    int argc = 1;
    char *argv[] = { "program_name" };
    size_t parameter_count = 0;
    const char *flag = "-a";
    Vector *vec = cr_std_vector_new(g_arena_cli_arena);
    if (!vec) {
        return false;
    }
    ArgumentDefinition *arg =
    cr_std_cli_new_argument_definition(g_arena_cli_arena, flag, parameter_count);
    if (!arg) {
        return false;
    }
    if (cr_std_vector_push_back(g_arena_cli_arena, vec, arg) != CR_STD_OK) {
        return false;
    }
    if (cr_std_cli_parse_args(g_arena_cli_arena, vec, argc, argv) != CR_STD_OK) {
        return false;
    }
    return true;
}

b8 cr_std_cli_test_parse_args_negative_argc() {
    int argc = -1;
    char *argv[] = { "program_name" };
    size_t parameter_count = 0;
    const char *flag = "-a";
    Vector *vec = cr_std_vector_new(g_arena_cli_arena);
    if (!vec) {
        return false;
    }
    ArgumentDefinition *arg =
    cr_std_cli_new_argument_definition(g_arena_cli_arena, flag, parameter_count);
    if (!arg) {
        return false;
    }
    if (cr_std_vector_push_back(g_arena_cli_arena, vec, arg) != CR_STD_OK) {
        return false;
    }
    if (cr_std_cli_parse_args(g_arena_cli_arena, vec, argc, argv) != CR_STD_OK) {
        return false;
    }
    return true;
}

b8 cr_std_cli_test_parse_args_null_arena() {
    int argc = 4;
    char *argv[] = { "program_name", "-a", ".data", ".data/path" };
    size_t parameter_count = 2;
    const char *flag = "-a";
    Vector *vec = cr_std_vector_new(g_arena_cli_arena);
    if (!vec) {
        return false;
    }
    ArgumentDefinition *arg =
    cr_std_cli_new_argument_definition(g_arena_cli_arena, flag, parameter_count);
    if (!arg) {
        return false;
    }
    if (cr_std_vector_push_back(g_arena_cli_arena, vec, arg) != CR_STD_OK) {
        return false;
    }
    if (cr_std_cli_parse_args(NULL, vec, argc, argv) != CR_STD_FAIL) {
        return false;
    }
    return true;
}

b8 cr_std_cli_test_parse_args_null_vec() {
    int argc = 4;
    char *argv[] = { "program_name", "-a", ".data", ".data/path" };
    if (cr_std_cli_parse_args(g_arena_cli_arena, NULL, argc, argv) != CR_STD_FAIL) {
        return false;
    }
    return true;
}

b8 cr_std_cli_test_parse_args_null_argv() {
    size_t parameter_count = 2;
    const char *flag = "-a";
    Vector *vec = cr_std_vector_new(g_arena_cli_arena);
    if (!vec) {
        return false;
    }
    ArgumentDefinition *arg =
    cr_std_cli_new_argument_definition(g_arena_cli_arena, flag, parameter_count);
    if (!arg) {
        return false;
    }
    if (cr_std_vector_push_back(g_arena_cli_arena, vec, arg) != CR_STD_OK) {
        return false;
    }
    if (cr_std_cli_parse_args(g_arena_cli_arena, vec, 0, NULL) != CR_STD_FAIL) {
        return false;
    }
    return true;
}
