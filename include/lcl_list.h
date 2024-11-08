#ifndef LCL_LIST_H
#define LCL_LIST_H
#include <stddef.h>
#include <stdbool.h>

#include "lcl_err.h"

typedef struct lcl_list lcl_list_t;

lcl_err_t lcl_list_init( lcl_list_t** list, size_t initial_capacity, size_t itemsize);

size_t lcl_list_len( lcl_list_t* list );
size_t lcl_list_cap( lcl_list_t* list );
size_t lcl_list_isize( lcl_list_t* list );
void* lcl_list_last( lcl_list_t* list );
void* lcl_list_end( lcl_list_t* list );

#define lcl_list_push( list, data ) lcl_list_extend( list, data, 1 )
lcl_err_t lcl_list_extend( lcl_list_t* list, const void* data, size_t items );

#define lcl_list_pop( list, dest ) lcl_list_truncate( list, dest, 1 )
lcl_err_t lcl_list_truncate( lcl_list_t* list, void* dest, size_t count );

// insert item
// insert list

// splice item
// splice list

lcl_err_t lcl_list_swap( lcl_list_t* list, size_t a, size_t b );

lcl_err_t lcl_list_display(lcl_list_t* list, const char* fmt);


#endif