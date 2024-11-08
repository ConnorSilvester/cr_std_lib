#ifndef CR_STD_FILESYSTEM
#define CR_STD_FILESYSTEM
#include <stdbool.h>

typedef struct string_t string_t;
typedef struct vector_t vector_t;

/**
 * @brief Writes the contents of `data` into the file at `file_path`
 *
 * @param `file_path` A raw string of the file path.
 * @param `data` A raw string containing the data to write.
 *
 * @return `1` on success.
 * @return `0` on failure.
 */
int cr_std_filesystem_write_to_file(const char *file_path, const char *data);

/**
 * @brief Appends the contents of `data` into the file at `file_path`
 *
 * @param `file_path` A raw string of the file path.
 * @param `data` A raw string containing the data to append to the file.
 *
 * @return `1` on success.
 * @return `0` on failure.
 */
int cr_std_filesystem_append_to_file(const char *file_path, const char *data);

/**
 * @brief Reads file contents into a `string_t` struct.
 *
 * @param `file_path` A raw string of the file path.
 *
 * @return A pointer to the new `string_t` struct containing the file contents.
 * @return `NULL` on failure.
 */
string_t *cr_std_filesystem_read_file_as_string(const char *file_path);

/**
 * @brief Reads file contents into a `vector_t` struct, splitting by newline.
 *
 * @param `file_path` A raw string of the file path.
 *
 * @return A pointer to a new `vector_t` struct containing the file contents.
 * @return `NULL` on failure.
 */
vector_t *cr_std_filesystem_read_file_as_vector(const char *file_path);

/**
 * @brief Gets entries given a path and settings.
 *
 * @param `file_path` A raw string of the file path.
 * @param `include_files` A flag to include files or not.
 * @param `include_dirs` A flag to include dirs or not.
 * @param `recursive` A flag to recursively search or not.
 *
 * @return A pointer to a new `vector_t` struct containing the entries contents.
 * @return `NULL` on failure.
 */
vector_t *cr_std_filesystem_get_entries(const char *file_path, bool include_files, bool include_dirs, bool recursive);

/**
 * @brief Reads the contents of a file dir
 *
 * @param `file_path` A raw string of the file path.
 *
 * @return A pointer to a new `vector_t` struct containing `dirent` structs of the directories in the dir.
 * @return `NULL` on failure.
 */
vector_t *cr_std_filesystem_get_dirs(const char *file_path);

/**
 * @brief Reads the contents of a file dir recursively.
 *
 * @param `file_path` A raw string of the file path.
 *
 * @return A pointer to a new `vector_t` struct containing `dirent` structs of the directories in the dir and all sub dirs.
 * @return `NULL` on failure.
 */
vector_t *cr_std_filesystem_get_dirs_r(const char *file_path);

/**
 * @brief Reads the contents of a file dir
 *
 * @param `file_path` A raw string of the file path.
 *
 * @return A pointer to a new `vector_t` struct containing `dirent` structs of the files in the dir.
 * @return `NULL` on failure.
 */
vector_t *cr_std_filesystem_get_dir_files(const char *file_path);

/**
 * @brief Reads the contents of a file dir recursively
 *
 * @param `file_path` A raw string of the file path.
 *
 * @return A pointer to a new `vector_t` struct containing `dirent` structs of the files in the dir and all sub dirs.
 * @return `NULL` on failure.
 */
vector_t *cr_std_filesystem_get_dir_files_r(const char *file_path);
#endif // CR_STD_FILESYSTEM
