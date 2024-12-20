#include "cr_std_logger.h"
#include "cr_std_string_tests.h"
#include "cr_std_vector_tests.h"

// Use `make run_tests` to run this entry and run all tests.
int main() {
    // Disable logger output for running the tests
    cr_std_logger_set_log_level(CR_STD_LOGGER_LOG_LEVEL_NONE);
    cr_std_string_test_all();
    cr_std_vector_test_all();
    return 0;
}
