#ifndef CR_STD_VECTOR
#define CR_STD_VECTOR

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdio.h>

typedef struct Arena Arena;

/**
 * @brief Dynamic growing vector that stores pointers.
 *
 * - Only stores pointers (void*)
 * - Does NOT copy or free the pointed-to data
 * - Memory for the pointer array is managed by the arena
 * - User is responsible for managing the pointed-to data
 */
typedef struct Vector {
    size_t size;     // Current number of elements
    size_t capacity; // Current capacity of the pointer array
    void **elements; // Array of pointers (void*)
} Vector;

#define CR_STD_VECTOR_DEFAULT_SIZE 8

/**
 * @brief Get an element from the vector with type casting.
 *
 * @param vector The vector
 * @param type The base type (e.g., String, int, MyStruct) - macro adds the *
 * @param index The index of the element
 * @return type* Pointer to the element
 */
#define cr_std_vector_get_at(vector, type, index) ((type *)cr_std_vector_get_element(vector, index))

/**
 * @brief Gets all elements from the vector with type casting.
 *
 * @param `vector` The vector
 * @param `type` The base type (e.g., String, int, MyStruct) - macro adds the **
 *
 * @return type* Pointer to the element
 */
#define cr_std_vector_get_all(vector, type) ((type **)(vector)->elements)

/**
 * @brief Creates a new `Vector` struct.
 *
 * @param `arena` The arena to store the memory in
 *
 * @return A pointer to the new `Vector` struct.
 * @return `NULL` if allocation fails.
 */
Vector *cr_std_vector_new(Arena *arena);

/**
 * @brief Creates a new `Vector` struct, with a set capacity
 *
 * @param `arena` The arena to store the memory in
 * @param `initial_capacity` The initial size of the vector
 *
 * @return A pointer to the new `Vector` struct.
 * @return `NULL` if allocation fails.
 */
Vector *cr_std_vector_newf(Arena *arena, size_t initial_capacity);

/**
 * @brief Adds an element to the vector.
 *
 * @param `arena` The arena to store the memory in case of expansion
 * @param `vector` The vector you want to add to.
 * @param `element` The element you want to add.
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_vector_push_back(Arena *arena, Vector *vector, void *element);

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
 * @brief Extends a vector with another vector does not allocate the elements.
 *
 * @param `arena` The arena to store the memory in case of expansion
 * @param `dest` The destination vector to add the items too.
 * @param `src` The source vector to add the items from.
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_vector_extend(Arena *arena, Vector *dest, Vector *src);

/**
 * @brief Clears the vector (resets size to 0).
 * @note Does NOT free the pointed-to data
 *
 * @param vector The vector to clear
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_vector_clear(Vector *vector);
#ifdef __cplusplus
}
#endif
#endif // CR_STD_VECTOR
