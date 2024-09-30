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

```
#include <cr_std_filesystem.h>
#include <cr_std_logger.h>
#include <cr_std_string.h>
#include <cr_std_vector.h>
```

## Code Examples

**Strings** : Prefix is cr_std_string *
```bash
string_t *str = cr_std_string_new("Hello, World");
string_t *str = cr_std_string_new("Hello, %s", "World"); //Same formatting as printf
string_t *str = cr_std_string_new("Hello, %s%d", "World", 4);
```
```bash
//Works with any amount of arguments.
cr_std_string_concat(str, ", This", " Has", " Been", " Concatenated");
cr_std_string_contains_string(str, "Been");
cr_std_string_split(str, " ");

cr_std_string_free(str);
```

**Vectors** : Prefix is cr_std_vector *
```bash
vector_t *int_vector = cr_std_vector_new(sizeof(int));
vector_t *string_vector = cr_std_vector_new(sizeof(string_t *));
```
```bash
int number = 4;
cr_std_vector_push_back(int_vector, &number);

int result = *(int *)cr_std_vector_get_element(int_vector, 0);
cr_std_vector_remove_element(int_vector, 0);

cr_std_vector_free(int_vector);
```

**Filesystem** : Prefix is cr_std_filesystem *
```bash
string_t *string = cr_std_filesystem_read_file_as_string("test.txt");
vector_t *vector = cr_std_filesystem_read_file_as_vector("test.txt");
vector_t *vector = cr_std_filesystem_get_dir_files("/home/connor/Downloads");
vector_t *vector = cr_std_filesystem_get_dir_files("src");
```

**Logging** : Prefix is cr_std_logger *
```bash
// 3 Log Levels
CR_STD_LOGGER_LOG_TYPE_INFO 
CR_STD_LOGGER_LOG_TYPE_WARNING
CR_STD_LOGGER_LOG_TYPE_ERROR
```

```bash
cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_INFO, "This is a info message");
cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "This is a error message with filename : %s", file_name); //Same formatting as printf
```

## Try Before Install
If you would like to test the library before install you can use the main.c file included.

Use the command:
```bash 
make run
```

This will compile all the code and run the main function for you. 

Feel free to run the already written code demonstrating the string concatenation.
