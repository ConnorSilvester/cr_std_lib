
#ifndef CR_STD_CSV
#define CR_STD_CSV

#include "cr_std_utils.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Vector Vector;
typedef struct Arena Arena;

/**
 * @brief Represents an entire CSV file in memory.
 * @note `cr_std_arena` is used for memory management
 *
 * The CSVFile struct stores the contents of a CSV file, including the column headers
 * (titles) and all rows of data. Each row contains a vector of fields.
 *
 */
typedef struct CSVFile {
    Vector *titles; // Vector <String>
    Vector *rows;   // Vector <CSVRow>
} CSVFile;

/**
 * @brief Represents a single row in a CSV file.
 * @note `cr_std_arena` is used for memory management
 *
 * Each CSVRow contains a vector of fields corresponding to the columns in the CSV file.
 *
 */
typedef struct CSVRow {
    Vector *fields; // Vector <String>
} CSVRow;

/**
 * @brief Parse a csv file into a CSVFile struct.
 *
 * @param `arena` The arena to store the memory in
 * @param `file_path` The path to the csv file.
 *
 * @return `CSVFile *` on success, which contains the csv file contents.
 * @return `NULL` on failure.
 */
CSVFile *cr_std_csv_parse_file(Arena *arena, const char *file_path);

/**
 * @brief Prints the contents of a CSVFile struct.
 *
 * @param `csv` The CSVFile struct to print.
 *
 * @return `CR_STD_OK` on success.
 * @return `CR_STD_FAIL` on failure.
 */
b8 cr_std_csv_print_contents(CSVFile *csv);

/**
 * @brief Finds the first index of a given title
 *
 * @param `csv` The CSVFile struct to print.
 * @param `title` The title to search for
 * @param `out` The index of the title if found, -1 if not found
 *
 * @return `CR_STD_OK` on success.
 * @return `CR_STD_FAIL` on failure.
 */
b8 cr_std_csv_find_title_index(CSVFile *csv, const char *title, i32 *out);

/**
 * @brief Finds the first index of a given title
 *
 * @param `csv` The CSVFile struct to print.
 * @param `title` The title to search for
 * @param `arena` Arena to allocate memory in the event the vector expands
 * @param `out` The vector to add indexs to, they are added as *i32
 *
 * @return `CR_STD_OK` on success.
 * @return `CR_STD_FAIL` on failure.
 */
b8 cr_std_csv_find_title_indexs(CSVFile *csv, const char *title, Arena *arena, Vector *out);

#ifdef __cplusplus
}
#endif
#endif // CR_STD_CSV
