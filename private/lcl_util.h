#ifndef LCL_UTIL_H
#define LCL_UTIL_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "lcl_err.h"

#define LCL_DISPSIZE_MAX    sizeof(uint64_t)
#define LCL_ERRPASS(x)  do { lcl_err_t __err = (x); if (__err != LCL_OK) { printf("%s:%d: Error: %s\n", __FILE__, __LINE__, lcl_strerror(__err)); return __err; } } while (0)

static inline void printf_unkown( const void* unkown, size_t size, const char* fmt ) {
    uint64_t data;
    memcpy( &data, unkown, size );
    printf(fmt, data);
}

#endif