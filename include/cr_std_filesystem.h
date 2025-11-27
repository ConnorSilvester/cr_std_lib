#ifndef CR_STD_FILESYSTEM
#define CR_STD_FILESYSTEM

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

#ifdef _WIN32
#define DT_UNKNOWN 0
#define DT_FIFO    1
#define DT_CHR     2
#define DT_DIR     4
#define DT_BLK     6
#define DT_REG     8
#define DT_LNK     10
#define DT_SOCK    12
#define DT_WHT     14
#else
#include <dirent.h>
#include <unistd.h>
#endif

typedef struct String String;
typedef struct Vector Vector;

/**
 * @brief Represents a single directory entry.
 *
 * Stores relevant information on the entry.
 *
 * Memory ownership:
 * - This struct owns all the String objects.
 * - Freeing a Dirent struct will also free all String objects.
 */
typedef struct Dirent {
    String *d_name;         // Entry name
    String *d_path;         // Entry full path
    String *d_ext;          // Entry extension
    int64_t d_size;         // Entry size in bytes
    uint64_t d_inode;       // Entry inode number
    uint64_t d_permissions; // Entry permissions
    unsigned char d_type;   // Entry type
    bool d_hidden;          // 1 = hidden, 0 = not hidden
} Dirent;

/**
 * @brief Get local time or date information
 *
 * @param `time_date_format` The format for the string, example ("%d-%m-%Y %H:%M:%S")
 *
 * @return `String` containing the information on success.
 * @return `NULL` on failure.
 */
String *cr_std_filesystem_get_current_time_date(const char *time_date_format);

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

#ifdef _WIN32
/**
 * @brief Gets the current working directory
 *
 * @return `String *` on success, containing the path.
 * @return `NULL` on failure.
 */
String *cr_std_filesystem_get_cwd();

/**
 * @brief Makes a directory
 *
 * @param `dir_path` The directory path to create
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_filesystem_make_dir(const char* dir_path);
#else
/**
 * @brief Gets the current working directory
 *
 * @return `String *` on success, containing the path.
 * @return `NULL` on failure.
 */
String *cr_std_filesystem_get_cwd();

/**
 * @brief Makes a directory
 *
 * @param `dir_path` The directory path to create
 * @param `permissions` The permissions of the dir
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_filesystem_make_dir(const char *dir_path, mode_t permissions);
#endif // _WIN32


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
 * @brief Reads file contents into a `String` struct.
 *
 * @param `file_path` A raw string of the file path.
 *
 * @return A pointer to the new `String` struct containing the file contents.
 * @return `NULL` on failure.
 */
String *cr_std_filesystem_read_file_as_string(const char *file_path);

/**
 * @brief Reads file contents into a `Vector` struct, splitting by newline.
 *
 * @param `file_path` A raw string of the file path.
 *
 * @return A pointer to a new `Vector` struct containing the file contents.
 * @return `NULL` on failure.
 */
Vector *cr_std_filesystem_read_file_as_vector(const char *file_path);

/**
 * @brief Gets entries given a path and settings.
 *
 * @param `file_path` A raw string of the file path.
 * @param `include_files` A flag to include files or not.
 * @param `include_dirs` A flag to include dirs or not.
 * @param `recursive` A flag to recursively search or not.
 *
 * @return A pointer to a new `Vector` struct containing the entries contents.
 * @return `NULL` on failure.
 */
Vector *cr_std_filesystem_get_entries(const char *file_path, bool include_files, bool include_dirs, bool recursive);

/**
 * @brief Reads the contents of a file dir
 *
 * @param `file_path` A raw string of the file path.
 *
 * @return A pointer to a new `Vector` struct containing `Dirent` structs of the directories in the dir.
 * @return `NULL` on failure.
 */
Vector *cr_std_filesystem_get_dirs(const char *file_path);

/**
 * @brief Reads the contents of a file dir recursively.
 *
 * @param `file_path` A raw string of the file path.
 *
 * @return A pointer to a new `Vector` struct containing `Dirent` structs of the directories in the dir and all sub dirs.
 * @return `NULL` on failure.
 */
Vector *cr_std_filesystem_get_dirs_r(const char *file_path);

/**
 * @brief Reads the contents of a file dir
 *
 * @param `file_path` A raw string of the file path.
 *
 * @return A pointer to a new `Vector` struct containing `Dirent` structs of the files in the dir.
 * @return `NULL` on failure.
 */
Vector *cr_std_filesystem_get_dir_files(const char *file_path);

/**
 * @brief Reads the contents of a file dir recursively
 *
 * @param `file_path` A raw string of the file path.
 *
 * @return A pointer to a new `Vector` struct containing `Dirent` structs of the files in the dir and all sub dirs.
 * @return `NULL` on failure.
 */
Vector *cr_std_filesystem_get_dir_files_r(const char *file_path);

/**
 * @brief Reads the contents of a file dir that match the extension
 *
 * @param `file_path` A raw string of the file path.
 * @param `extension` The extension to filter by.
 *
 * @return `Vector`struct containing `Dirent` structs of the files in the dir that match the extension.
 * @return `NULL` on failure.
 */
Vector *cr_std_filesystem_get_dirs_files_matching(const char *file_path, const char *extension);

/**
 * @brief Reads the contents of a file dir recursively and returns the ones that match the extension.
 *
 * @param `file_path` A raw string of the file path.
 * @param `extension` The extension to filter by.
 *
 * @return `Vector`struct containing `Dirent` structs of the files that match the extension.
 * @return `NULL` on failure.
 */
Vector *cr_std_filesystem_get_dirs_files_matching_r(const char *file_path, const char *extension);

/**
 * @brief Free a `Dirent` struct
 *
 * @param `dirent_ptr` A pointer to a pointer that stores the `Dirent`
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_filesystem_dirent_free(Dirent **dirent_ptr);
#define cr_std_filesystem_dirent_free_ptr ((int (*)(void **))cr_std_filesystem_dirent_free)

#ifdef __cplusplus
}
#endif
#endif // CR_STD_FILESYSTEM
