#include "lcl_rc.h"

#include <stdlib.h>
#include <stddef.h>

typedef size_t lcl_rc_count_t;

typedef struct lcl_rc {
    lcl_rc_count_t count;
    lcl_deallocfn_t dealloc;
    void* ptr;
} lcl_rc_t;



lcl_rc_t* lcl_rc_new(void* data, lcl_deallocfn_t dealloc) {
    lcl_rc_t* rc = (lcl_rc_t*) malloc(sizeof(lcl_rc_t));
    rc->count = 1;
    rc->dealloc = dealloc;
    rc->ptr = data;
    return rc;
}
lcl_rc_t *lcl_rc_alloc(size_t size)
{
    lcl_rc_t* rc = (lcl_rc_t*)malloc(sizeof(lcl_rc_t) + size);
    rc->count = 1;
    rc->ptr = &rc[1];
    rc->dealloc = LCL_DEALLOC_NONE;
    return rc;
}
lcl_rc_t *lcl_rc_ref(lcl_rc_t *rc)
{
    rc->count++;
    return rc;
}
void *lcl_rc_deref(lcl_rc_t *rc)
{
    return rc->ptr;
}
void **lcl_rc_ptrref(lcl_rc_t *rc)
{
    return &rc->ptr;
}
lcl_rc_count_t lcl_rc_count(lcl_rc_t *rc)
{
    return rc->count;
}
lcl_err_t lcl_rc_free(lcl_rc_t *rc)
{
    rc->count --;
    if (rc->count == 0) {
        if (rc->ptr != &rc[1]) {
            lcl_dealloc(rc->dealloc, &rc->ptr);
        }
        free(rc);
    }
    return LCL_OK;
}
