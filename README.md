# cr_std_lib

**cr_std_lib** is a personal C standard library for my own use but feel free to take it and use it in any way you want.

This is not meant as a professional project, but rather just something for me to develop as i learn C.

## Features so far
- **Filesystem Operations**
- **Dynamic Arrays**
- **String Handling**
- **Logging**
- **File Parsing**
- **CSV Parsing**
- **CLI Parsing**
- **Arena Memory Management**

## Pre-Installation
To build and install the library you must have `cmake` installed.

**Install CMake and a C compiler**
**Windows**:
```txt
# CMake
https://cmake.org/download/

# C Compiler, included with the C++ desktop development module
https://visualstudio.microsoft.com/

# GCC for Windows, if you don't want to use VS
https://www.mingw-w64.org/
```
**Linux**:
```bash
sudo apt update
sudo apt install cmake gcc
```

## Installation Directories
These can be updated in the `CMakeLists.txt` file or with the flag `-DCMAKE_INSTALL_PREFIX=""`

**Linux**:
```
/usr/local/lib
/usr/local/include
```
**Windows**:
```
C:\Program Files\CR_STD_LIB\lib
C:\Program Files\CR_STD_LIB\include
```
## Build Script
Run the build script located in the `scripts` folder.

Make sure to run the one for your platform.

## Manual Build
1. **Clone the repository**:
```bash
git clone git@github.com:ConnorSilvester/cr_std_lib.git
cd cr_std_lib
```
2. **Create the Build Dir**:
```bash
mkdir build
cd build
```
3. **Build The Library**:
```bash
cmake ..
cmake --build . --config Release
```
## Try Before Install
You can test the library before installing it by running the included `main.c` file.
Use the `run` script (found in the `scripts/` folder) for your platform or manually execute:
```bash
./bin/main
```

## System Wide Install Script
Run the install script for your platform scripts are found in the `scripts` directory
Make sure to run as administrator on Windows if needed

## System Wide Manual Install
```bash
cd build
cmake ..
cmake --build . --config Release
sudo cmake --install .
```

## Usage
All functions are documented in the respective header file, for help with a specific function check the header file for usage.

Once installed you can add the following to your program depending what your needs are:

```c
#include <cr_std_arena.h>
#include <cr_std_filesystem.h>
#include <cr_std_logger.h>
#include <cr_std_string.h>
#include <cr_std_vector.h>
#include <cr_std_testing.h>
#include <cr_std_utils.h>
#include <cr_std_csv.h>
#include <cr_std_cli.h>
```

## Code Examples

**Strings** : Prefix is cr_std_string *
```c
Arena *arena = cr_std_arena_new(CR_STD_KB);

// Create a new dynamically allocated string
// Memory is stored in the arena
String *str = cr_std_string_new(arena, "Hello, World");
String *str1 = cr_std_string_newf(arena, "Hello, %s", "World");
String *str2 = cr_std_string_newf(arena, "Hello, %s%d", "World", 4);
```
```c
// Concat strings
cr_std_string_concat(arena, str, ", This", " Has", " Been", " Concatenated");
cr_std_string_contains_string(str, "Been");
cr_std_string_split(arena, str, " ");

cr_std_arena_free(&arena);
```

**Vectors** : Prefix is cr_std_vector *
```c
Arena *arena = cr_std_arena_new(CR_STD_KB);

// New vector containing pointers, (only stores pointers)
// Memory is stored in the arena
Vector *string_vector = cr_std_vector_new(arena);
String *string = cr_std_string_new(arena, "String");
cr_std_vector_push_back(arena, string_vector, string);

// String example
Vector *dest = cr_std_vector_new(arena);
Vector *src = cr_std_vector_new(arena);

String *string_1 = cr_std_string_new(arena, "String 1");
String *string_2 = cr_std_string_new(arena, "String 2");
String *string_3 = cr_std_string_new(arena, "String 3");
String *string_4 = cr_std_string_new(arena, "String 4");

cr_std_vector_push_back(arena, dest, string_1);
cr_std_vector_push_back(arena, dest, string_2);
cr_std_vector_push_back(arena, src, string_3);
cr_std_vector_push_back(arena, src, string_4);

// Copies all data from src to dest vectors
cr_std_vector_extend(arena, dest, src);

// Get all elements and print them out
String **strings = cr_std_vector_get_all(dest, String); // Vector and the type its storing
for (int i = 0; i < dest->size; i++) {
    printf("%s\n", strings[i]->c_str);
}

// Get element 0 of the vector
String *string = cr_std_vector_get_at(dest, String, 0);

// Frees all data inside the arena
cr_std_arena_free(&arena);
```

