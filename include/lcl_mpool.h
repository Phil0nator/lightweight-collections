#ifndef LCL_MPOOL_H
#define LCL_MPOOL_H

#include <stddef.h>
#include "lcl_err.h"

typedef struct lcl_mpool lcl_mpool_t;

/**
 * @brief Initialize a memory pool
 * 
 * @param mpool mpool to init
 * @param itemsize size of items for the memory pool
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_mpool_init(lcl_mpool_t** mpool, size_t itemsize);

/**
 * @brief Initialize a memory pool with an initial capacity
 * 
 * @param mpool mpool to init
 * @param itemsize size of items in the memory pool
 * @param capacity number of items to initialize
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_mpool_with_capacity(lcl_mpool_t** mpool, size_t itemsize, size_t capacity);

/**
 * @brief Allocate from an mpool
 * 
 * @param mpool mpool to alloc from
 * @return void* a pointer to the allocated space of (itemsize) bytes
 */
void* lcl_mpool_alloc(lcl_mpool_t* mpool);

/**
 * @brief Deallocate an allocation from mpool_alloc()
 * 
 * @param mpool mpool to dealloc into
 * @param ptr pointer from mpool_alloc()
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_mpool_dealloc(lcl_mpool_t* mpool, void** ptr);

/**
 * @brief Free a memory pool
 * 
 * @param mpool mpool to free
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_mpool_free(lcl_mpool_t** mpool);


#endif