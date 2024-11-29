#include "lcl_mpool.h"
#include "private/lcl_util.h"
#include "lcl_bitvect.h"
#include <stdlib.h>
#include <stddef.h>


typedef struct lcl_mpool {
    void* dvec;
    lcl_bitvect_t bvec;
} lcl_mpool_t;

lcl_err_t lcl_mpool_init(lcl_mpool_t** mpool, size_t itemsize) {
    return lcl_mpool_with_capacity(mpool, itemsize,1);
}


lcl_err_t lcl_mpool_with_capacity(lcl_mpool_t** mpool, size_t itemsize, size_t capacity) {
    *mpool = malloc(sizeof(lcl_mpool_t));
    if (!*mpool) return LCL_NO_MEMORY;

    LCL_ERRPASS(__lcl_vect_init( (lcl_vect_t*) &(*mpool)->dvec, capacity * itemsize, itemsize));
    LCL_ERRPASS(__lcl_vect_init( (lcl_vect_t*) &(*mpool)->bvec, capacity / 8, 1) );

    return LCL_OK;

}

static lcl_err_t lcl_mpool_expand(lcl_mpool_t* mpool) {
    LCL_ERRPASS(lcl_bitvect_extend_with( &mpool->bvec, 0x00, 1 ));
    LCL_ERRPASS(lcl_vect_set_len( &mpool->dvec, lcl_vect_len(mpool->dvec) + 8 ));
    return LCL_OK;
}

void* lcl_mpool_alloc(lcl_mpool_t* mpool) {
    if (!mpool) return NULL;
    size_t bcount;
    lcl_err_t err;


    if ((err = lcl_bitvect_find_first( mpool->bvec, false, 0, &bcount )) == LCL_NOT_FOUND) {
        if (lcl_mpool_expand(mpool) != LCL_OK) return NULL;
        bcount = (lcl_vect_len(mpool->bvec) - 1) * 8;
    }

    if(lcl_bitvect_assign( mpool->bvec, bcount, true ) != LCL_OK) return NULL;
    size_t isize = lcl_vect_isize(mpool->dvec);
    return (char*)mpool->dvec + (isize * bcount);

    
}
lcl_err_t lcl_mpool_dealloc(lcl_mpool_t *mpool, void **ptr)
{
    if(!mpool || !ptr) return LCL_BAD_ARGUMENT;
    
    ptrdiff_t offset = (uint8_t*)(*ptr) - (uint8_t*) mpool->dvec;
    offset /= lcl_vect_isize(mpool->dvec);

    size_t bvec_len = lcl_vect_len(mpool->bvec);
    size_t bvec_bits = bvec_len * 8;
    if ( offset >= bvec_bits ) {
        return LCL_FAIL;
    }

    LCL_ERRPASS(lcl_bitvect_assign( mpool->bvec, offset, false ));
    size_t offsetbyte = offset / 8;
    if ( (offsetbyte == bvec_len - 1) && (mpool->bvec[offsetbyte] == 0x00)) {
        lcl_vect_pop( &mpool->bvec, NULL );
        lcl_vect_clean( &mpool->bvec );
    }

    *ptr = NULL;

    return LCL_OK;
}

lcl_err_t lcl_mpool_free(lcl_mpool_t** mpool) {
    lcl_vect_free(&(*mpool)->bvec);
    lcl_vect_free(&(*mpool)->dvec);
    free(*mpool);
    *mpool = NULL;
    return LCL_OK;
}
