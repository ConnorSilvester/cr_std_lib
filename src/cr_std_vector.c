#include "cr_std_vector.h"
#include "cr_std_logger.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Vector *cr_std_vector_new_t(size_t type_size) {
    if (type_size == 0) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_vector_new -> type_size cannot be zero");
        return NULL;
    }

    Vector *vector = malloc(sizeof(Vector));

    if (!vector) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_vector_new -> failed to allocate memory for new Vector struct");
        return NULL;
    }

    vector->size = 0;
    vector->capacity = CR_STD_VECTOR_DEFAULT_SIZE;
    vector->type_size = type_size;
    vector->free_function = NULL;
    vector->copy_function = NULL;
    vector->elements = malloc(vector->capacity * vector->type_size);

    return vector;
}

int cr_std_vector_free(Vector **vector_ptr) {
    if (!vector_ptr || !*vector_ptr) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_vector_free -> tried to free a NULL vector");
        return 1;
    }

    Vector *vector = *vector_ptr;

    // If the vector holds pointers, free each element using the custom free function
    if (cr_std_vector_contains_pointer(vector)) {
        for (size_t i = 0; i < vector->size; i++) {
            void *element = ((void **)vector->elements)[i];
            if (vector->free_function) { // Check if a custom free function is provided
                int result = vector->free_function(&element);
                if (result != 0) {
                    cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_vector_free -> failed on custom free_function call : element index = %ld", i);
                    return result;
                }
            } else if (element) {
                cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_WARNING, "cr_std_vector_free -> no custom free_function was found for pointer type, element was not freed");
            }
        }
    }

    free(vector->elements);
    free(vector);
    *vector_ptr = NULL;
    return 0;
}

int cr_std_vector_push_back(Vector *vector, void *element) {
    if (!vector) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_vector_push_back -> given vector is NULL");
        return 1;
    }

    if (vector->size >= vector->capacity) {
        vector->capacity *= 2;

        size_t new_size = vector->capacity * vector->type_size;

        void *temp = realloc(vector->elements, new_size);
        if (!temp) {
            cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_vector_push_back -> failed to realloc memory for Vector");
            return 1;
        }
        vector->elements = temp;
    }

    if (cr_std_vector_contains_pointer(vector)) {
        if (vector->copy_function) {
            // Use the custom copy function to create a deep copy
            void *copied_element = vector->copy_function(element);
            if (!copied_element) {
                cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_vector_push_back -> copy_function failed to allocate memory");
                return 1;
            }
            ((void **)vector->elements)[vector->size] = copied_element;
        } else {
            // If none is provided just copy the pointer
            ((void **)vector->elements)[vector->size] = element;
        }
    } else {
        // If storing raw data, copy the data into the vector
        int offset_from_start_of_vector = vector->size * vector->type_size;
        char *dest = (char *)vector->elements + offset_from_start_of_vector;
        memcpy(dest, element, vector->type_size);
    }

    vector->size++;
    return 0;
}

int cr_std_vector_remove_element(Vector *vector, size_t index) {
    if (!vector) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_vector_remove_element -> given vector is NULL");
        return 1;
    }

    if (index >= vector->size || index < 0) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_WARNING, "cr_std_vector_remove_element -> tried to access out of bounds index -> index : %ld max_index : %ld", index, vector->size - 1);
        return 1;
    }

    if (cr_std_vector_contains_pointer(vector)) {
        void *element = ((void **)vector->elements)[index];
        if (vector->free_function && element) {
            int result = vector->free_function(&element);
            if (result != 0) {
                cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_vector_remove_element -> failed on custom free_function call : element index = %ld", index);
                return result;
            }
        }
        for (size_t i = index; i < vector->size - 1; i++) {
            ((void **)vector->elements)[i] = ((void **)vector->elements)[i + 1];
        }
    } else {
        size_t bytes_to_move = (vector->size - index - 1) * vector->type_size;
        if (bytes_to_move > 0) {
            char *dest = (char *)vector->elements + (index * vector->type_size);
            char *src = (char *)vector->elements + ((index + 1) * vector->type_size);
            memmove(dest, src, bytes_to_move);
        }
    }

    vector->size--;
    return 0;
}

void *cr_std_vector_get_element(Vector *vector, size_t index) {
    if (!vector) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_vector_get_element -> given vector is NULL");
        return NULL;
    }

    if (index >= vector->size) {
        cr_std_logger_outf(CR_STD_LOGGER_LOG_TYPE_WARNING, "cr_std_vector_get_element -> tried to access out of bounds index -> index : %ld max_index : %ld", index, vector->size - 1);
        return NULL;
    }

    if (cr_std_vector_contains_pointer(vector)) {
        // If the vector holds pointers, return the pointer
        return ((void **)vector->elements)[index];
    } else {
        // If the vector holds raw data, return a pointer to the data
        char *base = (char *)vector->elements;
        return (void *)(base + (index * vector->type_size));
    }
}

int cr_std_vector_extend(Vector *dest, Vector *src) {
    if (!dest || !src) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_vector_extend -> given vector(s) is NULL");
        return 1;
    }

    if (dest->type_size != src->type_size) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR, "cr_std_vector_extend -> vector type sizes are not the same");
        return 1;
    }

    for (size_t i = 0; i < src->size; i++) {
        cr_std_vector_push_back(dest, cr_std_vector_get_element(src, i));
    }
    return 0;
}

bool cr_std_vector_contains_pointer(Vector *vector) {
    return vector->type_size == sizeof(void *);
}
