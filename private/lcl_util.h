#ifndef LCL_UTIL_H
#define LCL_UTIL_H

#include <stdint.h>

#define LCL_DISPSIZE_MAX    sizeof(uint64_t)
#define LCL_ERRPASS(x)  do { lcl_err_t __err = (x); if (__err != LCL_OK) return __err; } while (0)

#endif