**Filesystem** : Prefix is cr_std_filesystem *
```c
Arena *arena = cr_std_arena_new(CR_STD_KB);

cr_std_filesystem_write_to_file("test.txt", "This is test data");
cr_std_filesystem_append_to_file("test.txt", "."); // File will contain 'This is test data.'

String *string = cr_std_filesystem_read_file_as_string(arena, "test.txt");
Vector *vector = cr_std_filesystem_read_file_as_vector(arena, "test.txt");

// Returns a vector of custom Dirent structs.
Vector *vector = cr_std_filesystem_get_dirs(arena, "/home/connor/Downloads");
Vector *vector = cr_std_filesystem_get_dirs_r(arena, "/home/connor/Downloads"); // Recursive search
Vector *vector = cr_std_filesystem_get_dirs_files(arena, "src");
Vector *vector = cr_std_filesystem_get_dirs_files_r(arena, "src"); // Recursive search

// Filter out file types
Vector *vector = cr_std_filesystem_get_dirs_files_matching(arena, "src", ".txt");
Vector *vector = cr_std_filesystem_get_dirs_files_matching_r(arena, "src", ".csv"); // Recursive search

cr_std_arena_free(&arena);
```

**Logging** : Prefix is cr_std_logger *
```c
// 3 Defined Log Types
#define CR_STD_LOGGER_LOG_TYPE_INFO 0
#define CR_STD_LOGGER_LOG_TYPE_WARNING 1
#define CR_STD_LOGGER_LOG_TYPE_ERROR 2

// 4 Defined Log Levels
// Set to ALL by default
#define CR_STD_LOGGER_LOG_LEVEL_ALL 0
#define CR_STD_LOGGER_LOG_LEVEL_WARNING 1
#define CR_STD_LOGGER_LOG_LEVEL_ERROR 2
#define CR_STD_LOGGER_LOG_LEVEL_NONE 3
```

```c
// Disable logger output
cr_std_logger_set_log_level(CR_STD_LOGGER_LOG_LEVEL_NONE);

// Set to show all logger outputs
cr_std_logger_set_log_level(CR_STD_LOGGER_LOG_LEVEL_ALL);

cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_INFO, "This is a info message");
cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "This is a error message with filename : %s", file_name); // Same formatting as printf

// Macros for faster logging
CR_LOG_INFO("Read File");
CR_LOG_INFO_FMT("Read Line %d", 3);
```


**CSV** : Prefix is cr_std_csv *
```c
typedef struct CSVFile {
    Vector *titles; // Vector <String>
    Vector *rows;   // Vector <CSVRow>
} CSVFile;

typedef struct CSVRow {
    Vector *fields; // Vector <String>
} CSVRow;


Arena *arena = cr_std_arena_new(CR_STD_KB);
CSVFile *csv = cr_std_csv_parse_file(arena, "test_file.csv");
cr_std_csv_print_contents(csv);


```
```c

// Example of how to access all fields

CSVFile *csv = cr_std_csv_parse_file(arena, "test_file.csv");
for (int row_index = 0; row_index < csv->rows->size; row_index++) {
    CSVRow *row = cr_std_vector_get_at(csv->rows, CSVRow, row_index);
    printf("Row %d:\n", row_index + 1);
    for (int field_index = 0; field_index < row->fields->size; field_index++) {
        String *field = cr_std_vector_get_at(row->fields, String, field_index);
        printf("Field : %s\n", field->c_str);
    }
}

cr_std_arena_free(&arena);
```

**Testing** : Prefix is cr_std_testing *

For best examples see the /tests directory in the source code.
```c
Arena *arena = cr_std_arena_new(CR_STD_KB);

// Make vector of test_cases
Vector *tests = cr_std_vector_new(arena);

// cr_std_string_test_new_string_normal is a function pointer to test with. (1 for pass 0 for fail)
cr_std_vector_push_back(arena, tests, cr_std_testing_new_test(arena, "Make String -> Normal", cr_std_string_test_new_string_normal));
cr_std_vector_push_back(arena, tests, cr_std_testing_new_test(arena, "Make String -> Empty", cr_std_string_test_new_string_empty));
cr_std_vector_push_back(arena, tests, cr_std_testing_new_test(arena, "Make String -> Formatted", cr_std_string_test_new_string_formatted));

cr_std_testing_run_tests(arena, tests);
cr_std_arena_free(&arena);

```

**Arena** : Prefix is cr_std_arena *
```c
// Heap
Arena *arena = cr_std_arena_new(1 * CR_STD_KB);

// Stack
Arena stack_arena;
unsigned char stack_arena_memory[1 * CR_STD_KB];
cr_std_arena_init(&stack_arena, &stack_arena_memory, sizeof(stack_arena_memory));

// Heap
String *string = cr_std_string_newf(arena, "Hello, %s", "World");
cr_std_string_concat(arena, string, ", This", " Has", " Been", " Concatenated");

// Stack
String *stack_string = cr_std_string_new(&stack_arena, "Hello, World");
printf("%s\n", string->c_str);
printf("%s\n", stack_string->c_str);

// Do not call free for the stack_arena
cr_std_arena_free(&arena);
```
