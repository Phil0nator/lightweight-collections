#ifndef LCL_MPOOL_H
#define LCL_MPOOL_H

#include <stddef.h>
#include "lcl_err.h"

typedef struct lcl_mpool lcl_mpool_t;

lcl_err_t lcl_mpool_init(lcl_mpool_t** mpool, size_t itemsize);
lcl_err_t lcl_mpool_with_capacity(lcl_mpool_t** mpool, size_t itemsize, size_t capacity);

void* lcl_mpool_alloc(lcl_mpool_t* mpool);
lcl_err_t lcl_mpool_dealloc(lcl_mpool_t* mpool, void** ptr);

lcl_err_t lcl_mpool_free(lcl_mpool_t** mpool);


#endif