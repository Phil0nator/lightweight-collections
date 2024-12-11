#ifndef LCL_RC_H
#define LCL_RC_H

#include "lcl_err.h"
#include "lcl_allocators.h"

typedef size_t lcl_rc_count_t;
typedef struct lcl_rc lcl_rc_t;

/**
 * @brief initialize a new reference counter (RC)
 * 
 * @param data pointer to hold
 * @param dealloc deallocator function
 * @return lcl_rc_t* rc pointer
 */
lcl_rc_t* lcl_rc_new(void* data, lcl_deallocfn_t dealloc);

/**
 * @brief Allocate a new reference counter (RC)
 * 
 * @param size size in bytes to allocate
 * @return lcl_rc_t* rc pointer
 */
lcl_rc_t* lcl_rc_alloc(size_t size);

/**
 * @brief Reference an RC (create copy)
 * 
 * @param rc rc to reference
 * @return lcl_rc_t* new copy
 */
lcl_rc_t* lcl_rc_ref(lcl_rc_t* rc);

/**
 * @brief Dereference an RC (retrieve internal pointer)
 * 
 * @param rc rc to dereference
 * @return void* data pointer
 */
void* lcl_rc_deref(lcl_rc_t* rc);

/**
 * @brief Get pointer to the pointer held by the rc
 * 
 * @param rc rc to get double pointer to
 * @return void** double pointer
 */
void** lcl_rc_ptrref(lcl_rc_t* rc);

/**
 * @brief Retrieve the reference count
 * 
 * @param rc rc to check
 * @return lcl_rc_count_t number of references 
 */
lcl_rc_count_t lcl_rc_count(lcl_rc_t* rc);

/**
 * @brief Free an rc
 * 
 * @param rc rc to free
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_rc_free(lcl_rc_t* rc);




#endif