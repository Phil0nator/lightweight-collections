#include "lcl_alg.h"


lcl_err_t lcl_vect_find_by( const lcl_vect_t v, lcl_unary_predicate_t p, size_t* index_out ) {
    if (v) {
        size_t isize = lcl_vect_isize(v);
        size_t len = lcl_vect_len(v);
        *index_out = 0;
        for (size_t byte = 0; *index_out < len; (*index_out)++, byte += isize ) {
            if ( p((char*)v + byte) ) return LCL_OK;
        }
        *index_out = -1;
    } else return LCL_BAD_ARGUMENT;
    return LCL_OK;
}