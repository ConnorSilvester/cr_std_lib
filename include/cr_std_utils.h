#ifndef CR_STD_UTILS
#define CR_STD_UTILS

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

#define CR_STD_KB (1024ULL)
#define CR_STD_MB (1024ULL * 1024ULL)
#define CR_STD_GB (1024ULL * 1024ULL * 1024ULL)

#define CR_STD_PATH_MAX_SIZE (CR_STD_KB * 1)
#define CR_STD_FILE_MAX_SIZE (CR_STD_MB * 16)

#define CR_STD_OK 0
#define CR_STD_FAIL 1

// Types
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef i8 b8;

#ifdef __cplusplus
}
#endif
#endif // CR_STD_UTILS
