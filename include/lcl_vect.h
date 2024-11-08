#ifndef LCL_VECT_H
#define LCL_VECT_H

#include <stddef.h>
#include "lcl_err.h"


typedef void* lcl_vect_t;

#define lcl_vect_init( vectptr, capacity ) __lcl_vect_init( (lcl_vect_t*) vectptr, capacity, sizeof(**vectptr) )
lcl_err_t __lcl_vect_init( lcl_vect_t* vect, size_t initial_capacity, size_t itemsize);


size_t lcl_vect_len( const lcl_vect_t vect );
size_t lcl_vect_cap( const lcl_vect_t vect );
size_t lcl_vect_isize( const lcl_vect_t vect );
size_t lcl_vect_unused( const lcl_vect_t vect );

#define lcl_vect_last( vect ) __lcl_vect_last( (lcl_vect_t*) vect )
void* __lcl_vect_last( lcl_vect_t vect );

#define lcl_vect_end( vect ) __lcl_vect_end( (lcl_vect_t*) vect )
void* __lcl_vect_end( lcl_vect_t vect );

#define lcl_vect_reserve( vectptr, items ) __lcl_vect_reserve( (lcl_vect_t*) vectptr, items )
lcl_err_t __lcl_vect_reserve( lcl_vect_t* vect, size_t items );

#define lcl_vect_extend( vectptr, data, items) __lcl_vect_extend((lcl_vect_t*) vectptr,  data, items)
#define lcl_vect_push( vectptr, data ) __lcl_vect_extend( (lcl_vect_t*) vectptr, data, 1 )
lcl_err_t __lcl_vect_extend( lcl_vect_t* vect, const void* data, size_t items );

#define lcl_vect_truncate( vectptr, dest, count ) __lcl_vect_truncate( (lcl_vect_t*) vectptr, dest, count )
#define lcl_vect_pop( vectptr, dest ) __lcl_vect_truncate( (lcl_vect_t*) vectptr, dest, 1 )
lcl_err_t __lcl_vect_truncate( lcl_vect_t* vect, void* dest, size_t count );

#define lcl_vect_inserts(vectptr, index, data, items) __lcl_vect_inserts( (lcl_vect_t*) vectptr, index, data, items )
#define lcl_vect_insert(vectptr, index, data) __lcl_vect_inserts( (lcl_vect_t*) vectptr, index, data, 1 )
lcl_err_t __lcl_vect_inserts(lcl_vect_t* vect, size_t index, const void* data, size_t items);

#define lcl_vect_splice(vectptr, index, data, count) __lcl_vect_splice( (lcl_vect_t*) vectptr, index, data, count )
#define lcl_vect_remove(vectptr, index, data) __lcl_vect_splice((lcl_vect_t*) vectptr, index, data, 1)
lcl_err_t __lcl_vect_splice(lcl_vect_t* vect, size_t index, void* data, size_t count);


lcl_err_t lcl_vect_swap( lcl_vect_t vect, size_t a, size_t b );



#define lcl_vect_clean(vectptr) __lcl_vect_clean( (lcl_vect_t*) vectptr )
lcl_err_t __lcl_vect_clean(lcl_vect_t* vect);
#define lcl_vect_set_len(vectptr, len) __lcl_vect_set_len( (lcl_vect_t*) vectptr, len )
lcl_err_t __lcl_vect_set_len(lcl_vect_t* vect, size_t len);
#define lcl_vect_free(vectptr) __lcl_vect_free( (lcl_vect_t*) vectptr )
lcl_err_t __lcl_vect_free(lcl_vect_t* vect);

lcl_err_t lcl_vect_debug(const lcl_vect_t vect);

lcl_err_t lcl_vect_display(const lcl_vect_t vect, const char* fmt);

#endif 