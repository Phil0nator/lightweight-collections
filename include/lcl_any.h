#ifndef LCL_ANY_H
#define LCL_ANY_H

#include <stdint.h>
#include "lcl_arch.h"

/**
 * @brief Type representing something that can be any type
 * 
 * This falls into 1 of 2 categories:
 * 
 *  Register-sized, in-place data.
 * 
 * For example, on 32-bit or larger architectures, an any_t
 * can store any int type 32 bits or less. This is stored in-place,
 * and the fact that this is a void* pointer is ignored.
 * 
 *  Unkown Pointer Data
 * 
 * This is the typical void* pointer that could point to anything
 * 
 */
typedef void* lcl_any_t;

static inline uint8_t lcl_any2u8( lcl_any_t a )
    { return *(uint8_t*)(&a); }
static inline int8_t lcl_any2i8( lcl_any_t a )
    { return *(int8_t*)(&a); }
static inline uint16_t lcl_any2u16( lcl_any_t a )
    { return *(uint16_t*)(&a); }
static inline int16_t lcl_any2i16( lcl_any_t a )
    { return *(int8_t*)(&a); }
static inline uint32_t lcl_any2u32( lcl_any_t a )
    { return *(uint32_t*)(&a); }
static inline int32_t lcl_any2i32( lcl_any_t a )
    { return *(int32_t*)(&a); }
static inline int lcl_any2i(lcl_any_t a) 
    { return *(int*)(&a); }
static inline int lcl_any2u(lcl_any_t a) 
    { return *(unsigned*)(&a); }
static inline float lcl_any2f(lcl_any_t a)
    { return *(float*)(&a); }
static inline char* lcl_any2str(lcl_any_t a)
    { return (char*)(a); }
static inline void* lcl_any2ptr(lcl_any_t a)
    { return (void*)a; }

static inline lcl_any_t lcl_any_u8(uint8_t u8)
    { lcl_any_t a; *(uint8_t*)(&a) = u8; return a; }
static inline lcl_any_t lcl_any_i8(int8_t i8)
    { lcl_any_t a; *(int8_t*)(&a) = i8; return a; }
static inline lcl_any_t lcl_any_u16(uint16_t u16)
    { lcl_any_t a; *(uint16_t*)(&a) = u16; return a; }
static inline lcl_any_t lcl_any_i16(int16_t i16)
    { lcl_any_t a; *(int16_t*)(&a) = i16; return a; }
static inline lcl_any_t lcl_any_u32(uint32_t u32)
    { lcl_any_t a; *(uint32_t*)(&a) = u32; return a; }
static inline lcl_any_t lcl_any_i32(int32_t i32)
    { lcl_any_t a; *(int32_t*)(&a) = i32; return a; }
static inline lcl_any_t lcl_any_f(float f)
    { lcl_any_t a; *(float*)(&a) = f; return a; }
static inline lcl_any_t lcl_any_str(char* s)
    { return (lcl_any_t)s; }
static inline lcl_any_t lcl_any_ptr(void* p)
    { return (lcl_any_t)p; }
static inline lcl_any_t lcl_any(void* p)
    { return (lcl_any_t)p; }


#if LCL_SIZEOF_PTR == 8

static inline uint64_t lcl_any2u64( lcl_any_t a )
    { return *(uint64_t*)(&a); }
static inline int64_t lcl_any2i64( lcl_any_t a )
    { return *(int64_t*)(&a); }
static inline double lcl_any2d(lcl_any_t a)
    { return *(double*)(&a); }

static inline lcl_any_t lcl_any_d(double d)
    { lcl_any_t a; *(double*)(&a) = d; return a; }
static inline lcl_any_t lcl_any_u64(uint64_t u64)
    { lcl_any_t a; *(uint64_t*)(&a) = u64; return a; }
static inline lcl_any_t lcl_any_i64(int64_t i64)
    { lcl_any_t a; *(int64_t*)(&a) = i64; return a; }

#endif

#define LCL_ANY_MK_NULL (0)
#define LCL_ANY_MK_U8   (1)
#define LCL_ANY_MK_I8   (2)
#define LCL_ANY_MK_U16  (3)
#define LCL_ANY_MK_I16  (4)
#define LCL_ANY_MK_U32  (5)
#define LCL_ANY_MK_I32  (6)
#define LCL_ANY_MK_U64  (7)
#define LCL_ANY_MK_I64  (8)
#define LCL_ANY_MK_FLT  (9)
#define LCL_ANY_MK_DBL  (10)
#define LCL_ANY_MK_CSTR (11)
#define LCL_ANY_MK_PTR  (12)

typedef struct lcl_marked_any {
    lcl_any_t value;
    uint8_t marker;
} lcl_marked_any_t;


#endif