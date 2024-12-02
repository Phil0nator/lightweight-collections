#ifndef LCL_ALLOCATORS_H
#define LCL_ALLOCATORS_H

#include <stdlib.h>


typedef void (*lcl_deallocfn_t)(void**);

#define LCL_DEALLOC_NONE    (NULL)
#define LCL_DEALLOC_FREE    ((lcl_deallocfn_t)(free))

static inline void lcl_dealloc( lcl_deallocfn_t dealloc, void** ptr ) {
    if (dealloc) {
        if (dealloc == LCL_DEALLOC_FREE) 
        {
            free(*ptr);
            *ptr = NULL;
        }
        else dealloc(ptr);
    }
}


#endif