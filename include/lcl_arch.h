#ifndef LCL_ARCH_H
#define LCL_ARCH_H

#include <stdint.h>

#if INTPTR_MAX == INT64_MAX
#   define LCL_SIZEOF_PTR   (8)
#elif INTPTR_MAX == INT32_MAX
#   define LCL_SIZEOF_PTR   (4)
#else
#   error "Unsupported architecture width (requires 32, or 64 bit)"
#endif 





#endif