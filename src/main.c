#include "cr_std_csv.h"
#include "cr_std_filesystem.h"
#include "cr_std_logger.h"
#include "cr_std_string.h"
#include "cr_std_testing.h"
#include "cr_std_utils.h"
#include "cr_std_vector.h"
#include <stdio.h>

// If you want to test the library see the end of the README.md file
int main(int argc, char **argv) {

    String *string = cr_std_string_newf("Hello, %s", "World");
    cr_std_string_concat(string, ", This", " Has", " Been", " Concatenated");
    printf("%s\n", string->c_str);

    cr_std_string_free(&string);
    return 0;
}
