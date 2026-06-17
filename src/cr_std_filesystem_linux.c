#include "cr_std_arena.h"
#if defined(__linux__) || defined(__APPLE__)

#include "cr_std_filesystem.h"
#include "cr_std_logger.h"
#include "cr_std_string.h"
#include "cr_std_utils.h"
#include "cr_std_vector.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>

String *cr_std_filesystem_get_cwd(Arena *arena) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_filesystem_get_cwd -> arena* was NULL");
        return NULL;
    }

    char cwd[CR_STD_PATH_MAX_SIZE];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        CR_LOG_ERROR("Failed to get current working directory");
        return NULL;
    }

    return cr_std_string_new(arena, cwd);
}

int cr_std_filesystem_make_dir(const char *dir_path, mode_t permissions) {
    if (mkdir(dir_path, permissions) == 0) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_INFO,
                           "cr_std_filesystem_make_dir -> Directory created '%s'", dir_path);
        return 0;
    } else {
        if (errno == EEXIST) {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_WARNING,
                               "cr_std_filesystem_make_dir -> Directory already exists '%s'",
                               dir_path);
        } else if (errno == EACCES) {
            cr_std_logger_outf(
            CR_STD_LOGGER_LOG_TYPE_ERROR,
            "cr_std_filesystem_make_dir -> Permission denied could not create directory '%s'",
            dir_path);
        } else if (errno == ENOENT) {
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
    if (!arena) {
        CR_LOG_ERROR("cr_std_filesystem_get_entries -> arena* was NULL");
        return NULL;
    }

    DIR *dir = opendir(file_path);
    if (!dir) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR,
                           "cr_std_filesystem_get_entries -> failed to open dir -> %s", file_path);
        return NULL;
    }

    Vector *vector = cr_std_vector_new(arena);

    String *current_dir = cr_std_string_new(arena, ".");
    String *parent_dir = cr_std_string_new(arena, "..");

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        String *file_name = cr_std_string_new(arena, entry->d_name);
        // Skip current and parent directory entries
        if (cr_std_string_compare(file_name, current_dir) == 1 ||
            cr_std_string_compare(file_name, parent_dir) == 1) {
            continue;
        }

        String *full_path = cr_std_string_newf(arena, "%s/%s", file_path, file_name->c_str);
        if (!full_path) {
            cr_std_logger_outf(
            CR_STD_LOGGER_LOG_TYPE_ERROR,
            "cr_std_filesystem_get_entries -> memory allocation failed for path -> %s/%s",
            file_path, file_name->c_str);
            continue;
        }

        Dirent *custom_entry = cr_std_arena_alloc(arena, sizeof(*custom_entry));
        if (!custom_entry) {
            cr_std_logger_outf(
            CR_STD_LOGGER_LOG_TYPE_ERROR,
            "cr_std_filesystem_get_entries -> memory allocation failed for Dirent");
            continue;
        }

        custom_entry->d_name = file_name;
        custom_entry->d_path = cr_std_string_make_copy(arena, full_path);
        custom_entry->d_type = entry->d_type;
        custom_entry->d_hidden = (file_name->c_str[0] == '.') ? 1 : 0;

        struct stat file_stat;
        if (stat(full_path->c_str, &file_stat) == 0) {
            if (!S_ISDIR(file_stat.st_mode) && cr_std_string_find_char_last(file_name, '.') != -1) {
                custom_entry->d_ext = cr_std_string_sub_string(
                arena, file_name, cr_std_string_find_char_last(file_name, '.'), file_name->length);
            } else {
                custom_entry->d_ext = cr_std_string_new(arena, "N/A");
            }

            custom_entry->d_size = file_stat.st_size;
            custom_entry->d_inode = file_stat.st_ino;
            custom_entry->d_permissions = file_stat.st_mode;
        } else {
            custom_entry->d_ext = cr_std_string_new(arena, "N/A");
            custom_entry->d_size = -1;
            custom_entry->d_inode = 0;
            custom_entry->d_permissions = 0;
        }

        // Only push the entry to the vector if it matches the include conditions
        if ((S_ISDIR(file_stat.st_mode) && include_dirs) ||
            (!S_ISDIR(file_stat.st_mode) && include_files)) {
            cr_std_vector_push_back(arena, vector, custom_entry);
        }

        // Recursive call for directories if enabled
        if (recursive && S_ISDIR(file_stat.st_mode)) {
            Vector *sub_dir_vector = cr_std_filesystem_get_entries(
            arena, full_path->c_str, include_files, include_dirs, recursive);
            if (sub_dir_vector) {
                cr_std_vector_extend(arena, vector, sub_dir_vector);
            }
        }
    }

    closedir(dir);
    return vector;
}
#endif // __linux__ __APPLE__
