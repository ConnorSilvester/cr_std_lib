#include "cr_std_csv_tests.h"
#include "cr_std_arena.h"
#include "cr_std_cli.h"
#include "cr_std_csv.h"
#include "cr_std_string.h"
#include "cr_std_testing.h"
#include "cr_std_utils.h"
#include "cr_std_vector.h"
#include <stdio.h>
#include <stdlib.h>

Arena *g_arena_csv_arena = NULL;
#define EXAMPLE_CSV_PATH "tests/data/example.csv"

void cr_std_csv_test_all() {
    printf("%s\n", "Running CSV Tests:");

    g_arena_csv_arena = cr_std_arena_new(CR_STD_MB);
    Vector *tests = cr_std_vector_new(g_arena_csv_arena);

    // clang-format off
    // Parse File
    cr_std_vector_push_back(g_arena_csv_arena, tests, cr_std_testing_new_test(g_arena_csv_arena, "Parse File -> Normal", cr_std_csv_test_parse_file_normal));
    cr_std_vector_push_back(g_arena_csv_arena, tests, cr_std_testing_new_test(g_arena_csv_arena, "Parse File -> Null Arena", cr_std_csv_test_parse_file_null_arena));
    cr_std_vector_push_back(g_arena_csv_arena, tests, cr_std_testing_new_test(g_arena_csv_arena, "Parse File -> Null File", cr_std_csv_test_parse_file_null_file));
    cr_std_vector_push_back(g_arena_csv_arena, tests, cr_std_testing_new_test(g_arena_csv_arena, "Parse File -> Invalid Filepath", cr_std_csv_test_parse_file_invalid_filepath));

    // Print Contents
    cr_std_vector_push_back(g_arena_csv_arena, tests, cr_std_testing_new_test(g_arena_csv_arena, "Print Contents -> Null Value", cr_std_csv_test_print_contents_null_value));

    // clang-format on
    cr_std_testing_run_tests(g_arena_csv_arena, tests);
    cr_std_arena_free(&g_arena_csv_arena);
}

b8 cr_std_csv_test_parse_file_normal() {
    CSVFile *csv = cr_std_csv_parse_file(g_arena_csv_arena, EXAMPLE_CSV_PATH);
    if (!csv) {
        return false;
    }
    size_t expected_cols = 14;
    size_t expected_rows = 45;
    if (csv->titles->size != expected_cols) {
        return false;
    }
    if (csv->rows->size != expected_rows) {
        return false;
    }
    return true;
}

b8 cr_std_csv_test_parse_file_null_arena() {
    CSVFile *csv = cr_std_csv_parse_file(NULL, EXAMPLE_CSV_PATH);
    return csv == NULL;
}

b8 cr_std_csv_test_parse_file_null_file() {
    CSVFile *csv = cr_std_csv_parse_file(g_arena_csv_arena, NULL);
    return csv == NULL;
}

b8 cr_std_csv_test_parse_file_invalid_filepath() {
    CSVFile *csv = cr_std_csv_parse_file(g_arena_csv_arena, "SHOULD_NOT_EXIST.csv");
    return csv == NULL;
}

b8 cr_std_csv_test_print_contents_null_value() {
    return cr_std_csv_print_contents(NULL) == CR_STD_FAIL;
}
