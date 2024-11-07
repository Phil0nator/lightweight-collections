#ifndef LCL_ALG_H
#define LCL_ALG_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "lcl_vect.h"

typedef bool (*lcl_unary_predicate_t)(const void* a);
typedef bool (*lcl_binary_predicate_t)(const void* a, const void* b);

lcl_err_t lcl_vect_find_by( const lcl_vect_t v, lcl_unary_predicate_t p, size_t* index_out );


#endif