#ifndef CR_STD_VECTOR
#define CR_STD_VECTOR

#include <stdbool.h>
#include <stdio.h>
typedef struct vector_t {
    void *elements;
    size_t size;
    size_t capacity;
    size_t type_size;
    bool is_pointer;
    int (*free_function)(void **);
} vector_t;

/**
 * @brief Creates a new `vector_t` struct.
 *
 * @param `type_size` The `sizeof` the element you want to store (e.g. `sizeof(int)`, `sizeof(string_t *)`).
 * @param `free_function` The custom free function to be called when freeing the vector,
 *                        Use `cr_std_free_ptr` for generic cr_std types that do not have a custom free function, 
 *                        Use `NULL` if you don't want any sub elements freed.
 *                        Use `NULL` for all primitive types.
 *
 * @return A pointer to the new `vector_t` struct.
 * @return `NULL` if allocation fails.
 */
vector_t *cr_std_vector_new(size_t type_size, int (*free_function)(void **));

/**
 * @brief Frees a `vector_t` struct.
 *
 * @param `vector` A pointer to a pointer containing the `vector_t` struct
 *
 * @return `1` on success.
 * @return `0` on failure.
 */
int cr_std_vector_free(vector_t **vector_ptr);

/**
 * @brief Adds an element to the vector.
 *
 * @param `vector` The vector you want to add to.
 * @param `element` The element you want to add.
 *
 * @return `1` on success.
 * @return `0` on failure.
 */
int cr_std_vector_push_back(vector_t *vector, void *element);

/**
 * @brief Removes an element from the vector.
 *
 * @param `vector` The vector you want to remove from.
 * @param `index` The index of the element you want to remove.
 *
 * @return `1` on success.
 * @return `0` on failure.
 */
int cr_std_vector_remove_element(vector_t *vector, size_t index);

/**
 * @brief Fetches an element from the vector.
 *
 * @param `vector` The vector you want to fetch from.
 * @param `index` The index of the element inside the vector.
 *
 * @return A `void *` to the element at the given index.
 * @return `NULL` if the index is not valid.
 */
void *cr_std_vector_get_element(vector_t *vector, size_t index);

/**
 * @brief Extends a vector with another vector does not realloc the elements.
 *
 * @param `dest` The destination vector to add the items too.
 * @param `src` The source vector to add the items from.
 *
 * @return `1` on success.
 * @return `0` on failure.
 */
int cr_std_vector_extend(vector_t *dest, vector_t *src);
#endif // CR_STD_VECTOR
