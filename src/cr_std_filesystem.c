#include "cr_std_filesystem.h"
#include "cr_std_logger.h"
#include "cr_std_string.h"
#include "cr_std_utils.h"
#include "cr_std_vector.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

string_t *cr_std_filesystem_read_file_as_string(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_read_file_as_string -> file cant be found -> %s", file_path);
        fclose(file);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(file_size + 1);
    if (!buffer) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_read_file_as_string -> failed to allocate memory for buffer");
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';

    fclose(file);
    return cr_std_string_new(buffer);
}

vector_t *cr_std_filesystem_read_file_as_vector(const char *file_path) {
    FILE *file = fopen(file_path, "r");

    if (!file) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_read_file_as_vector -> failed to find file %s", file_path);
        return NULL;
    }

    vector_t *vector = cr_std_vector_new(sizeof(string_t *), cr_std_string_free_ptr);
    if (!vector) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_read_file_as_vector -> failed to allocate memory for vector_t struct");
        fclose(file);
        return NULL;
    }

    char *line;
    size_t line_length = 0;
    ssize_t current_index = getline(&line, &line_length, file);

    while (current_index != -1) {
        if (current_index > 0 && line[current_index - 1] == '\n') {
            line[current_index - 1] = '\0';
        }

        string_t *new_string = cr_std_string_new(line);
        if (!new_string) {
            cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_read_file_as_vector -> failed to allocate memory for string_t struct");
            free(line);
            fclose(file);
            cr_std_vector_free(&vector);
            return NULL;
        }
        cr_std_vector_push_back(vector, new_string);
        current_index = getline(&line, &line_length, file);
    }

    free(line);
    fclose(file);
    return vector;
}

vector_t *cr_std_filesystem_get_dirs(const char *file_path) {
    DIR *dir = opendir(file_path);
    if (!dir) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_get_dirs -> failed to open dir -> %s", file_path);
        return NULL;
    }

    // Unwanted entries.
    string_t *current_dir = cr_std_string_new(".");
    string_t *parent_dir = cr_std_string_new("..");

    vector_t *vector = cr_std_vector_new(sizeof(struct dirent *), cr_std_free_ptr);

    struct dirent *entry = readdir(dir);
    while (entry != NULL) {
        if (entry->d_type == DT_DIR) {
            string_t *file_name = cr_std_string_new(entry->d_name);
            if (cr_std_string_compare(file_name, current_dir) != 1 &&
                cr_std_string_compare(file_name, parent_dir) != 1) {

                struct dirent *allocated_entry = malloc(sizeof(struct dirent));
                if (allocated_entry != NULL) {
                    memcpy(allocated_entry, entry, sizeof(struct dirent));
                    cr_std_vector_push_back(vector, allocated_entry);
                }
            }
            cr_std_string_free(&file_name);
        }
        entry = readdir(dir);
    }

    closedir(dir);
    cr_std_string_free(&current_dir);
    cr_std_string_free(&parent_dir);

    return vector;
}

vector_t *cr_std_filesystem_get_dir_files(const char *file_path) {
    DIR *dir = opendir(file_path);
    if (!dir) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_get_dir_files -> failed to open dir -> %s", file_path);
        return NULL;
    }

    vector_t *vector = cr_std_vector_new(sizeof(struct dirent *), cr_std_free_ptr);

    struct dirent *entry = readdir(dir);
    while (entry != NULL) {
        if (entry->d_type != DT_DIR) {
            string_t *file_name = cr_std_string_new(entry->d_name);
            struct dirent *allocated_entry = malloc(sizeof(struct dirent));
            if (allocated_entry != NULL) {
                memcpy(allocated_entry, entry, sizeof(struct dirent));
                cr_std_vector_push_back(vector, allocated_entry);
            }
            cr_std_string_free(&file_name);
        }
        entry = readdir(dir);
    }

    closedir(dir);
    return vector;
}

vector_t *cr_std_filesystem_get_dir_files_r(const char *file_path) {
    DIR *dir = opendir(file_path);
    if (!dir) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_get_dir_files_r -> failed to open dir -> %s", file_path);
        return NULL;
    }

    vector_t *vector = cr_std_vector_new(sizeof(struct dirent *), cr_std_free_ptr);

    string_t *current_dir = cr_std_string_new(".");
    string_t *parent_dir = cr_std_string_new("..");

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {

        string_t *file_name = cr_std_string_new(entry->d_name);
        if (cr_std_string_compare(file_name, current_dir) == 1 ||
            cr_std_string_compare(file_name, parent_dir) == 1) {
            continue;
        }

        string_t *full_path = cr_std_string_newf("%s/%s", file_path, file_name->c_str);

        if (!full_path) {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "Memory allocation failed for path -> %s/%s", file_path, file_name->c_str);
            continue;
        }

        if (entry->d_type == DT_DIR) {
            vector_t *sub_dir_vector = cr_std_filesystem_get_dir_files_r(full_path->c_str);
            if (sub_dir_vector) {
                for (size_t i = 0; i < sub_dir_vector->size; i++) {
                    struct dirent *sub_entry = (struct dirent *)cr_std_vector_get_element(sub_dir_vector, i);
                    cr_std_vector_push_back(vector, sub_entry);
                }
                free(sub_dir_vector);
            }
        } else {
            struct dirent *allocated_entry = malloc(sizeof(struct dirent));
            if (allocated_entry != NULL) {
                memcpy(allocated_entry, entry, sizeof(struct dirent));
                cr_std_vector_push_back(vector, allocated_entry);
            }
        }
        cr_std_string_free(&full_path);
    }

    closedir(dir);
    cr_std_string_free(&current_dir);
    cr_std_string_free(&parent_dir);
    return vector;
}

