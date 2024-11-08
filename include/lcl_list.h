#ifndef LCL_LIST_H
#define LCL_LIST_H
#include <stddef.h>
#include <stdbool.h>

#include "lcl_err.h"

typedef struct lcl_list lcl_list_t;
typedef struct lcl_list_it lcl_list_it_t;

lcl_err_t lcl_list_init( lcl_list_t** list, size_t itemsize);

size_t lcl_list_len( lcl_list_t* list );
size_t lcl_list_isize( lcl_list_t* list );
void* lcl_list_last( lcl_list_t* list );

lcl_list_it_t* lcl_list_begin( lcl_list_t* list );
lcl_list_it_t* lcl_list_end( lcl_list_t* list );

lcl_list_it_t* lcl_list_it_next( lcl_list_it_t* it );
lcl_list_it_t* lcl_list_it_prev( lcl_list_it_t* it );

#define lcl_list_each(it, list) \
    ( lcl_list_it_t* it = lcl_list_begin( list ); it ; it = lcl_list_it_next(it) )

void* lcl_list_it_get( lcl_list_it_t* it );
lcl_err_t lcl_list_it_advance_by( lcl_list_it_t** it, size_t by );

lcl_list_it_t* lcl_list_index( lcl_list_t* list, size_t idx );

#define lcl_list_push( list, data ) lcl_list_extend( list, data, 1 )
lcl_err_t lcl_list_extend( lcl_list_t* list, const void* data, size_t items );

#define lcl_list_pop( list, dest ) lcl_list_truncate( list, dest, 1 )
lcl_err_t lcl_list_truncate( lcl_list_t* list, void* dest, size_t count );

// insert item
// insert list

// splice item
// splice list

lcl_err_t lcl_list_inserts( lcl_list_t* list, lcl_list_it_t *at, const void* data, size_t count );

lcl_err_t lcl_list_splice( lcl_list_t* list, lcl_list_it_t *at, void* data, size_t count );


lcl_err_t lcl_list_swap( lcl_list_t* list, size_t a, size_t b );

lcl_err_t lcl_list_display(lcl_list_t* list, const char* fmt);


#endif