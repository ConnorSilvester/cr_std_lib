#include "cr_std_filesystem.h"
#include "cr_std_arena.h"
#include "cr_std_logger.h"
#include "cr_std_string.h"
#include "cr_std_utils.h"
#include "cr_std_vector.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

String *cr_std_filesystem_get_current_time_date(Arena *arena, const char *time_date_format) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_filesystem_get_current_time -> arena* was NULL");
        return NULL;
    }

    if (!time_date_format) {
        CR_LOG_ERROR("cr_std_filesystem_get_current_time -> time_date_format* was NULL");
        return NULL;
    }

    time_t rawtime;
    struct tm *timeinfo;
    char time_str[256];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    if (strftime(time_str, sizeof(time_str), time_date_format, timeinfo) == 0) {
        CR_LOG_ERROR(
        "cr_std_filesystem_get_current_time -> strftime failed to format the format string");
        return NULL;
    }

    return cr_std_string_new(arena, time_str);
}

b8 cr_std_filesystem_copy_file(const char *src, const char *dest) {

    FILE *src_file = fopen(src, "rb");
    if (!src_file) {
        CR_LOG_ERROR_FMT("cr_std_filesystem_copy_file -> file can't be opened -> %s", src);
        return CR_STD_FAIL;
    }

    FILE *dest_file = fopen(dest, "wb");
    if (!dest_file) {
        fclose(src_file);
        CR_LOG_ERROR_FMT("cr_std_filesystem_copy_file -> file can't be opened -> %s", dest);
        return CR_STD_FAIL;
    }

    setvbuf(src_file, NULL, _IOFBF, 64 * CR_STD_KB);
    setvbuf(dest_file, NULL, _IOFBF, 64 * CR_STD_KB);

    char buffer[64 * CR_STD_KB];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), src_file)) > 0) {
        if (fwrite(buffer, 1, bytes, dest_file) != bytes) {
            CR_LOG_ERROR_FMT("cr_std_filesystem_copy_file -> error writing to file -> %s", dest);
            fclose(src_file);
            fclose(dest_file);
            return CR_STD_FAIL;
        }
    }

    if (ferror(src_file)) {
        CR_LOG_ERROR_FMT("cr_std_filesystem_copy_file -> error reading from file -> %s", src);
        fclose(src_file);
        fclose(dest_file);
        return CR_STD_FAIL;
    }

    fclose(dest_file);
    fclose(src_file);

    return CR_STD_OK;
}

b8 cr_std_filesystem_move_file(const char *src, const char *dest) {
    if (rename(src, dest) == 0) {
        cr_std_logger_outf(
        CR_STD_LOGGER_LOG_TYPE_INFO,
        "cr_std_filesystem_move_file -> File moved from '%s' to '%s' successfully", src, dest);
        return CR_STD_OK;
    } else {
        if (errno == ENOENT) {
            cr_std_logger_outf(
            CR_STD_LOGGER_LOG_TYPE_ERROR,
            "cr_std_filesystem_move_file -> Source file '%s' or destination '%s' does not exist",
            src, dest);
        } else if (errno == EACCES) {
            cr_std_logger_outf(
            CR_STD_LOGGER_LOG_TYPE_ERROR,
            "cr_std_filesystem_move_file -> Permission denied when accessing '%s' or '%s'", src,
            dest);
        } else if (errno == ENOTDIR) {
            cr_std_logger_outf(
            CR_STD_LOGGER_LOG_TYPE_ERROR,
            "cr_std_filesystem_move_file -> Part of the path does not exist in '%s' or '%s'", src,
            dest);
        } else if (errno == EEXIST) {
            cr_std_logger_outf(
            CR_STD_LOGGER_LOG_TYPE_ERROR,
            "cr_std_filesystem_move_file -> The destination file '%s' already exists", dest);
        } else if (errno == EXDEV) {
            cr_std_logger_out(
            CR_STD_LOGGER_LOG_TYPE_ERROR,
            "cr_std_filesystem_move_file -> Can not move file across different filesystems");
        } else {
            cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                              "cr_std_filesystem_move_file -> Failed to move file");
        }
        return CR_STD_FAIL;
    }
}

b8 cr_std_filesystem_write_file_operations(const char *file_path,
                                           const char *data,
                                           const char *mode) {
    if (!data || !file_path || !mode) {
        return CR_STD_FAIL;
    }

    FILE *file = fopen(file_path, mode);
    if (!file) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR,
                           "cr_std_filesystem_write_file_operations -> file can't be opened -> %s",
                           file_path);
        return CR_STD_FAIL;
    }

    if (fputs(data, file) == EOF) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_filesystem_write_file_operations  -> failed to write data");
        fclose(file);
        return CR_STD_FAIL;
    }

    fclose(file);
    return CR_STD_OK;
}

b8 cr_std_filesystem_write_to_file(const char *file_path, const char *data) {
    return cr_std_filesystem_write_file_operations(file_path, data, "w");
}

b8 cr_std_filesystem_append_to_file(const char *file_path, const char *data) {
    return cr_std_filesystem_write_file_operations(file_path, data, "a");
}

