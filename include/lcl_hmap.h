#ifndef LCL_HMAP_H
#define LCL_HMAP_H

#include "lcl_err.h"
#include "lcl_hash.h"
#include "lcl_allocators.h"

#include <stdbool.h>

typedef struct lcl_hmap lcl_hmap_t;

typedef struct {
    lcl_hmap_t* _hmap;
    size_t _index;
} lcl_hmap_iter_t;


lcl_err_t lcl_hmap_init( lcl_hmap_t** dest, lcl_hashfn_t hfn, lcl_deallocfn_t keydealloc );
lcl_err_t lcl_hmap_insert( lcl_hmap_t* dest, void* key, void* data, void** old);
lcl_hmap_iter_t lcl_hmap_find( lcl_hmap_t* hmap, const void* key );
lcl_err_t lcl_hmap_get(lcl_hmap_t* hmap, const void* key, void** data );
bool lcl_hmap_contains( lcl_hmap_t* hmap, const void* key );
lcl_err_t lcl_hmap_remove( lcl_hmap_t* hmap, const void* key );
lcl_err_t lcl_hmap_remove_it( lcl_hmap_t* hmap, lcl_hmap_iter_t iter);

lcl_hmap_iter_t lcl_hmap_begin(lcl_hmap_t* hmap);
lcl_hmap_iter_t lcl_hmap_end( lcl_hmap_t* hmap );
bool lcl_hmap_iter_eq( lcl_hmap_iter_t a, lcl_hmap_iter_t b );
lcl_hmap_iter_t lcl_hmap_iter_next( lcl_hmap_iter_t i );
lcl_hmap_iter_t lcl_hmap_iter_prev( lcl_hmap_iter_t i );
void* lcl_hmap_iter_key( lcl_hmap_iter_t i );
void* lcl_hmap_iter_value( lcl_hmap_iter_t i );

#define lcl_hmap_each(it, hmap)   ( lcl_hmap_iter_t it = lcl_hmap_begin(hmap);\
                                    !lcl_hmap_iter_eq(it, lcl_hmap_end(hmap));\
                                    it = lcl_hmap_iter_next(it) )


lcl_err_t lcl_hmap_display( lcl_hmap_t* hmap, const char* keyfmt, const char* valuefmt );

lcl_err_t lcl_hmap_free( lcl_hmap_t** hmap, lcl_deallocfn_t vdealloc );

#endif