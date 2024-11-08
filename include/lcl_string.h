#ifndef LCL_STRING_H
#define LCL_STRING_H

#include <string.h>

#include "lcl_vect.h"


typedef char* lcl_str_t;

#define lcl_str_init(sptr, capacity) lcl_vect_init( sptr, capacity )

lcl_err_t lcl_str_from( lcl_str_t* sptr, const char* other );
lcl_err_t lcl_str_froms( lcl_str_t* sptr, const char* other, size_t len );

#define lcl_str_len lcl_vect_len
#define lcl_str_cap lcl_vect_cap

#define lcl_str_last( sptr ) (((char*) lcl_vect_last( sptr )) - 1)
#define lcl_str_end( sptr ) (((char*) lcl_vect_end(sptr)) - 1)

lcl_err_t lcl_str_extend( lcl_str_t* sptr, const char* other );
lcl_err_t lcl_str_push(lcl_str_t* sptr, char other);

lcl_err_t lcl_str_truncate( lcl_str_t* sptr, char* dest, size_t count );
#define lcl_str_pop(sptr, dest) lcl_str_truncate( sptr, dest, 1 )

#define lcl_str_inserts lcl_vect_inserts
#define lcl_str_insert lcl_vect_insert

lcl_err_t lcl_str_splice( lcl_str_t* sptr, size_t index, char* data, size_t count );
#define lcl_str_remove(sptr, index, data) lcl_str_splice(sptr, index, data, 1)


#define lcl_str_swap(sptr, a, b) do { char __lcl_strswap = sptr[a]; sptr[a] = sptr[b]; sptr[b] = __lcl_strswap; } while (0) 

#define lcl_str_clean lcl_vect_clean
#define lcl_str_set_len lcl_vect_set_len
#define lcl_str_free lcl_vect_free

#define lcl_str_debug lcl_vect_debug

#endif