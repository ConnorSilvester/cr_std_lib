# cr_std_lib

**cr_std_lib** is a personal C standard library for my own use but feel free to take it and use it in any way you want.

This is not meant as a professional project, but rather just something for me to develop as i learn C.

## Features so far
- **Filesystem Operations**
- **Dynamic Arrays**
- **String Handling**
- **Logging**

## Installation
To build and install the library, follow these steps:

1. **Clone the repository**:
```bash
git clone git@github.com:ConnorSilvester/cr_std_lib.git
cd cr_std_lib
```
2. **Build the Library**:
```bash 
make
```
3. **Install the Library**:
```bash 
sudo make install
```
4. **Clean Up**:
```bash 
make clean
```

## Usage
All functions are documented in the respective header file, for help with a specific function check the header file for usage.

Once installed you can add the following to your program depending what your needs are:

```c
#include <cr_std_filesystem.h>
#include <cr_std_logger.h>
#include <cr_std_string.h>
#include <cr_std_vector.h>
#include <cr_std_testing.h>
#include <cr_std_utils.h>
```

## Code Examples

**Strings** : Prefix is cr_std_string *
```c
string_t *str = cr_std_string_new("Hello, World");
string_t *str = cr_std_string_newf("Hello, %s", "World");
string_t *str = cr_std_string_newf("Hello, %s%d", "World", 4);
```
```c
cr_std_string_concat(str, ", This", " Has", " Been", " Concatenated");
cr_std_string_contains_string(str, "Been");
cr_std_string_split(str, " ");

cr_std_string_free(&str);
```

**Vectors** : Prefix is cr_std_vector *
```c
vector_t *int_vector = cr_std_vector_new(sizeof(int), NULL);
vector_t *string_vector = cr_std_vector_new(sizeof(string_t *), cr_std_string_free_ptr); //string_t custom free function
vector_t *tests_vector = cr_std_vector_new(sizeof(test_case_t *), cr_std_free_ptr); //general use free function
```
```c
int number = 4;
cr_std_vector_push_back(int_vector, &number);

int result = *(int *)cr_std_vector_get_element(int_vector, 0);
cr_std_vector_remove_element(int_vector, 0);

cr_std_vector_free(&int_vector);
```

**Filesystem** : Prefix is cr_std_filesystem *
```c
cr_std_filesystem_write_to_file("test.txt", "This is test data");
cr_std_filesystem_append_to_file("test.txt", "."); //file will contain 'This is test data.'

string_t *string = cr_std_filesystem_read_file_as_string("test.txt");
vector_t *vector = cr_std_filesystem_read_file_as_vector("test.txt");
vector_t *vector = cr_std_filesystem_get_dirs("/home/connor/Downloads");
vector_t *vector = cr_std_filesystem_get_dirs_files("src");
vector_t *vector = cr_std_filesystem_get_dirs_files_r("src"); //recursive search
```

**Logging** : Prefix is cr_std_logger *
```c
// 3 Defined Log Levels
#define CR_STD_LOGGER_LOG_TYPE_INFO 0
#define CR_STD_LOGGER_LOG_TYPE_WARNING 1
#define CR_STD_LOGGER_LOG_TYPE_ERROR 2
```

```c
cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_INFO, "This is a info message");
cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "This is a error message with filename : %s", file_name); //Same formatting as printf
```


**Testing** : Prefix is cr_std_testing *

For best examples see the /tests directory.
```c
// Make vector of test_cases
vector_t *tests = cr_std_vector_new(sizeof(test_case_t *), cr_std_free_ptr);

// cr_std_string_test_new_string_normal is a function pointer to test with. (1 for pass 0 for fail)
cr_std_vector_push_back(tests, cr_std_testing_new_test("Make String -> Normal", cr_std_string_test_new_string_normal));
cr_std_vector_push_back(tests, cr_std_testing_new_test("Make String -> Empty", cr_std_string_test_new_string_empty));
cr_std_vector_push_back(tests, cr_std_testing_new_test("Make String -> Formatted", cr_std_string_test_new_string_formatted));

cr_std_testing_run_tests(tests);
cr_std_vector_free(&tests);

```
## Try Before Install
If you would like to test the library before install you can use the main.c file included.

Use the command:
```bash 
make run
```

This will compile all the code and run the main function for you. 

Feel free to run the already written code demonstrating the string concatenation.
