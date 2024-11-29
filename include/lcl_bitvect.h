#ifndef LCL_BITVECT_H
#define LCL_BITVECT_H

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t* lcl_bitvect_t;

#include "lcl_vect.h"

lcl_err_t lcl_bitvect_extend_with( lcl_bitvect_t* bvec, uint8_t value, size_t count );

lcl_err_t lcl_bitvect_find_first( const lcl_bitvect_t bvec, bool bit, size_t start, size_t* bitcount );
lcl_err_t lcl_bitvect_find_last( const lcl_bitvect_t bvec, bool bit, size_t* bitcount );

lcl_err_t lcl_bitvect_assign( const lcl_bitvect_t bvec, size_t index, bool bit );
lcl_err_t lcl_bitvect_toggle( const lcl_bitvect_t bvec, size_t index );



#endif