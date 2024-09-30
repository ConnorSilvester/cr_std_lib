#include "cr_std_vector.h"
#include "cr_std_logger.h"
#include <stdlib.h>
#include <string.h>

vector_t *cr_std_vector_new(size_t type_size) {
    vector_t *vector = malloc(sizeof(vector_t));

    if (!vector) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_vector_new -> failed to allocate memory for new vector_t struct");
        return NULL;
    }

    vector->size = 0;
    vector->capacity = 4;
    vector->type_size = type_size;
    vector->is_pointer = vector->type_size == sizeof(void *);
    vector->elements = malloc(vector->capacity * sizeof(void *));

    return vector;
}

int cr_std_vector_free(vector_t *vector) {
    if (!vector) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_vector_free -> tried to free a NULL vector");
        return 0;
    }

    // If the vector holds pointers, free each element
    if (vector->is_pointer) {
        for (size_t i = 0; i < vector->size; i++) {
            free(((void **)vector->elements)[i]);
        }
    }

    free(vector->elements);
    free(vector);

    return 1;
}

int cr_std_vector_push_back(vector_t *vector, void *element) {
    if (!vector) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_vector_push_back -> given vector is NULL");
        return 0;
    }

    if (vector->size >= vector->capacity) {
        vector->capacity *= 2;
        void *temp = realloc(vector->elements, vector->capacity * vector->type_size);
        if (!temp) {
            cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_vector_push_back -> failed to realloc memory for vector_t");
            return 0;
        }
        vector->elements = temp;
    }

    if (vector->is_pointer) {
        // If storing pointers, directly assign the pointer
        ((void **)vector->elements)[vector->size] = element;
    } else {
        // If storing raw data, copy the data into the vector
        int offset_from_start_of_vector = vector->size * vector->type_size;
        char *dest = (char *)vector->elements + offset_from_start_of_vector;
        memcpy(dest, element, vector->type_size);
    }

    vector->size++;
    return 1;
}

int cr_std_vector_remove_element(vector_t *vector, size_t index) {
    if (!vector) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_vector_remove_element -> given vector is NULL");
        return 0;
    }

    if (index >= vector->size || index < 0) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_WARNING, "cr_std_vector_remove_element -> tried to access out of bounds index -> index : %ld max_index : %ld", index, vector->size - 1);
        return 0;
    }

    if (vector->is_pointer) {
        // Free current element, then move shift each element down.
        free(((void **)vector->elements)[index]);
        for (size_t i = index; i < vector->size - 1; i++) {
            ((void **)vector->elements)[i] = ((void **)vector->elements)[i + 1];
        }
    } else {
        size_t bytes_to_move = (vector->size - index - 1) * vector->type_size;
        if (bytes_to_move > 0) {
            char *dest = (char *)vector->elements + (index * vector->type_size);
            char *src = (char *)vector->elements + ((index + 1) * vector->type_size);
            memcpy(dest, src, bytes_to_move);
        }
    }

    vector->size--;
    return 1;
}

void *cr_std_vector_get_element(vector_t *vector, size_t index) {
    if (!vector) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_vector_get_element -> given vector is NULL");
        return 0;
    }

    if (index >= vector->size) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_WARNING, "cr_std_vector_get_element -> tried to access out of bounds index -> index : %ld max_index : %ld", index, vector->size - 1);
        return NULL;
    }

    if (vector->is_pointer) {
        // If the vector holds pointers, return the pointer
        return ((void **)vector->elements)[index];
    } else {
        // If the vector holds raw data, return a pointer to the data
        char *base = (char *)vector->elements;
        return (void *)(base + (index * vector->type_size));
    }
}
