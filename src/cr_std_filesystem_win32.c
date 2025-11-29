#ifdef _WIN32

#include "cr_std_filesystem.h"
#include "cr_std_logger.h"
#include "cr_std_string.h"
#include "cr_std_utils.h"
#include "cr_std_vector.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

// Windows Specific Functions
#include <windows.h>
#include <direct.h>

String *cr_std_filesystem_get_cwd() {
    char cwd[CR_STD_PATH_MAX_SIZE];

    if (_getcwd(cwd, sizeof(cwd)) == NULL) {
        CR_LOG_ERROR("Failed to get current working directory");
        return NULL;
    }

    return cr_std_string_new(cwd);
}

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

        String *full_path = cr_std_string_newf("%s\\%s", file_path, file_name->c_str);
        if (!full_path) {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_get_entries -> memory allocation failed for path -> %s\\%s", file_path, file_name->c_str);
            cr_std_string_free(&file_name);
            continue;
        }

        Dirent *custom_entry = malloc(sizeof(Dirent));
        if (!custom_entry) {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_get_entries -> memory allocation failed for Dirent");
            cr_std_string_free(&file_name);
            cr_std_string_free(&full_path);
            continue;
        }

        custom_entry->d_name = file_name;
        custom_entry->d_path = cr_std_string_make_copy(full_path);
        custom_entry->d_type = (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? DT_DIR : DT_REG;
        custom_entry->d_hidden = (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) ? 1 : 0;

        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && cr_std_string_find_char_last(file_name, '.') != -1) {
            custom_entry->d_ext = cr_std_string_sub_string(file_name, cr_std_string_find_char_last(file_name, '.'), file_name->length);
        } else {
            custom_entry->d_ext = cr_std_string_new("N/A");
        }

        custom_entry->d_size = ((long long)findData.nFileSizeHigh << 32) | findData.nFileSizeLow;
        custom_entry->d_inode = 0;
        custom_entry->d_permissions = findData.dwFileAttributes;

        // Only push the entry to the vector if it matches the include conditions
        if (((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && include_dirs) || (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && include_files)) {
            cr_std_vector_push_back(vector, custom_entry);
        } else {
            // If the entry is not included, free the memory
            cr_std_filesystem_dirent_free(&custom_entry);
        }

        if (recursive && (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            Vector *sub_dir_vector = cr_std_filesystem_get_entries(full_path->c_str, include_files, include_dirs, recursive);
            if (sub_dir_vector) {
                cr_std_vector_extend(vector, sub_dir_vector);
                free(sub_dir_vector);
            }
        }

        cr_std_string_free(&full_path);
    } while (FindNextFile(hFind, &findData) != 0);

    if (GetLastError() != ERROR_NO_MORE_FILES) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_filesystem_get_entries -> error reading directory: %s", file_path);
    }

    FindClose(hFind);
    cr_std_string_free(&current_dir);
    cr_std_string_free(&parent_dir);

    return vector;
}

#endif // _WIN32
