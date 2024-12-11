#ifndef LCL_ALG_H
#define LCL_ALG_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "lcl_err.h"
#include "lcl_any.h"

typedef bool (*lcl_unary_predicate_t)(const lcl_any_t a);
typedef bool (*lcl_binary_predicate_t)(const lcl_any_t a, const lcl_any_t b);
typedef void (*lcl_unary_viewer_t)(const lcl_any_t a);
typedef void (*lcl_unary_modifier_t)(lcl_any_t a);
typedef void (*lcl_binary_viewer_t)(const lcl_any_t a, const lcl_any_t b);
typedef void (*lcl_binary_modifier_t)(lcl_any_t a, lcl_any_t b);
typedef int (*lcl_refcomparator_t)(const void* a, const void* b);

bool lcl_ipeq(const lcl_any_t a, const lcl_any_t b);

#define lcl_u8eq (lcl_ipeq)
#define lcl_i8eq (lcl_ipeq)
#define lcl_u16eq (lcl_ipeq)
#define lcl_i16eq (lcl_ipeq)
#define lcl_u32eq (lcl_ipeq)
#define lcl_i32eq (lcl_ipeq)
#define lcl_f32eq (lcl_ipeq)
#define lcl_ptreq (lcl_ipeq)


bool lcl_streq(const lcl_any_t a, const lcl_any_t b);

#define __lcl_create_cmptp(name, type) \
    static inline int lcl_cmp##name(const void* a, const void* b) \
        { return (int) (*(type *)b - *(type *)a); }

__lcl_create_cmptp(u8, uint8_t);
__lcl_create_cmptp(i8, int8_t);
__lcl_create_cmptp(u16, uint16_t);
__lcl_create_cmptp(i16, int16_t);
__lcl_create_cmptp(u32, uint32_t);
__lcl_create_cmptp(i32, int32_t);
__lcl_create_cmptp(ptr, char*);

static inline int lcl_cmpf32(const void* a, const void* b)
    { float c = (*(float *)b - *(float *)a);
        return c > 0 ? ( 1 ) : ( c == 0 ? 0 : -1 ); }



#endif