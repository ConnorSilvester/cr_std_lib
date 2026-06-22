#ifndef CR_STD_CLI_TESTS_H
#define CR_STD_CLI_TESTS_H
#include "cr_std_utils.h"

// Test functions for all functions in cr_std_cli
void cr_std_cli_test_all();

// New Argument Definition
b8 cr_std_cli_test_new_arg_def_normal();
b8 cr_std_cli_test_new_arg_def_negative();
b8 cr_std_cli_test_new_arg_def_null_arena();
b8 cr_std_cli_test_new_arg_def_null_flag();

// Parse Args
b8 cr_std_cli_test_parse_args_normal();
b8 cr_std_cli_test_parse_args_normal_found();
b8 cr_std_cli_test_parse_args_normal_found_sub();
b8 cr_std_cli_test_parse_args_error_flag();
b8 cr_std_cli_test_parse_args_0_argc();
b8 cr_std_cli_test_parse_args_negative_argc();
b8 cr_std_cli_test_parse_args_null_arena();
b8 cr_std_cli_test_parse_args_null_vec();
b8 cr_std_cli_test_parse_args_null_argv();

#endif // CR_STD_CLI_TESTS_H
