#ifdef _WIN32
#include "cr_std_arena.h"
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
#include <direct.h>
#include <windows.h>

String *cr_std_filesystem_get_cwd(Arena *arena) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_filesystem_get_cwd -> arena* was NULL");
        return NULL;
    }

    char cwd[CR_STD_PATH_MAX_SIZE];

    if (_getcwd(cwd, sizeof(cwd)) == NULL) {
        CR_LOG_ERROR("Failed to get current working directory");
        return NULL;
    }

    return cr_std_string_new(arena, cwd);
}

int cr_std_filesystem_make_dir(const char *dir_path, mode_t permissions) {
    if (CreateDirectory(dir_path, NULL) != 0) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_INFO,
                           "cr_std_filesystem_make_dir -> Directory created '%s'", dir_path);
        return 0;
    } else {
        DWORD error = GetLastError();
        if (error == ERROR_ALREADY_EXISTS) {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_WARNING,
                               "cr_std_filesystem_make_dir -> Directory already exists '%s'",
                               dir_path);
        } else if (error == ERROR_ACCESS_DENIED) {
            cr_std_logger_outf(
            CR_STD_LOGGER_LOG_TYPE_ERROR,
            "cr_std_filesystem_make_dir -> Permission denied could not create directory '%s'",
            dir_path);
        } else if (error == ERROR_FILE_NOT_FOUND) {
            cr_std_logger_outf(
            CR_STD_LOGGER_LOG_TYPE_ERROR,
            "cr_std_filesystem_make_dir -> Part of the directory could not be found '%s'",
            dir_path);
        } else {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR,
                               "cr_std_filesystem_make_dir -> Failed to create directory");
        }
        return 1;
    }
}

Vector *cr_std_filesystem_get_entries(Arena *arena,
                                      const char *file_path,
                                      bool include_files,
                                      bool include_dirs,
                                      bool recursive) {
    char search_path[MAX_PATH];
    snprintf(search_path, MAX_PATH, "%s\\*", file_path);

    WIN32_FIND_DATA findData;

    HANDLE hFind = FindFirstFile(search_path, &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR,
                           "cr_std_filesystem_get_entries -> failed to open dir -> %s", file_path);
        return NULL;
    }

    Vector *vector = cr_std_vector_new(arena);

    // Temp arena for temporary strings
    Arena *temp_arena = cr_std_arena_new(CR_STD_MB);
    if (!temp_arena) {
        CR_LOG_ERROR("cr_std_filesystem_get_entries -> failed to create temp arena");
        FindClose(hFind);
        return vector;
    }

    String *current_dir = cr_std_string_new(temp_arena, ".");
    String *parent_dir = cr_std_string_new(temp_arena, "..");
    size_t current_mark = cr_std_arena_get_mark(temp_arena);

    do {
        cr_std_arena_reset_to_mark(temp_arena, current_mark);
        String *file_name = cr_std_string_new(temp_arena, findData.cFileName);
        if (cr_std_string_compare(file_name, current_dir) == 1 ||
            cr_std_string_compare(file_name, parent_dir) == 1) {
            continue;
        }

        String *full_path = cr_std_string_newf(temp_arena, "%s\\%s", file_path, file_name->c_str);
        if (!full_path) {
            CR_LOG_ERROR_FMT("cr_std_filesystem_get_entries -> failed to create path: %s\\%s",
                             file_path, file_name->c_str);
            continue;
        }

        Dirent *custom_entry = cr_std_arena_alloc(arena, sizeof(*custom_entry));
        if (!custom_entry) {
            CR_LOG_ERROR("cr_std_filesystem_get_entries -> failed to allocate Dirent");
            continue;
        }

        custom_entry->d_name = cr_std_string_make_copy(arena, file_name);
        custom_entry->d_path = cr_std_string_make_copy(arena, full_path);
        custom_entry->d_type =
        (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? DT_DIR : DT_REG;
        custom_entry->d_hidden = (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) ? 1 : 0;

        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
            cr_std_string_find_char_last(file_name, '.') != -1) {
            String *temp_ext = cr_std_string_sub_string(
            temp_arena, file_name, cr_std_string_find_char_last(file_name, '.'), file_name->length);
            custom_entry->d_ext = cr_std_string_make_copy(arena, temp_ext);
        } else {
            custom_entry->d_ext = cr_std_string_new(arena, "N/A");
        }

        custom_entry->d_size = ((long long)findData.nFileSizeHigh << 32) | findData.nFileSizeLow;
        custom_entry->d_inode = 0;
        custom_entry->d_permissions = findData.dwFileAttributes;

        // Only push the entry to the vector if it matches the include conditions
        if (((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && include_dirs) ||
            (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && include_files)) {
            cr_std_vector_push_back(arena, vector, custom_entry);
        }

        if (recursive && (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            Vector *sub_dir_vector = cr_std_filesystem_get_entries(
            arena, full_path->c_str, include_files, include_dirs, recursive);
            if (sub_dir_vector) {
                cr_std_vector_extend(arena, vector, sub_dir_vector);
            }
        }

    } while (FindNextFile(hFind, &findData) != 0);

    if (GetLastError() != ERROR_NO_MORE_FILES) {
        CR_LOG_ERROR_FMT("cr_std_filesystem_get_entries -> error reading directory: %s", file_path);
    }

    FindClose(hFind);
    cr_std_arena_free(&temp_arena);
    return vector;
}
#endif // _WIN32
