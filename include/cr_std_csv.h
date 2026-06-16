
#ifndef CR_STD_CSV
#define CR_STD_CSV

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Vector Vector;
typedef struct Arena Arena;

/**
 * @brief Represents an entire CSV file in memory.
 *
 * The CSVFile struct stores the contents of a CSV file, including the column headers
 * (titles) and all rows of data. Each row contains a vector of fields.
 *
 * Memory ownership:
 * - The CSVFile owns both the titles vector and the rows vector.
 * - Each CSVRow in the rows vector owns its fields.
 * - Freeing a CSVFile will free all nested CSVRow and String objects.
 */
typedef struct CSVFile {
    Vector *titles; // Vector <String>
    Vector *rows;   // Vector <CSVRow>
} CSVFile;

/**
 * @brief Represents a single row in a CSV file.
 *
 * Each CSVRow contains a vector of fields corresponding to the columns in the CSV file.
 *
 * Memory ownership:
 * - The CSVRow owns the fields vector.
 * - Freeing a CSVRow will free all nested String objects.
 */
typedef struct CSVRow {
    Vector *fields; // Vector <String>
} CSVRow;

/**
 * @brief Creates a new `CSVFile` struct
 *
 * @param `arena` The arena to store the memory in
 *
 * @return A pointer to the new `CSVFile` struct.
 * @return `NULL` if allocation fails.
 */
CSVFile *cr_std_csv_new(Arena *arena);

/**
 * @brief Creates a new `CSVRow` struct
 *
 * @param `arena` The arena to store the memory in
 *
 * @return A pointer to the new `CSVRow` struct.
 * @return `NULL` if allocation fails.
 */
CSVRow *cr_std_csv_row_new(Arena *arena);

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
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_csv_print_contents(CSVFile *csv);

#ifdef __cplusplus
}
#endif
#endif // CR_STD_CSV
