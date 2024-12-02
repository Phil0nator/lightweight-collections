#ifndef LCL_RC_H
#define LCL_RC_H

#include "lcl_err.h"
#include "lcl_allocators.h"

typedef size_t lcl_rc_count_t;
typedef struct lcl_rc lcl_rc_t;

lcl_rc_t* lcl_rc_new(void* data, lcl_deallocfn_t dealloc);
lcl_rc_t* lcl_rc_alloc(size_t size);
lcl_rc_t* lcl_rc_ref(lcl_rc_t* rc);
void* lcl_rc_deref(lcl_rc_t* rc);
void** lcl_rc_ptrref(lcl_rc_t* rc);
lcl_rc_count_t lcl_rc_count(lcl_rc_t* rc);

lcl_err_t lcl_rc_free(lcl_rc_t* rc);




#endif