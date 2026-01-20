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
        cr_std_logger_out(
        CR_STD_LOGGER_LOG_TYPE_ERROR,
        "cr_std_filesystem_get_current_time -> strftime failed to format the format string");
        return NULL;
    }

    return cr_std_string_new(time_str);
}

int cr_std_filesystem_copy_file(const char *src, const char *dest) {

    FILE *src_file = fopen(src, "rb");
    if (!src_file) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR,
                           "cr_std_filesystem_copy_file -> file can't be opened -> %s", src);
        return 1;
    }

    FILE *dest_file = fopen(dest, "wb");
    if (!dest_file) {
        fclose(src_file);
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR,
                           "cr_std_filesystem_copy_file -> file can't be opened -> %s", dest);
        return 1;
    }

    int ch;
    while ((ch = fgetc(src_file)) != EOF) {
        if (fputc(ch, dest_file) == EOF) {
            cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR,
                               "cr_std_filesystem_copy_file -> error writing to file -> %s", dest);
            fclose(src_file);
            fclose(dest_file);
            return 1;
        }
    }

    if (ferror(src_file)) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR,
                           "cr_std_filesystem_copy_file -> error reading from file -> %s", src);
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
        cr_std_logger_outf(
        CR_STD_LOGGER_LOG_TYPE_INFO,
        "cr_std_filesystem_move_file -> File moved from '%s' to '%s' successfully", src, dest);
        return 0;
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
        return 1;
    }
}

int cr_std_filesystem_write_file_operations(const char *file_path,
                                            const char *data,
                                            const char *mode) {
    if (!data || !file_path || !mode) {
        return 1;
    }

    FILE *file = fopen(file_path, mode);
    if (!file) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR,
                           "cr_std_filesystem_write_file_operations -> file can't be opened -> %s",
                           file_path);
        return 1;
    }

    if (fputs(data, file) == EOF) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_filesystem_write_file_operations  -> failed to write data");
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
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR,
                           "cr_std_filesystem_read_file_as_string -> file can't be found -> %s",
                           file_path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(file_size + 1);
    if (!buffer) {
        cr_std_logger_out(
        CR_STD_LOGGER_LOG_TYPE_ERROR,
        "cr_std_filesystem_read_file_as_string -> failed to allocate memory for buffer");
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
        cr_std_logger_outf(
        CR_STD_LOGGER_LOG_TYPE_ERROR,
        "cr_std_filesystem_read_file_as_vector -> failed to read file contents -> %s", file_path);
        return NULL;
    }

    Vector *file_lines = cr_std_string_split_hard(file_contents, '\n');
    if (!file_lines) {
        cr_std_logger_outf(
        CR_STD_LOGGER_LOG_TYPE_ERROR,
        "cr_std_filesystem_read_file_as_vector -> failed to split file contents -> %s", file_path);
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

Vector *cr_std_filesystem_get_dirs_files_matching(const char *file_path, const char *extension) {
    Vector *files = cr_std_filesystem_get_dir_files(file_path);
    for (int i = files->size - 1; i >= 0; i--) {
        Dirent *file = cr_std_vector_get_at(files, Dirent, i);
        if (cr_std_string_compare_c_str(file->d_ext, extension) != 1) {
            cr_std_vector_remove_element(files, i);
        }
    }
    return files;
}

Vector *cr_std_filesystem_get_dirs_files_matching_r(const char *file_path, const char *extension) {
    Vector *files = cr_std_filesystem_get_dir_files_r(file_path);
    for (int i = files->size - 1; i >= 0; i--) {
        Dirent *file = cr_std_vector_get_at(files, Dirent, i);
        if (cr_std_string_compare_c_str(file->d_ext, extension) != 1) {
            cr_std_vector_remove_element(files, i);
        }
    }
    return files;
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
    cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_WARNING,
                      "cr_std_filesystem_dirent_free -> tried to free a NULL dirent_ptr*");
    return 1;
}