String *cr_std_filesystem_read_file_as_string(Arena *arena, const char *file_path) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_filesystem_read_file_as_string -> arena* was NULL");
        return NULL;
    }

    FILE *file = fopen(file_path, "r");
    if (!file) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR,
                           "cr_std_filesystem_read_file_as_string -> file can't be found -> %s",
                           file_path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    Arena *temp_arena = cr_std_arena_new(CR_STD_FILE_MAX_SIZE);
    char *buffer = cr_std_arena_alloc(temp_arena, file_size + 1);
    if (!buffer) {
        cr_std_logger_out(
        CR_STD_LOGGER_LOG_TYPE_ERROR,
        "cr_std_filesystem_read_file_as_string -> failed to allocate memory for buffer");
        fclose(file);
        cr_std_arena_free(&temp_arena);
        return NULL;
    }

    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';

    fclose(file);
    String *result = cr_std_string_new(arena, buffer);
    cr_std_arena_free(&temp_arena);
    return result;
}

Vector *cr_std_filesystem_read_file_as_vector(Arena *arena, const char *file_path) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_filesystem_read_file_as_vector -> arena* was NULL");
        return NULL;
    }

    if (!arena) {
        CR_LOG_ERROR("cr_std_filesystem_read_file_as_vector -> file_path* was NULL");
        return NULL;
    }

    Arena *temp_arena = cr_std_arena_new(CR_STD_FILE_MAX_SIZE);
    String *file_contents = cr_std_filesystem_read_file_as_string(temp_arena, file_path);
    if (!file_contents) {
        CR_LOG_ERROR_FMT(
        "cr_std_filesystem_read_file_as_vector -> failed to read file contents -> %s", file_path);
        cr_std_arena_free(&temp_arena);
        return NULL;
    }

    Vector *result = cr_std_string_split_hard(arena, file_contents, '\n');
    cr_std_arena_free(&temp_arena);
    return result;
}

Vector *cr_std_filesystem_get_dirs(Arena *arena, const char *file_path) {
    return cr_std_filesystem_get_entries(arena, file_path, false, true, false);
}

Vector *cr_std_filesystem_get_dirs_r(Arena *arena, const char *file_path) {
    return cr_std_filesystem_get_entries(arena, file_path, false, true, true);
}

Vector *cr_std_filesystem_get_dir_files(Arena *arena, const char *file_path) {
    return cr_std_filesystem_get_entries(arena, file_path, true, false, false);
}

Vector *cr_std_filesystem_get_dir_files_r(Arena *arena, const char *file_path) {
    return cr_std_filesystem_get_entries(arena, file_path, true, false, true);
}

Vector *cr_std_filesystem_get_dirs_files_matching(Arena *arena,
                                                  const char *file_path,
                                                  const char *extension) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_filesystem_get_dirs_files_matching -> arena* was NULL");
        return NULL;
    }
    if (!file_path) {
        CR_LOG_ERROR("cr_std_filesystem_get_dirs_files_matching -> file_path* was NULL");
        return NULL;
    }
    if (!extension) {
        CR_LOG_ERROR("cr_std_filesystem_get_dirs_files_matching -> extension* was NULL");
        return NULL;
    }

    Vector *files = cr_std_filesystem_get_dir_files(arena, file_path);
    if (!files) {
        CR_LOG_ERROR_FMT(
        "cr_std_filesystem_get_dirs_files_matching -> failed to fetch files for dir '%s'",
        file_path);
        return NULL;
    }

    for (i32 i = files->size - 1; i >= 0; i--) {
        Dirent *file = cr_std_vector_get_at(files, Dirent, i);
        if (cr_std_string_compare_c_str(file->d_ext, extension) != CR_STD_STRING_EQUAL) {
            if (cr_std_vector_remove_element(files, i) != CR_STD_OK) {
                CR_LOG_WARNING_FMT(
                "cr_std_filesystem_get_dirs_files_matching -> failed to remove element [%zu]", i);
            }
        }
    }
    return files;
}

Vector *cr_std_filesystem_get_dirs_files_matching_r(Arena *arena,
                                                    const char *file_path,
                                                    const char *extension) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_filesystem_get_dirs_files_matching_r -> arena* was NULL");
        return NULL;
    }
    if (!file_path) {
        CR_LOG_ERROR("cr_std_filesystem_get_dirs_files_matching_r -> file_path* was NULL");
        return NULL;
    }
    if (!extension) {
        CR_LOG_ERROR("cr_std_filesystem_get_dirs_files_matching_r -> extension* was NULL");
        return NULL;
    }

    Vector *files = cr_std_filesystem_get_dir_files_r(arena, file_path);
    if (!files) {
        CR_LOG_ERROR_FMT(
        "cr_std_filesystem_get_dirs_files_matching_r -> failed to fetch files for dir '%s'",
        file_path);
        return NULL;
    }

    for (i32 i = files->size - 1; i >= 0; i--) {
        Dirent *file = cr_std_vector_get_at(files, Dirent, i);
        if (cr_std_string_compare_c_str(file->d_ext, extension) != 1) {
            if (cr_std_vector_remove_element(files, i) != CR_STD_OK) {
                CR_LOG_WARNING_FMT(
                "cr_std_filesystem_get_dirs_files_matching_r -> failed to remove element [%zu]", i);
            }
        }
    }
    return files;
}
