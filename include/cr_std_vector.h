#ifndef CR_STD_VECTOR
#define CR_STD_VECTOR

#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Dynamic growing vector.
 *
 * Can store any type.
 * For each field see inline comments.
 *
 * Memory ownership:
 * - The Vector owns memory for the elements void *.
 * - The Vector can own memory stored at elements if copy_function is defined.
 * - Freeing a Vector can free the elements if free_function is defined.
 */
typedef struct Vector {
    size_t size;                    // Current size of the elements array.
    size_t capacity;                // Current capacity of the elements.
    size_t type_size;               // The size of the elements being stored in the Vector.
    int (*free_function)(void **);  // If defined; It will free each element when cr_std_vector_free is called.
    void *(*copy_function)(void *); // If defined; It will copy each element when cr_std_vector_push_back is called.
    void *elements;                 // The raw elements pointer, is allocated and owned by the struct.
} Vector;

#define CR_STD_VECTOR_DEFAULT_SIZE 8
#define cr_std_vector_new(type) cr_std_vector_new_t(sizeof(type))
#define cr_std_vector_get_all(vector, type) ((type *)(vector)->elements)
#define cr_std_vector_get_at(vector, type, index) \
    ((type *)cr_std_vector_get_element(vector, index))

/**
 * @brief Creates a new `Vector` struct.
 *
 * @param `type_size` The `sizeof` the element you want to store (e.g. `sizeof(int)`, `sizeof(String *)`).
 * @return A pointer to the new `Vector` struct.
 * @return `NULL` if allocation fails.
 */
Vector *cr_std_vector_new_t(size_t type_size);

/**
 * @brief Frees a `Vector` struct.
 *
 * @param `vector` A pointer to a pointer containing the `Vector` struct
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_vector_free(Vector **vector_ptr);
#define cr_std_vector_free_ptr ((int (*)(void **))cr_std_vector_free)

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

/**
 * @brief Checks if a given vector contains pointers.
 *
 * @param `vector` The vector to check.
 *
 * @return `true` if it contains pointers.
 * @return `false` if it contains data not pointers.
 */
bool cr_std_vector_contains_pointer(Vector *vector);
#endif // CR_STD_VECTOR
