#ifndef CR_STD_VECTOR
#define CR_STD_VECTOR

#include <stdbool.h>
#include <stdio.h>
typedef struct Vector {
    void *elements;
    size_t size;
    size_t capacity;
    size_t type_size;
    bool is_pointer;
    int (*free_function)(void **);
    void *(*copy_function)(void *);
} Vector;

/**
 * @brief Creates a new `Vector` struct.
 *
 * @param `type_size` The `sizeof` the element you want to store (e.g. `sizeof(int)`, `sizeof(String *)`).
 * @param `free_function` The custom free function to be called when freeing the vector,
 *                        Use `cr_std_free_ptr` for generic cr_std types that do not have a custom free function, this just calls stdlib `free`.
 *                        Use `NULL` if you don't want any elements freed, when freeing the vector.
 *                        Use `NULL` for all primitive types, that are not allocated.
 * @param `copy_function` The custom copy function to be called when adding to the vector with push_back, if you want to copy the data over.
 *                        Use `NULL` if you don't want any data to be copied.
 *
 * @return A pointer to the new `Vector` struct.
 * @return `NULL` if allocation fails.
 */
Vector *cr_std_vector_new(size_t type_size, int (*free_function)(void **), void *(*copy_function)(void *src));

/**
 * @brief Creates a new `Vector` struct.
 *                        This function calls `cr_std_vector_new`, with both functions set to `NULL`.
 *
 * @param `type_size` The `sizeof` the element you want to store (e.g. `sizeof(int)`, `sizeof(String *)`).
 *
 * @return A pointer to the new `Vector` struct.
 * @return `NULL` if allocation fails.
 */
Vector *cr_std_vector_new_n(size_t type_size);

/**
 * @brief Frees a `Vector` struct.
 *
 * @param `vector` A pointer to a pointer containing the `Vector` struct
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_vector_free(Vector **vector_ptr);

/**
 * @brief Adds an element to the vector.
 *                  If the vector has a copy function it will copy the data when added.
 *
 * @param `vector` The vector you want to add to.
 * @param `element` The element you want to add.
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_vector_push_back(Vector *vector, void *element);

/**
 * @brief Removes an element from the vector.
 *
 * @param `vector` The vector you want to remove from.
 * @param `index` The index of the element you want to remove.
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_vector_remove_element(Vector *vector, size_t index);

/**
 * @brief Fetches an element from the vector.
 *
 * @param `vector` The vector you want to fetch from.
 * @param `index` The index of the element inside the vector.
 *
 * @return A `void *` to the element at the given index.
 * @return `NULL` if the index is not valid.
 */
void *cr_std_vector_get_element(Vector *vector, size_t index);

/**
 * @brief Extends a vector with another vector does not realloc the elements.
 *
 * @param `dest` The destination vector to add the items too.
 * @param `src` The source vector to add the items from.
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_vector_extend(Vector *dest, Vector *src);
#endif // CR_STD_VECTOR
