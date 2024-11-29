#ifndef LCL_ALG_H
#define LCL_ALG_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "lcl_err.h"


typedef bool (*lcl_unary_predicate_t)(const void* a);
typedef bool (*lcl_binary_predicate_t)(const void* a, const void* b);
typedef void (*lcl_unary_viewer_t)(const void* a);
typedef void (*lcl_unary_modifier_t)(void* a);
typedef void (*lcl_binary_viewer_t)(const void* a, const void* b);
typedef void (*lcl_binary_modifier_t)(void* a, void* b);


#endif