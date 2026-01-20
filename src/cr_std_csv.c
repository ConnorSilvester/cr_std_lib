#include "cr_std_csv.h"
#include "cr_std_filesystem.h"
#include "cr_std_logger.h"
#include "cr_std_string.h"
#include "cr_std_vector.h"
#include <stdio.h>
#include <stdlib.h>

CSVFile *cr_std_csv_new() {
    CSVFile *csv = (CSVFile *)malloc(sizeof(CSVFile));
    if (!csv) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_csv_new -> Failed to allocate memory for CSVFile");
        return NULL;
    }
    csv->titles = cr_std_vector_new(String *);
    csv->titles->free_function = cr_std_string_free_ptr;

    csv->rows = cr_std_vector_new(CSVRow *);
    csv->rows->free_function = cr_std_csv_row_free_ptr;

    return csv;
}

CSVRow *cr_std_csv_row_new() {
    CSVRow *row = (CSVRow *)malloc(sizeof(CSVRow));
    if (!row) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_csv_row_new -> Failed to allocate memory for Row");
        return NULL;
    }
    row->fields = cr_std_vector_new(String *);
    row->fields->free_function = cr_std_string_free_ptr;

    return row;
}

int cr_std_csv_free(CSVFile **csv_ptr) {
    if (!csv_ptr || !*csv_ptr) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_csv_free -> csv_ptr is NULL");
        return 1;
    }

    CSVFile *csv = *csv_ptr;
    if (csv->titles) {
        cr_std_vector_free(&(csv->titles));
        csv->titles = NULL;
    }

    if (csv->rows) {
        cr_std_vector_free(&(csv->rows));
        csv->rows = NULL;
    }

    free(csv);
    *csv_ptr = NULL;

    return 0;
}

int cr_std_csv_row_free(CSVRow **row_ptr) {
    if (!row_ptr || !*row_ptr) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_csv_row_free -> row_ptr is NULL");
        return 1;
    }

    CSVRow *row = *row_ptr;
    if (row->fields) {
        cr_std_vector_free(&(row->fields));
        row->fields = NULL;
    }

    free(row);
    *row_ptr = NULL;

    return 0;
}

CSVFile *cr_std_csv_parse_file(const char *file_path) {
    CSVFile *csv = cr_std_csv_new();

    String *file_contents = cr_std_filesystem_read_file_as_string(file_path);
    if (!file_contents) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_WARNING,
                          "cr_std_csv_parse_file -> returned empty CSVFile");
        return csv;
    }
    const char *src = file_contents->c_str;

    bool in_quotes = false;
    bool is_first_row = true;
    StringBuilder *sb = cr_std_string_builder_new("");

    CSVRow *row = cr_std_csv_row_new();

    int index = 0;
    if (file_contents->length >= 3 && (unsigned char)src[0] == 0xEF &&
        (unsigned char)src[1] == 0xBB && (unsigned char)src[2] == 0xBF) {
        index = 3; // Skip BOM
    }

    for (; index < file_contents->length; index++) {

        char c = src[index];

        if (c == '"' && !in_quotes) {
            in_quotes = true;
        } else if (c == '"' && in_quotes) {
            if (src[index + 1] == '"') {
                cr_std_string_builder_append_string(sb, "\"");
                index++;
            } else {
                in_quotes = false;
            }
        } else if (c == ',' && !in_quotes) {
            String *field = cr_std_string_builder_to_string(sb);
            cr_std_vector_push_back(row->fields, field);
            cr_std_string_builder_reset(sb);
        } else if ((c == '\n') && !in_quotes) {
            String *field = cr_std_string_builder_to_string(sb);
            cr_std_vector_push_back(row->fields, field);

            if (is_first_row) {
                cr_std_vector_free(&csv->titles);
                csv->titles = row->fields; // Move first row's fields to titles
                is_first_row = false;
                free(row);
            } else {
                cr_std_vector_push_back(csv->rows, row);
            }

            // New Row (reset row and sb)
            row = cr_std_csv_row_new();
            cr_std_string_builder_reset(sb);
        } else {
            if (c != '\r') {
                cr_std_string_builder_append_char(sb, c);
            }
        }
    }

    if (sb->size > 0 || row->fields->size > 0) {
        if (sb->size > 0) {
            String *field = cr_std_string_builder_to_string(sb);
            cr_std_vector_push_back(row->fields, field);
        }
        cr_std_vector_push_back(csv->rows, row);
    }
    cr_std_string_free(&file_contents);
    cr_std_string_builder_free(&sb);
    return csv;
}

int cr_std_csv_print_contents(CSVFile *csv) {
    if (!csv) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_csv_print_contents -> CSVFile struct is NULL");
        return 1;
    }

    // Header
    for (int title_index = 0; title_index < csv->titles->size; title_index++) {
        String *title = cr_std_vector_get_at(csv->titles, String, title_index);
        printf("Title : %s\n", title->c_str);
    }

    // Rows
    for (int row_index = 0; row_index < csv->rows->size; row_index++) {
        CSVRow *row = cr_std_vector_get_at(csv->rows, CSVRow, row_index);
        printf("Row %d:\n", row_index + 1);
        for (int field_index = 0; field_index < row->fields->size; field_index++) {
            String *field = cr_std_vector_get_at(row->fields, String, field_index);
            if (field_index < csv->titles->size) {
                String *title = cr_std_vector_get_at(csv->titles, String, field_index);
                printf("    %s : %s\n", title->c_str, field->c_str);
            } else {
                printf("    Field : %s\n", field->c_str);
            }
        }
    }

    return 0;
}
