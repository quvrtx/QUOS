#ifndef STDINT_H
#define STDINT_H

typedef signed char int8_t;
typedef unsigned char uint8_t;

typedef short int16_t;
typedef unsigned short uint16_t;

typedef int int32_t;
typedef unsigned int uint32_t;

#if defined(__x86_64__) || defined(__aarch64__) || defined(__LP64__)
typedef long int64_t;
typedef unsigned long uint64_t;
#else
typedef long long int64_t;
typedef unsigned long long uint64_t;
#endif

#if __SIZEOF_POINTER__ == 4
typedef uint32_t uintptr_t;
typedef int32_t intptr_t;
#elif __SIZEOF_POINTER__ == 8
typedef uint64_t uintptr_t;
typedef int64_t intptr_t;
#else
#error "Unsupported pointer size"
#endif

#if __SIZEOF_POINTER__ == 8
typedef uint64_t uintmax_t;
typedef int64_t intmax_t;
#else
typedef uint32_t uintmax_t;
typedef int32_t intmax_t;
#endif

#define INT8_MIN   (-128)
#define INT16_MIN  (-32768)
#define INT32_MIN  (-2147483647 - 1)
#define INT64_MIN  (-9223372036854775807LL - 1)

#define INT8_MAX   127
#define INT16_MAX  32767
#define INT32_MAX  2147483647
#define INT64_MAX  9223372036854775807LL

#define UINT8_MAX  255
#define UINT16_MAX 65535
#define UINT32_MAX 4294967295U
#define UINT64_MAX 18446744073709551615ULL

#endif //STDINT_H