#ifndef CR_STD_UTILS
#define CR_STD_UTILS

#ifdef __cplusplus
extern "C" {
#endif

#define CR_STD_PATH_MAX_SIZE 1024

/**
 * @brief Custom free function used to free my custom cr_std types.
 *
 * @param `ptr` A pointer to a pointer to be freed.
 *
 * @return `0` on success.
 * @return `1` on failure.
 */
int cr_std_free(void **ptr);
#define cr_std_free_ptr ((int (*)(void **))cr_std_free)

#ifdef __cplusplus
}
#endif
#endif // CR_STD_UTILS
