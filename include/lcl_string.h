#ifndef LCL_STRING_H
#define LCL_STRING_H

#include <string.h>

#include "lcl_vect.h"


typedef char* lcl_str_t;

static inline lcl_err_t lcl_str_init(lcl_str_t* sptr, size_t capacity) 
    { return lcl_vect_init( sptr, capacity ); } 

lcl_err_t lcl_str_from( lcl_str_t* sptr, const char* other );
lcl_err_t lcl_str_froms( lcl_str_t* sptr, const char* other, size_t len );

static inline lcl_err_t lcl_str_len( lcl_str_t sptr ) 
    { return lcl_vect_len( sptr ); }
static inline lcl_err_t lcl_str_cap( lcl_str_t sptr ) 
    { return lcl_vect_cap( sptr ); }
static inline char* lcl_str_last( lcl_str_t sptr ) 
    { return (char*) lcl_vect_last( sptr ) - 1; }
static inline char* lcl_str_end( lcl_str_t sptr ) 
    { return (char*) lcl_vect_end( sptr ) - 1; }



lcl_err_t lcl_str_extend( lcl_str_t* sptr, const char* other );
lcl_err_t lcl_str_push(lcl_str_t* sptr, char other);

lcl_err_t lcl_str_truncate( lcl_str_t* sptr, char* dest, size_t count );
#define lcl_str_pop(sptr, dest) lcl_str_truncate( sptr, dest, 1 )

static inline lcl_err_t lcl_str_inserts( lcl_str_t* sptr, size_t index, const char* data, size_t items )
    { return lcl_vect_inserts( sptr, index, data, items ); }

#define lcl_str_insert(sptr, index, data) lcl_str_inserts( sptr, index, data, 1 )


lcl_err_t lcl_str_splice( lcl_str_t* sptr, size_t index, char* data, size_t count );
#define lcl_str_remove(sptr, index, data) lcl_str_splice(sptr, index, data, 1)


#define lcl_str_swap(sptr, a, b) do { char __lcl_strswap = sptr[a]; sptr[a] = sptr[b]; sptr[b] = __lcl_strswap; } while (0) 

static inline lcl_err_t lcl_str_clean( lcl_str_t* sptr ) 
    { return lcl_vect_clean( sptr ); }
static inline lcl_err_t lcl_str_set_len( lcl_str_t* sptr, size_t len ) 
    { return lcl_vect_set_len( sptr, len ); }
static inline lcl_err_t lcl_str_free( lcl_str_t* sptr ) 
    { return lcl_vect_free(sptr); }

#define lcl_str_debug lcl_vect_debug

#endif