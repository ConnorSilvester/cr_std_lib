#ifndef CR_STD_FILESYSTEM
#define CR_STD_FILESYSTEM

typedef struct string_t string_t;
typedef struct vector_t vector_t;

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
 * @brief Reads the contents of a file dir
 *
 * @param `file_path` A raw string of the file path.
 *
 * @return A pointer to a new `vector_t`struct containing `string_t` struct of the file names in the dir.
 * @return `NULL` on failure.
 */
vector_t *cr_std_filesystem_get_dir_files(const char *file_path);
#endif // CR_STD_FILESYSTEM
