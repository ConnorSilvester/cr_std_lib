#include "cr_std_filesystem.h"
#include "cr_std_logger.h"
#include "cr_std_string.h"
#include "cr_std_vector.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

String *cr_std_filesystem_get_current_time_date(const char *time_date_format) {
    time_t rawtime;
    struct tm *timeinfo;
    char time_str[256];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    if (strftime(time_str, sizeof(time_str), time_date_format, timeinfo) == 0) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_get_current_time -> strftime failed to format the format string");
        return NULL;
    }

    return cr_std_string_new(time_str);
}

int cr_std_filesystem_copy_file(const char *src, const char *dest) {

    FILE *src_file = fopen(src, "rb");
    if (!src_file) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_copy_file -> file can't be opened -> %s", src);
        return 1;
    }

    FILE *dest_file = fopen(dest, "wb");
    if (!dest_file) {
        fclose(src_file);
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_copy_file -> file can't be opened -> %s", dest);
        return 1;
    }

    int ch;
    while ((ch = fgetc(src_file)) != EOF) {
        if (fputc(ch, dest_file) == EOF) {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_copy_file -> error writing to file -> %s", dest);
            fclose(src_file);
            fclose(dest_file);
            return 1;
        }
    }

    if (ferror(src_file)) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_copy_file -> error reading from file -> %s", src);
        fclose(src_file);
        fclose(dest_file);
        return 1;
    }

    fclose(dest_file);
    fclose(src_file);

    return 0;
}

int cr_std_filesystem_move_file(const char *src, const char *dest) {
    if (rename(src, dest) == 0) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_INFO, "cr_std_filesystem_move_file -> File moved from '%s' to '%s' successfully", src, dest);
        return 0;
    } else {
        if (errno == ENOENT) {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_move_file -> Source file '%s' or destination '%s' does not exist", src, dest);
        } else if (errno == EACCES) {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_move_file -> Permission denied when accessing '%s' or '%s'", src, dest);
        } else if (errno == ENOTDIR) {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_move_file -> Part of the path does not exist in '%s' or '%s'", src, dest);
        } else if (errno == EEXIST) {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_move_file -> The destination file '%s' already exists", dest);
        } else if (errno == EXDEV) {
            cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_move_file -> Can not move file across different filesystems");
        } else {
            cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_move_file -> Failed to move file");
        }
        return 1;
    }
}

#ifdef _WIN32
// Windows Specific Functions
#include <windows.h>

int cr_std_filesystem_make_dir(const char *dir_path) {
    if (CreateDirectory(dir_path, NULL) != 0) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_INFO, "cr_std_filesystem_make_dir -> Directory created '%s'", dir_path);
        return 0;
    } else {
        DWORD error = GetLastError();
        if (error == ERROR_ALREADY_EXISTS) {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_WARNING, "cr_std_filesystem_make_dir -> Directory already exists '%s'", dir_path);
        } else if (error == ERROR_ACCESS_DENIED) {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_make_dir -> Permission denied could not create directory '%s'", dir_path);
        } else if (error == ERROR_FILE_NOT_FOUND) {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_make_dir -> Part of the directory could not be found '%s'", dir_path);
        } else {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_make_dir -> Failed to create directory");
        }
        return 1;
    }
}

