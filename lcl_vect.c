#include "lcl_vect.h"
#include <stdlib.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "private/lcl_util.h"


typedef struct lcl_vect_header {
    size_t cap, len, isize;
} lcl_vect_header_t;


#define LCL_VECT_STATIC_SWAPSIZE (64)

#define lcl_vect_get_header(vect) ((lcl_vect_header_t*)(((char*)(vect)) - sizeof(lcl_vect_header_t)))
#define lcl_vect_header_get_start(header) (&(header)[1])
#define lcl_vect_get_indexptr(vect, idx, isize) (((char*)(vect))+((idx)*(isize)))
#define lcl_vectptr_valid(vectptr) ((vectptr) && (*(const char**)(vectptr)))

static size_t lcl_vect_cap_expander(size_t cap) {
    return cap*2;
}

static lcl_err_t lcl_vect_expand( lcl_vect_t* vect, size_t min_size ) {
    lcl_vect_header_t *header = lcl_vect_get_header(*vect);
    size_t new_cap = lcl_vect_cap_expander( header->cap );

    // ensure that min size is met
    if (new_cap < min_size) {
        new_cap = min_size;
    }

    lcl_vect_header_t* newh = realloc( header, sizeof(lcl_vect_header_t) + new_cap * header->isize );
    if (!newh) return LCL_NO_MEMORY;
    newh->cap = new_cap;
    *vect = lcl_vect_header_get_start(newh);
    return LCL_OK;
}



lcl_err_t __lcl_vect_init(lcl_vect_t* vect, size_t initial_capacity, size_t itemsize) {
    size_t total_memory = sizeof(lcl_vect_header_t) + (initial_capacity * itemsize);
    lcl_vect_header_t* header = (lcl_vect_header_t*) malloc(total_memory);
    if (!header) return LCL_NO_MEMORY;
    header->cap = initial_capacity;
    header->isize = itemsize;
    header->len = 0;
    *vect = lcl_vect_header_get_start(header);

    return LCL_OK;
}


size_t lcl_vect_len(const lcl_vect_t vect)
{
    return lcl_vect_get_header(vect)->len;
}

size_t lcl_vect_cap(const lcl_vect_t vect)
{
    return lcl_vect_get_header(vect)->cap;
}
size_t lcl_vect_isize( const lcl_vect_t vect ) {
    return lcl_vect_get_header(vect)->isize;
}

size_t lcl_vect_unused(const lcl_vect_t vect)
{
    lcl_vect_header_t* h =  lcl_vect_get_header(vect);
    return h->cap - h->len;
}

void *__lcl_vect_last(lcl_vect_t vect)
{   
    lcl_vect_header_t* header = lcl_vect_get_header(vect);
    return (char*)vect + ( (header->len - 1) * header->isize );
}

void *__lcl_vect_end(lcl_vect_t vect)
{
    lcl_vect_header_t* header = lcl_vect_get_header(vect);
    return (char*)vect + ( header->len * header->isize );
}

lcl_err_t __lcl_vect_reserve(lcl_vect_t *vect, size_t items)
{
    if (lcl_vectptr_valid(vect)) {
        lcl_vect_header_t* header = lcl_vect_get_header(vect);
        if (header->cap < items) {
            LCL_ERRPASS( lcl_vect_expand( vect, items ) );
        }
    } else return LCL_BAD_ARGUMENT;
    return LCL_OK;
}

lcl_err_t __lcl_vect_extend(lcl_vect_t *vect, const void *data, size_t items)
{
    if (lcl_vectptr_valid(vect)) {
        lcl_vect_header_t *header = lcl_vect_get_header(*vect);
        if (header->cap - header->len < items) {
            LCL_ERRPASS( lcl_vect_expand( vect, (header->len + items) ) );
            header = lcl_vect_get_header(*vect);
        }
        memcpy( __lcl_vect_end(*vect), data, items * header->isize );
        header->len += items;
    } else return LCL_BAD_ARGUMENT;
    return LCL_OK;
}

lcl_err_t __lcl_vect_inserts(lcl_vect_t *vect, size_t index, const void *data, size_t items)
{
    if (lcl_vectptr_valid(vect)) {
        lcl_vect_header_t *header = lcl_vect_get_header(*vect);
        if (index > header->len) return LCL_INDEX_OUT_OF_BOUNDS;

        if (header->cap - header->len < items) {
            LCL_ERRPASS( lcl_vect_expand( vect, (header->len + items) ) );
            header = lcl_vect_get_header( *vect );
        }
        
        memmove( 
            lcl_vect_get_indexptr( *vect, index + items, header->isize ), 
            lcl_vect_get_indexptr( *vect, index, header->isize ), 
            (header->len-index) * header->isize 
        );
        memcpy(  lcl_vect_get_indexptr( *vect, index, header->isize ), data, items * header->isize );
        header->len += items;
    } else return LCL_BAD_ARGUMENT;
    return LCL_OK;
}

