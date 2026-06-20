#include "cr_std_vector.h"
#include "cr_std_arena.h"
#include "cr_std_logger.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Vector *cr_std_vector_newf(Arena *arena, size_t initial_capacity) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_vector_newf -> arena* was NULL");
        return NULL;
    }

    Vector *vector = cr_std_arena_alloc(arena, sizeof(*vector));

    if (!vector) {
        cr_std_logger_out(CR_STD_LOGGER_LOG_TYPE_ERROR,
                          "cr_std_vector_newf -> failed to allocate memory for new Vector struct");
        return NULL;
    }

    vector->size = 0;
    vector->capacity = initial_capacity;
    vector->elements = cr_std_arena_alloc(arena, vector->capacity * sizeof(void *));

    return vector;
}

Vector *cr_std_vector_new(Arena *arena) {
    return cr_std_vector_newf(arena, CR_STD_VECTOR_DEFAULT_SIZE);
}

b8 cr_std_vector_push_back(Arena *arena, Vector *vector, void *element) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_vector_push_back -> arena* was NULL");
        return CR_STD_FAIL;
    }

    if (!vector) {
        CR_LOG_ERROR("cr_std_vector_push_back -> given vector is NULL");
        return CR_STD_FAIL;
    }

    if (vector->size >= vector->capacity) {
        size_t new_capacity = vector->capacity * 2;
        if (new_capacity == 0) new_capacity = CR_STD_VECTOR_DEFAULT_SIZE;

        void **new_elements = cr_std_arena_alloc(arena, new_capacity * sizeof(void *));
        if (!new_elements) {
            CR_LOG_ERROR("cr_std_vector_push_back -> arena allocation failed for growth");
            return CR_STD_FAIL;
        }

        if (vector->elements) {
            memcpy(new_elements, vector->elements, vector->size * sizeof(void *));
        }

        vector->elements = new_elements;
        vector->capacity = new_capacity;
    }

    vector->elements[vector->size] = element;
    vector->size++;
    return CR_STD_OK;
}

b8 cr_std_vector_remove_element(Vector *vector, size_t index) {
    if (!vector) {
        CR_LOG_ERROR("cr_std_vector_remove_element -> given vector is NULL");
        return CR_STD_FAIL;
    }

    if (index >= vector->size) {
        CR_LOG_ERROR_FMT("cr_std_vector_remove_element -> index out of bounds: %zu >= %zu", index,
                         vector->size);
        return CR_STD_FAIL;
    }

    for (size_t i = index; i < vector->size - 1; i++) {
        vector->elements[i] = vector->elements[i + 1];
    }

    vector->size--;
    return CR_STD_OK;
}

void *cr_std_vector_get_element(Vector *vector, size_t index) {
    if (!vector) {
        CR_LOG_ERROR("cr_std_vector_get_element -> given vector is NULL");
        return NULL;
    }

    if (index >= vector->size) {
        CR_LOG_ERROR_FMT("cr_std_vector_get_element -> index out of bounds: %zu >= %zu", index,
                         vector->size);
        return NULL;
    }

    return vector->elements[index];
}

b8 cr_std_vector_extend(Arena *arena, Vector *dest, Vector *src) {
    if (!arena) {
        CR_LOG_ERROR("cr_std_vector_extend -> arena* was NULL");
        return CR_STD_FAIL;
    }

    if (!dest || !src) {
        CR_LOG_ERROR("cr_std_vector_extend -> given vector(s) is NULL");
        return CR_STD_FAIL;
    }

    for (size_t i = 0; i < src->size; i++) {
        void *element = cr_std_vector_get_element(src, i);
        if (cr_std_vector_push_back(arena, dest, element) != 0) {
            CR_LOG_ERROR("cr_std_vector_extend -> failed to push element");
            return CR_STD_FAIL;
        }
    }
    return CR_STD_OK;
}

b8 cr_std_vector_clear(Vector *vector) {
    if (!vector) {
        CR_LOG_ERROR("cr_std_vector_clear -> given vector is NULL");
        return CR_STD_FAIL;
    }
    vector->size = 0;
    return CR_STD_OK;
}