Vector *cr_std_filesystem_get_entries(const char *file_path, bool include_files, bool include_dirs, bool recursive) {
    char search_path[MAX_PATH];
    snprintf(search_path, MAX_PATH, "%s\\*", file_path);

    WIN32_FIND_DATA findData;

    HANDLE hFind = FindFirstFile(search_path, &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_get_entries -> failed to open dir -> %s", file_path);
        return NULL;
    }

    Vector *vector = cr_std_vector_new(Dirent *);
    vector->free_function = cr_std_filesystem_dirent_free_ptr;

    String *current_dir = cr_std_string_new(".");
    String *parent_dir = cr_std_string_new("..");

    do {
        String *file_name = cr_std_string_new(findData.cFileName);
        if (cr_std_string_compare(file_name, current_dir) == 1 ||
            cr_std_string_compare(file_name, parent_dir) == 1) {
            cr_std_string_free(&file_name);
            continue;
        }

        // Construct full path to the entry
        String *full_path = cr_std_string_newf("%s\\%s", file_path, file_name->c_str);
        if (!full_path) {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_get_entries -> memory allocation failed for path -> %s\\%s", file_path, file_name->c_str);
            cr_std_string_free(&file_name);
            continue;
        }

        // Allocate and initialize a new Dirent structure
        Dirent *custom_entry = malloc(sizeof(Dirent));
        if (!custom_entry) {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_get_entries -> memory allocation failed for Dirent");
            cr_std_string_free(&file_name);
            cr_std_string_free(&full_path);
            continue;
        }

        // Initialize fields in Dirent
        custom_entry->d_name = file_name;
        custom_entry->d_path = cr_std_string_make_copy(full_path);
        custom_entry->d_type = (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? DT_DIR : DT_REG;
        custom_entry->d_hidden = (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) ? 1 : 0;

        // Set file extension
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && cr_std_string_find_char_last(file_name, '.') != -1) {
            custom_entry->d_ext = cr_std_string_sub_string(file_name, cr_std_string_find_char_last(file_name, '.'), file_name->length);
        } else {
            custom_entry->d_ext = cr_std_string_new("N/A");
        }

        // Get file stats
        custom_entry->d_size = ((long long)findData.nFileSizeHigh << 32) | findData.nFileSizeLow;
        custom_entry->d_inode = 0; // Windows does not have inodes
        custom_entry->d_permissions = findData.dwFileAttributes;

        // Only push the entry to the vector if it matches the include conditions
        if (((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && include_dirs) || (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && include_files)) {
            cr_std_vector_push_back(vector, custom_entry);
        } else {
            // If the entry is not included, free the memory
            cr_std_filesystem_dirent_free(&custom_entry);
        }

        // Recursive call for directories if enabled
        if (recursive && (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            Vector *sub_dir_vector = cr_std_filesystem_get_entries(full_path->c_str, include_files, include_dirs, recursive);
            if (sub_dir_vector) {
                cr_std_vector_extend(vector, sub_dir_vector);
                free(sub_dir_vector);
            }
        }

        cr_std_string_free(&full_path);
    } while (FindNextFile(hFind, &findData) != 0);

    // Check if the loop ended due to an error
    if (GetLastError() != ERROR_NO_MORE_FILES) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_get_entries -> error reading directory: %s", file_path);
    }

    // Close the search handle
    FindClose(hFind);
    cr_std_string_free(&current_dir);
    cr_std_string_free(&parent_dir);

    return vector;
}

#else
// Clashing Linux Functions
#include <dirent.h>

int cr_std_filesystem_make_dir(const char *dir_path, mode_t permissions) {
    if (mkdir(dir_path, permissions) == 0) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_INFO, "cr_std_filesystem_make_dir -> Directory created '%s'", dir_path);
        return 0;
    } else {
        if (errno == EEXIST) {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_WARNING, "cr_std_filesystem_make_dir -> Directory already exists '%s'", dir_path);
        } else if (errno == EACCES) {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_make_dir -> Permission denied could not create directory '%s'", dir_path);
        } else if (errno == ENOENT) {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_make_dir -> Part of the directory could not be found '%s'", dir_path);
        } else {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_make_dir -> Failed to create directory");
        }
        return 1;
    }
}