lcl_err_t __lcl_vect_splice(lcl_vect_t *vect, size_t index, void *data, size_t count)
{
    if (lcl_vectptr_valid(vect)) {
        lcl_vect_header_t *header = lcl_vect_get_header(*vect);
        if (index > header->len) return LCL_INDEX_OUT_OF_BOUNDS;

        if (data) {
            memcpy(  data, lcl_vect_get_indexptr( *vect, index, header->isize ), count * header->isize );
        }
        
        memmove( 
            lcl_vect_get_indexptr( *vect, index, header->isize ), 
            lcl_vect_get_indexptr( *vect, index + count, header->isize ), 
            (header->len-(index+count)) * header->isize 
        );
        header->len -= count;
    } else return LCL_BAD_ARGUMENT;
    return LCL_OK;
}

lcl_err_t lcl_vect_swap(lcl_vect_t vect, size_t a, size_t b)
{
    if (vect) {
        lcl_vect_header_t* header = lcl_vect_get_header( vect );
        
        // check indices
        if (a >= header->len) return LCL_INDEX_OUT_OF_BOUNDS;
        if (b >= header->len) return LCL_INDEX_OUT_OF_BOUNDS;
        
        // get a and b as pointers
        void* aptr = lcl_vect_get_indexptr( vect, a, header->isize );
        void* bptr = lcl_vect_get_indexptr( vect, b, header->isize );
        
        // keep static buffer for small items in a full vector
        char static_buf[LCL_VECT_STATIC_SWAPSIZE];

        // check if there is extra memory already allocated at the end of the array
        bool full = !(header->len < header->cap);
        bool mallocd = false;
        char* buf;

        if (!full) {
            buf = __lcl_vect_end( vect );
        } else if (header->isize < LCL_VECT_STATIC_SWAPSIZE) {
            buf = static_buf;
        } else {
            buf = malloc(header->isize);
            if (!buf) return LCL_NO_MEMORY;
            mallocd = true;
        }

        memcpy( buf, aptr, header->isize );
        memcpy( aptr, bptr, header->isize );
        memcpy( bptr, buf, header->isize );

        if (mallocd) free(buf);

    } else return LCL_BAD_ARGUMENT;
    return LCL_OK;
}

lcl_err_t lcl_vect_display(const lcl_vect_t vect, const char *fmt)
{
    if (vect) {
        lcl_vect_header_t* header = lcl_vect_get_header( vect );
        if ( header->isize > LCL_DISPSIZE_MAX ) return LCL_BAD_ARGUMENT;
        const char* byte_it = vect;
        printf("[");
        for (size_t i = 0; i < header->len; i++) {
            byte_it += header->isize;
            if (i) printf(", ");
            printf_unkown( byte_it, header->isize, fmt );
        }
        printf("]\n");
    } else return LCL_BAD_ARGUMENT;
    return LCL_OK;
}

lcl_err_t __lcl_vect_clean(lcl_vect_t *vect)
{
    if (lcl_vectptr_valid(vect)) {
        lcl_vect_header_t* header = lcl_vect_get_header(*vect);
        if (header->cap > header->len) {
            lcl_vect_header_t* newh = realloc( header, header->len * header->isize );
            if (!newh) return LCL_FAIL;
            *vect = lcl_vect_header_get_start(newh);
            newh->cap = newh->len;
        }
    } else return LCL_BAD_ARGUMENT;
    return LCL_OK;
}

lcl_err_t __lcl_vect_set_len(lcl_vect_t *vect, size_t len)
{
    if (lcl_vectptr_valid(vect)) {
        lcl_vect_header_t* header = lcl_vect_get_header(*vect);
        if (header->cap > len) {
            header->len = len;
        } else {
            LCL_ERRPASS( lcl_vect_expand( vect, len ) );
            header = lcl_vect_get_header(*vect);
            header->len = len;
        }
    } else return LCL_BAD_ARGUMENT;
    return LCL_OK;
}

lcl_err_t __lcl_vect_free(lcl_vect_t *vect)
{
    if (lcl_vectptr_valid(vect)) {
        lcl_vect_header_t* header = lcl_vect_get_header(*vect);
        free(header);
        *vect = NULL;
    } else return LCL_BAD_ARGUMENT;
    return LCL_OK; 
}

lcl_err_t lcl_vect_debug(const lcl_vect_t vect)
{
    if (vect) {
        lcl_vect_header_t* header = lcl_vect_get_header(vect);
        printf("lcl_vect_t: { len: %lu, cap: %lu, isize: %lu }\n", header->len, header->cap, header->isize);
    } else return LCL_BAD_ARGUMENT;
    return LCL_OK;
}

lcl_err_t __lcl_vect_truncate(lcl_vect_t *vect, void *dest, size_t count)
{
    if (lcl_vectptr_valid(vect)) {
        lcl_vect_header_t* header = lcl_vect_get_header(*vect);
        if (header->len == 0) return LCL_COLLECTION_EMPTY;
        if (header->len < count) return LCL_INDEX_OUT_OF_BOUNDS;
        if (dest) {
            memcpy(
                dest, 
                lcl_vect_get_indexptr( *vect, header->len-count, header->isize ) , 
                header->isize * count 
            );
        }
        header->len -= count;
    } else return LCL_BAD_ARGUMENT;
    return LCL_OK;
}