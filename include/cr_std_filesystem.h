#ifndef CR_STD_FILESYSTEM
#define CR_STD_FILESYSTEM
#include <stdbool.h>
#include <stdint.h>

typedef struct string_t string_t;
typedef struct vector_t vector_t;

typedef struct dirent_t {
    string_t *d_name;       // Entry name
    string_t *d_path;       // Entry full path
    string_t *d_ext;        // Entry extension
    int64_t d_size;         // Entry size in bytes
    uint64_t d_inode;       // Entry inode number
    uint64_t d_permissions; // Entry permissions
    unsigned char d_type;   // Entry type (from struct dirent)
    unsigned char d_hidden; // 1 = hidden, 0 = not hidden
} dirent_t;

/**
 * @brief Copy a file from src to dest
 *
 * @param `src` The source file to copy from
 * @param `dest` The destination file to copy to
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_filesystem_copy_file(const char *src, const char *dest);

/**
 * @brief Move a file from src to dest
 *
 * @param `src` The source file to move
 * @param `dest` The destination file path
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_filesystem_move_file(const char *src, const char *dest);

/**
 * @brief Writes the contents of `data` into the file at `file_path`
 *
 * @param `file_path` A raw string of the file path.
 * @param `data` A raw string containing the data to write.
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_filesystem_write_to_file(const char *file_path, const char *data);

/**
 * @brief Appends the contents of `data` into the file at `file_path`
 *
 * @param `file_path` A raw string of the file path.
 * @param `data` A raw string containing the data to append to the file.
 *
 * @return `0` on success.
 * @return `1` on failure.
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
 * @return A pointer to a new `vector_t` struct containing `dirent_t` structs of the directories in the dir.
 * @return `NULL` on failure.
 */
vector_t *cr_std_filesystem_get_dirs(const char *file_path);

/**
 * @brief Reads the contents of a file dir recursively.
 *
 * @param `file_path` A raw string of the file path.
 *
 * @return A pointer to a new `vector_t` struct containing `dirent_t` structs of the directories in the dir and all sub dirs.
 * @return `NULL` on failure.
 */
vector_t *cr_std_filesystem_get_dirs_r(const char *file_path);

/**
 * @brief Reads the contents of a file dir
 *
 * @param `file_path` A raw string of the file path.
 *
 * @return A pointer to a new `vector_t` struct containing `dirent_t` structs of the files in the dir.
 * @return `NULL` on failure.
 */
vector_t *cr_std_filesystem_get_dir_files(const char *file_path);

/**
 * @brief Reads the contents of a file dir recursively
 *
 * @param `file_path` A raw string of the file path.
 *
 * @return A pointer to a new `vector_t` struct containing `dirent_t` structs of the files in the dir and all sub dirs.
 * @return `NULL` on failure.
 */
vector_t *cr_std_filesystem_get_dir_files_r(const char *file_path);

/**
 * @brief Free a `dirent_t` struct
 *
 * @param `dirent_ptr` A pointer to a pointer that stores the `dirent_t`
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_filesystem_dirent_free(dirent_t **dirent_ptr);
#define cr_std_filesystem_dirent_free_ptr ((int (*)(void **))cr_std_filesystem_dirent_free)

#endif // CR_STD_FILESYSTEM