Vector *cr_std_filesystem_get_entries(const char *file_path, bool include_files, bool include_dirs, bool recursive) {
    DIR *dir = opendir(file_path);
    if (!dir) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_get_entries -> failed to open dir -> %s", file_path);
        return NULL;
    }

    // Create a vector to store entries of type Dirent*
    Vector *vector = cr_std_vector_new(Dirent *);
    vector->free_function = cr_std_filesystem_dirent_free_ptr;

    String *current_dir = cr_std_string_new(".");
    String *parent_dir = cr_std_string_new("..");

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        String *file_name = cr_std_string_new(entry->d_name);
        // Skip current and parent directory entries
        if (cr_std_string_compare(file_name, current_dir) == 1 ||
            cr_std_string_compare(file_name, parent_dir) == 1) {
            cr_std_string_free(&file_name);
            continue;
        }

        // Construct full path to the entry
        String *full_path = cr_std_string_newf("%s/%s", file_path, file_name->c_str);
        if (!full_path) {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_get_entries -> memory allocation failed for path -> %s/%s", file_path, file_name->c_str);
            cr_std_string_free(&file_name);
            continue;
        }

        // Allocate and initialize a new Dirent structure
        Dirent *custom_entry = malloc(sizeof(Dirent));
        if (!custom_entry) {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_get_entries -> memory allocation failed for Dirent");
            cr_std_string_free(&file_name);
            cr_std_string_free(&full_path);
            continue;
        }

        // Initialize fields in Dirent
        custom_entry->d_name = file_name;
        custom_entry->d_path = cr_std_string_make_copy(full_path);
        custom_entry->d_type = entry->d_type;
        custom_entry->d_hidden = (file_name->c_str[0] == '.') ? 1 : 0;

        // Set file extension
        struct stat file_stat;
        if (stat(full_path->c_str, &file_stat) == 0) {
            if (!S_ISDIR(file_stat.st_mode) && cr_std_string_find_char_last(file_name, '.') != -1) {
                custom_entry->d_ext = cr_std_string_sub_string(file_name, cr_std_string_find_char_last(file_name, '.'), file_name->length);
            } else {
                custom_entry->d_ext = cr_std_string_new("N/A");
            }

            // Get file stats
            custom_entry->d_size = file_stat.st_size;
            custom_entry->d_inode = file_stat.st_ino;
            custom_entry->d_permissions = file_stat.st_mode;
        } else {
            custom_entry->d_ext = cr_std_string_new("N/A");
            custom_entry->d_size = -1;
            custom_entry->d_inode = 0;
            custom_entry->d_permissions = 0;
        }

        // Only push the entry to the vector if it matches the include conditions
        if ((S_ISDIR(file_stat.st_mode) && include_dirs) || (!S_ISDIR(file_stat.st_mode) && include_files)) {
            cr_std_vector_push_back(vector, custom_entry);
        } else {
            // If the entry is not included, free the memory
            cr_std_filesystem_dirent_free(&custom_entry);
        }

        // Recursive call for directories if enabled
        if (recursive && S_ISDIR(file_stat.st_mode)) {
            Vector *sub_dir_vector = cr_std_filesystem_get_entries(full_path->c_str, include_files, include_dirs, recursive);
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
#endif // _WIN32

int cr_std_filesystem_write_file_operations(const char *file_path, const char *data, const char *mode) {
    if (!data || !file_path || !mode) {
        return 1;
    }

    FILE *file = fopen(file_path, mode);
    if (!file) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_write_file_operations -> file can't be opened -> %s", file_path);
        return 1;
    }

    if (fputs(data, file) == EOF) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_write_file_operations  -> failed to write data");
        fclose(file);
        return 1;
    }

    fclose(file);
    return 0;
}

int cr_std_filesystem_write_to_file(const char *file_path, const char *data) {
    return cr_std_filesystem_write_file_operations(file_path, data, "w");
}

int cr_std_filesystem_append_to_file(const char *file_path, const char *data) {
    return cr_std_filesystem_write_file_operations(file_path, data, "a");
}

String *cr_std_filesystem_read_file_as_string(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_read_file_as_string -> file can't be found -> %s", file_path);
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
    String *result = cr_std_string_new(buffer);
    free(buffer);
    return result;
}

Vector *cr_std_filesystem_read_file_as_vector(const char *file_path) {
    String *file_contents = cr_std_filesystem_read_file_as_string(file_path);
    if (!file_contents) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_read_file_as_vector -> failed to read file contents -> %s", file_path);
        return NULL;
    }

    Vector *file_lines = cr_std_string_split_hard(file_contents, '\n');
    if (!file_lines) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_read_file_as_vector -> failed to split file contents -> %s", file_path);
    }
    cr_std_string_free(&file_contents);
    return file_lines;
}

Vector *cr_std_filesystem_get_dirs(const char *file_path) {
    return cr_std_filesystem_get_entries(file_path, false, true, false);
}

Vector *cr_std_filesystem_get_dirs_r(const char *file_path) {
    return cr_std_filesystem_get_entries(file_path, false, true, true);
}

Vector *cr_std_filesystem_get_dir_files(const char *file_path) {
    return cr_std_filesystem_get_entries(file_path, true, false, false);
}

Vector *cr_std_filesystem_get_dir_files_r(const char *file_path) {
    return cr_std_filesystem_get_entries(file_path, true, false, true);
}

int cr_std_filesystem_dirent_free(Dirent **dirent_ptr) {
    if (dirent_ptr && *dirent_ptr) {
        Dirent *entry = *dirent_ptr;
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
        return 0;
    }
    cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_WARNING, "cr_std_filesystem_dirent_free -> tried to free a NULL dirent_ptr*");
    return 1;
}
