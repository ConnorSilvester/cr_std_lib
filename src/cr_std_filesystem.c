#include "cr_std_filesystem.h"
#include "cr_std_logger.h"
#include "cr_std_string.h"
#include "cr_std_vector.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int cr_std_filesystem_write_file_operations(const char *file_path, const char *data, const char *mode) {
    if (!data || !file_path || !mode) {
        return 0;
    }

    FILE *file = fopen(file_path, mode);
    if (!file) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_write_file_operations -> file can't be opened -> %s", file_path);
        return 0;
    }

    if (fputs(data, file) == EOF) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_write_file_operations  -> failed to write data");
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}

int cr_std_filesystem_write_to_file(const char *file_path, const char *data) {
    return cr_std_filesystem_write_file_operations(file_path, data, "w");
}

int cr_std_filesystem_append_to_file(const char *file_path, const char *data) {
    return cr_std_filesystem_write_file_operations(file_path, data, "a");
}

string_t *cr_std_filesystem_read_file_as_string(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_read_file_as_string -> file can't be found -> %s", file_path);
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

    vector_t *vector = cr_std_vector_new(sizeof(string_t *), cr_std_string_free_ptr, NULL);
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

vector_t *cr_std_filesystem_get_entries(const char *file_path, bool include_files, bool include_dirs, bool recursive) {
    DIR *dir = opendir(file_path);
    if (!dir) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_get_entries -> failed to open dir -> %s", file_path);
        return NULL;
    }

    // Create a vector to store entries of type dirent_t*
    vector_t *vector = cr_std_vector_new(sizeof(dirent_t *), cr_std_filesystem_dirent_free_ptr, NULL);
    string_t *current_dir = cr_std_string_new(".");
    string_t *parent_dir = cr_std_string_new("..");

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        string_t *file_name = cr_std_string_new(entry->d_name);
        // Skip current and parent directory entries
        if (cr_std_string_compare(file_name, current_dir) == 1 ||
            cr_std_string_compare(file_name, parent_dir) == 1) {
            cr_std_string_free(&file_name);
            continue;
        }

        // Construct full path to the entry
        string_t *full_path = cr_std_string_newf("%s/%s", file_path, file_name->c_str);
        if (!full_path) {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_get_entries -> memory allocation failed for path -> %s/%s", file_path, file_name->c_str);
            cr_std_string_free(&file_name);
            continue;
        }

        // Allocate and initialize a new dirent_t structure
        dirent_t *custom_entry = malloc(sizeof(dirent_t));
        if (!custom_entry) {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_get_entries -> memory allocation failed for dirent_t");
            cr_std_string_free(&file_name);
            cr_std_string_free(&full_path);
            continue;
        }

        // Initialize fields in dirent_t
        custom_entry->d_name = file_name;
        custom_entry->d_path = cr_std_string_make_copy(full_path);
        custom_entry->d_type = entry->d_type;
        custom_entry->d_hidden = (file_name->c_str[0] == '.') ? 1 : 0;

        // Set file extension
        if (entry->d_type != DT_DIR && cr_std_string_find_char_last(file_name, '.') != -1) {
            custom_entry->d_ext = cr_std_string_sub_string(file_name, cr_std_string_find_char_last(file_name, '.'), file_name->length);
        } else {
            custom_entry->d_ext = cr_std_string_new("N/A");
        }

        // Get file stats
        struct stat file_stat;
        if (stat(full_path->c_str, &file_stat) == 0) {
            custom_entry->d_size = file_stat.st_size;
            custom_entry->d_inode = file_stat.st_ino;
            custom_entry->d_permissions = file_stat.st_mode;
        } else {
            custom_entry->d_size = -1;
            custom_entry->d_inode = 0;
            custom_entry->d_permissions = 0;
        }

        // Only push the entry to the vector if it matches the include conditions
        if ((entry->d_type == DT_DIR && include_dirs) || (entry->d_type != DT_DIR && include_files)) {
            cr_std_vector_push_back(vector, custom_entry);
        } else {
            // If the entry is not included, free the memory
            cr_std_filesystem_dirent_free(&custom_entry);
        }

        // Recursive call for directories if enabled
        if (recursive && entry->d_type == DT_DIR) {
            vector_t *sub_dir_vector = cr_std_filesystem_get_entries(full_path->c_str, include_files, include_dirs, recursive);
            if (sub_dir_vector) {
                cr_std_vector_extend(vector, sub_dir_vector);
                free(sub_dir_vector);
            }
        }
        cr_std_string_free(&full_path);
    }

    closedir(dir);
    cr_std_string_free(&current_dir);
    cr_std_string_free(&parent_dir);

    return vector;
}

vector_t *cr_std_filesystem_get_dirs(const char *file_path) {
    return cr_std_filesystem_get_entries(file_path, false, true, false);
}

vector_t *cr_std_filesystem_get_dirs_r(const char *file_path) {
    return cr_std_filesystem_get_entries(file_path, false, true, true);
}

vector_t *cr_std_filesystem_get_dir_files(const char *file_path) {
    return cr_std_filesystem_get_entries(file_path, true, false, false);
}

vector_t *cr_std_filesystem_get_dir_files_r(const char *file_path) {
    return cr_std_filesystem_get_entries(file_path, true, false, true);
}

int cr_std_filesystem_dirent_free(dirent_t **dirent_ptr) {
    if (dirent_ptr && *dirent_ptr) {
        dirent_t *entry = *dirent_ptr;
        if (entry->d_name) {
            cr_std_string_free(&(entry->d_name));
        }
        if (entry->d_path) {
            cr_std_string_free(&(entry->d_path));
        }
        if (entry->d_ext) {
            cr_std_string_free(&(entry->d_ext));
        }
        free(entry);
        *dirent_ptr = NULL;
        return 1;
    }
    cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_WARNING, "cr_std_filesystem_dirent_free -> tried to free a NULL dirent_ptr*");
    return 0;
}
