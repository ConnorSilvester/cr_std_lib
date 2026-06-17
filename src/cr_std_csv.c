#include "cr_std_csv.h"
#include "cr_std_arena.h"
#include "cr_std_filesystem.h"
#include "cr_std_logger.h"
#include "cr_std_string.h"
#include "cr_std_vector.h"
#include <stdio.h>
#include <stdlib.h>

CSVFile *cr_std_csv_new(Arena *arena) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_csv_new -> arena* was NULL");
        return NULL;
    }

    CSVFile *csv = cr_std_arena_alloc(arena, sizeof(CSVFile));
    if (!csv) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_csv_new -> failed to allocate memory for CSVFile");
        return NULL;
    }
    csv->titles = cr_std_vector_new(arena);
    csv->rows = cr_std_vector_new(arena);
    return csv;
}

CSVRow *cr_std_csv_row_new(Arena *arena) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_csv_row_new -> arena* was NULL");
        return NULL;
    }

    CSVRow *row = cr_std_arena_alloc(arena, sizeof(*row));
    if (!row) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_csv_row_new -> failed to allocate memory for Row");
        return NULL;
    }
    row->fields = cr_std_vector_new(arena);
    return row;
}

CSVFile *cr_std_csv_parse_file(Arena *arena, const char *file_path) {
    CSVFile *csv = cr_std_csv_new(arena);

    Arena *temp_arena = cr_std_arena_new(CR_STD_FILE_MAX_SIZE);
    String *file_contents = cr_std_filesystem_read_file_as_string(temp_arena, file_path);
    if (!file_contents) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_WARNING,
                          "cr_std_csv_parse_file -> returned empty CSVFile");
        return csv;
    }
    const char *src = file_contents->c_str;

    bool in_quotes = false;
    bool is_first_row = true;
    StringBuilder *sb = cr_std_string_builder_newc(temp_arena, "", temp_arena->capacity / 4);

    CSVRow *row = cr_std_csv_row_new(arena);

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
                cr_std_string_builder_append_string(temp_arena, sb, "\"");
                index++;
            } else {
                in_quotes = false;
            }
        } else if (c == ',' && !in_quotes) {
            String *field = cr_std_string_builder_to_string(arena, sb);
            cr_std_vector_push_back(arena, row->fields, field);
            cr_std_string_builder_reset(sb);
        } else if ((c == '\n') && !in_quotes) {
            String *field = cr_std_string_builder_to_string(arena, sb);
            cr_std_vector_push_back(arena, row->fields, field);

            if (is_first_row) {
                csv->titles = row->fields;
                is_first_row = false;
            } else {
                cr_std_vector_push_back(arena, csv->rows, row);
            }

            // New Row (reset row and sb)
            row = cr_std_csv_row_new(arena);
            cr_std_string_builder_reset(sb);
        } else {
            if (c != '\r') {
                cr_std_string_builder_append_char(temp_arena, sb, c);
            }
        }
    }

    if (sb->size > 0 || row->fields->size > 0) {
        if (sb->size > 0) {
            String *field = cr_std_string_builder_to_string(arena, sb);
            cr_std_vector_push_back(arena, row->fields, field);
        }
        cr_std_vector_push_back(arena, csv->rows, row);
    }
    cr_std_arena_free(&temp_arena);
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
