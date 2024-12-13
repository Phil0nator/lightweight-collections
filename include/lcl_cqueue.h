#ifndef LCL_CQUEUE_H
#define LCL_CQUEUE_H

#include "lcl_err.h"
#include <stddef.h>

typedef struct lcl_cqueue lcl_cqueue_t;

lcl_err_t __lcl_cqueue_init( lcl_cqueue_t** dest, void* data, size_t isize, size_t capacity );
#define lcl_cqueue_init( dest, data, capacity ) __lcl_cqueue_init( dest, data, sizeof(*data), capacity )

lcl_err_t lcl_cqueue_alloc( lcl_cqueue_t** dest, size_t isize, size_t cap );

lcl_err_t lcl_cqueue_enqueue( lcl_cqueue_t *dest, const void* item );
lcl_err_t lcl_cqueue_dequeue( lcl_cqueue_t* src, void* dst );
lcl_err_t lcl_cqueue_peek( lcl_cqueue_t* src, void* dst );

size_t lcl_cqueue_isize(lcl_cqueue_t* cqueue);
size_t lcl_cqueue_cap(lcl_cqueue_t* cqueue);
size_t lcl_cqueue_size(lcl_cqueue_t* cqueue);




void lcl_cqueue_free(lcl_cqueue_t** cqueue);

lcl_err_t lcl_cqueue_display( lcl_cqueue_t* cqueue, const char* fmt);

#endif