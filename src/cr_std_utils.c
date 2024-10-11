#include "cr_std_utils.h"
#include <stdlib.h>

int cr_std_free(void **ptr) {
    if (ptr && *ptr) {
        free(*ptr);
        *ptr = NULL;
        return 1;
    }
    return 0;
}
