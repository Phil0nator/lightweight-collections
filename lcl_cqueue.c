#include "lcl_cqueue.h"

#include <stdlib.h>
#include <memory.h>

typedef struct lcl_cqueue {
    void* dptr;
    size_t isize;
    size_t capacity;

    size_t incoming;
    size_t outgoing;

} lcl_cqueue_t;


lcl_err_t __lcl_cqueue_init(lcl_cqueue_t **dest, void *data, size_t isize, size_t capacity)
{
    
    *dest = malloc(sizeof(lcl_cqueue_t));
    if (!*dest) return LCL_NO_MEMORY;
    (*dest)->dptr = data;
    (*dest)->isize = isize;
    (*dest)->capacity = capacity;
    (*dest)->incoming = 0;
    (*dest)->outgoing = 0;

    return LCL_OK;
}

lcl_err_t lcl_cqueue_alloc(lcl_cqueue_t **dest, size_t isize, size_t cap)
{
    *dest = malloc(sizeof(lcl_cqueue_t) + isize*cap);
    if (!*dest) return LCL_NO_MEMORY;
    (*dest)->dptr = &(*dest)[1];
    (*dest)->isize = isize;
    (*dest)->capacity = cap;
    (*dest)->incoming = 0;
    (*dest)->outgoing = 0;

    return LCL_OK;
}

lcl_err_t lcl_cqueue_enqueue(lcl_cqueue_t *dest, const void *item)
{
    if (!dest || !item) return LCL_BAD_ARGUMENT;
    
    memcpy( &((char*)dest->dptr)[ dest->incoming * dest->isize ], item, dest->isize );
    dest->incoming ++;
    if (dest->incoming >= dest->capacity) {
        dest->incoming = 0;
    }

    return LCL_OK;
}

lcl_err_t lcl_cqueue_dequeue(lcl_cqueue_t *src, void *dst)
{
    if (!src) return LCL_BAD_ARGUMENT;
    

    if (src->outgoing == src->incoming) {
        return LCL_COLLECTION_EMPTY;
    }

    if (dst) {
        memcpy( dst, &((char*)src->dptr)[ src->outgoing * src->isize ], src->isize );
    }
    src->outgoing++;
    if (src->outgoing >= src->capacity) {
        src->outgoing = 0;
    }

    return LCL_OK;
}

lcl_err_t lcl_cqueue_peek(lcl_cqueue_t *src, void *dst)
{
    if (!src || !dst) return LCL_BAD_ARGUMENT;
    
    if (src->outgoing == src->incoming) {
        return LCL_COLLECTION_EMPTY;
    }

    memcpy( dst, &((char*)src->dptr)[ src->outgoing * src->isize ], src->isize );

    return LCL_OK;
}

size_t lcl_cqueue_isize(lcl_cqueue_t *cqueue)
{
    return cqueue->isize;
}

size_t lcl_cqueue_cap(lcl_cqueue_t *cqueue)
{
    return cqueue->capacity;
}

size_t lcl_cqueue_size(lcl_cqueue_t *cqueue)
{
    if (cqueue->incoming > cqueue->outgoing) {
        return cqueue->incoming - cqueue->outgoing;
    } else {
        return cqueue->capacity - ( cqueue->incoming - cqueue->outgoing );
    }
}

void lcl_cqueue_free(lcl_cqueue_t **cqueue)
{
    free(*cqueue);
    *cqueue = NULL;
}
