#include "cr_std_filesystem.h"
#include "cr_std_logger.h"
#include "cr_std_string.h"
#include "cr_std_testing.h"
#include "cr_std_vector.h"
#include <stdio.h>

// Use if you want to test the library
// Use `make run` to run this.
int main(int argc, char **argv) {

    string_t *string = cr_std_string_new("Hello, %s", "World");
    cr_std_string_concat(string, ", This", " Has", " Been", " Concatenated");
    printf("%s\n", string->c_str);

    cr_std_string_free(&string);
    return 0;
}
