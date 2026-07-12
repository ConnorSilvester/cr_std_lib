#ifndef CR_STD_CSV_TESTS_H
#define CR_STD_CSV_TESTS_H
#include "cr_std_utils.h"

// Test functions for all functions in cr_std_csv
void cr_std_csv_test_all();

// Parse File
b8 cr_std_csv_test_parse_file_normal();
b8 cr_std_csv_test_parse_file_null_arena();
b8 cr_std_csv_test_parse_file_null_file();
b8 cr_std_csv_test_parse_file_invalid_filepath();

// Print Contents
b8 cr_std_csv_test_print_contents_null_value();

#endif // CR_STD_CSV_TESTS_H
