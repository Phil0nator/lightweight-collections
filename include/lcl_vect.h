#ifndef LCL_VECT_H
#define LCL_VECT_H

#include <stddef.h>
#include "lcl_err.h"


typedef void* lcl_vect_t;

/**
 * @brief initialize a vect with a starting capacity
 * 
 * @returns lcl_err_t any errors
 */
#define lcl_vect_init( vectptr, capacity ) __lcl_vect_init( (lcl_vect_t*) vectptr, capacity, sizeof(**(vectptr)) )
lcl_err_t __lcl_vect_init( lcl_vect_t* vect, size_t initial_capacity, size_t itemsize);

/**
 * @brief Create a vect from a starting piece of data
 * 
 * @param vectptr vect to init
 * @param data initial data to copy
 * @param len number of elements to copy
 */
#define lcl_vect_from(vectptr, data, len) __lcl_vect_from( (lcl_vect_t*) vectptr, data, sizeof(**(vectptr)), len )
lcl_err_t __lcl_vect_from( lcl_vect_t* vect, const void* data, size_t itemsize, size_t items );

/**
 * @brief Clone a vect to a new vect
 * 
 * @param vect a vect to clone
 * @return lcl_vect_t a new vect with the same length and data, 
 *  or NULL if no memory is available 
 */
lcl_vect_t lcl_vect_clone(const lcl_vect_t vect);

/**
 * @brief Get the len of vect
 * 
 * @warning there is no NULL check on this argument
 * 
 * @param vect a vect to get the length of
 * @return size_t the length of the vect
 */
size_t lcl_vect_len( const lcl_vect_t vect );
/**
 * @brief Get the capacity of vect
 * 
 * @warning there is no NULL check on this argument
 * 
 * @param vect a vect to get the capacity of
 * @return size_t the capacity of the vect
 */
size_t lcl_vect_cap( const lcl_vect_t vect );
/**
 * @brief Get the item size of vect
 * 
 * @warning there is no NULL check on this argument
 * 
 * @param vect a vect to get the item size of
 * @return size_t the item size of the vect
 */
size_t lcl_vect_isize( const lcl_vect_t vect );
/**
 * @brief Get the number of allocated free items available
 *  in this vect
 * @param vect a vect to check
 * @return size_t the number of allocated free items available
 */
size_t lcl_vect_unused( const lcl_vect_t vect );

/**
 * @brief Get a pointer to the last item of a vect
 * 
 * @warning there is no NULL check on this argument
 * 
 * @param vect a vect to get the last item of
 * @return void* a pointer to the last element
 */
#define lcl_vect_last( vect ) __lcl_vect_last( (lcl_vect_t*) vect )
void* __lcl_vect_last( lcl_vect_t vect );

/**
 * @brief Get the 'end' pointer of this vect. The 'end'
 *  pointer points just past the last element of the vect.
 * @warning there is no NULL check on this argument
 * 
 * @param vect a vect to get the end of
 * @return void* the end of the vector
 */
#define lcl_vect_end( vect ) __lcl_vect_end( (lcl_vect_t*) vect )
void* __lcl_vect_end( lcl_vect_t vect );

/**
 * @brief Reserve at least 'items' items of capacity
 * @returns any errors
 */
#define lcl_vect_reserve( vectptr, items ) __lcl_vect_reserve( (lcl_vect_t*) vectptr, items )
lcl_err_t __lcl_vect_reserve( lcl_vect_t* vect, size_t items );

/**
 * @brief extend a vector with a given buffer of items
 * @param vectptr a vect to extend
 * @param data the data to add to the end
 * @param items the number of items to add to the end
 * @returns any errors
 */
#define lcl_vect_extend( vectptr, data, items) __lcl_vect_extend((lcl_vect_t*) vectptr,  data, items)
/**
 * @brief Push an item to the end of the vector
 * @param vectptr the vect to push to 
 * @param data the item to push
 * @returns any errors
 */
#define lcl_vect_push( vectptr, data ) __lcl_vect_extend( (lcl_vect_t*) vectptr, data, 1 )
lcl_err_t __lcl_vect_extend( lcl_vect_t* vect, const void* data, size_t items );

/**
 * @brief Truncate the vector by some amount
 * @param vectptr vect to modify
 * @param dest destination to copy truncated elements into
 *  (May be NULL)
 * @param count number of elements to remove from the
 *  end of the vect
 * @returns any errors
 */
#define lcl_vect_truncate( vectptr, dest, count ) __lcl_vect_truncate( (lcl_vect_t*) vectptr, dest, count )
/**
 * @brief Pop a single item off the end of the vect
 * @param vectptr vect to pop from 
 * @param dest destination to store popped item
 *  (May be NULL)
 * @returns any error
 */
#define lcl_vect_pop( vectptr, dest ) __lcl_vect_truncate( (lcl_vect_t*) vectptr, dest, 1 )
lcl_err_t __lcl_vect_truncate( lcl_vect_t* vect, void* dest, size_t count );

/**
 * @brief Insert items into the middle of a vect
 * @param vectptr vect to modify
 * @param index index to insert at
 *  (The item at vect[index] after this call will be data[0])
 * @param data the data to insert
 * @param items the number of items to insert
 * @returns any errors
 */
#define lcl_vect_inserts(vectptr, index, data, items) __lcl_vect_inserts( (lcl_vect_t*) vectptr, index, data, items )

/**
 * @brief Insert an item into the middle of a vect
 * @param vectptr vect to modify
 * @param index index to insert at
 *  (The item at vect[index] after this call will be '*data')
 * @param data the data to insert
 * @returns any errors
 */
#define lcl_vect_insert(vectptr, index, data) __lcl_vect_inserts( (lcl_vect_t*) vectptr, index, data, 1 )
lcl_err_t __lcl_vect_inserts(lcl_vect_t* vect, size_t index, const void* data, size_t items);

/**
 * @brief Remove a series of items from the middle of a vect
 * @param vectptr vect to modify
 * @param index index to begin removing at (inclusive)
 * @param data destination buffer to copy removed items to
 *  (May be NULL)
 * @param count the number of items to remove from the vect
 * @returns any errors
 */
#define lcl_vect_splice(vectptr, index, data, count) __lcl_vect_splice( (lcl_vect_t*) vectptr, index, data, count )
/**
 * @brief Remove an item from the middle of a vect
 * @param vectptr vect to modify
 * @param index index to remove
 * @param data destination buffer to copy removed item to
 *  (May be NULL)
 * @returns any errors
 */
#define lcl_vect_remove(vectptr, index, data) __lcl_vect_splice((lcl_vect_t*) vectptr, index, data, 1)
lcl_err_t __lcl_vect_splice(lcl_vect_t* vect, size_t index, void* data, size_t count);

/**
 * @brief Swap two items of a vect
 * 
 * @param vect vect to modify
 * @param a index A
 * @param b index B
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_vect_swap( lcl_vect_t vect, size_t a, size_t b );


/**
 * @brief Cleanup extra unused capcity of a vect
 * @param vectptr vect to clean
 * @returns any errors
 */
#define lcl_vect_clean(vectptr) __lcl_vect_clean( (lcl_vect_t*) vectptr )
lcl_err_t __lcl_vect_clean(lcl_vect_t* vect);

/**
 * @brief Force the length value of a vect
 * 
 * If capacity is not enough to hold this length, it will be expanded
 * to at least this length. If capacity is already enough to hold this
 * length, the internal length count is simply assigned to this new value.
 * 
 * Two example use cases include:
 * 
 * To 'clear' the vect without freeing memory:
 * e.g:
 * ```
 *  lcl_vect_set_len( &my_vect, 0 );
 * ```
 * 
 * For loading dynamically sized data into a buffer:
 * e.g:
 * ```
 *  size_t my_length = get_dynamic_length_of_something();
 *  lcl_vect_set_len(&my_vect, my_length);
 *  read_dynamic_thing_of_length_into(my_vect);
 * ```
 * 
 * 
 * @param vectptr vect to modify
 * @param len length to force
 * @returns any errors
 */
#define lcl_vect_set_len(vectptr, len) __lcl_vect_set_len( (lcl_vect_t*) vectptr, len )
lcl_err_t __lcl_vect_set_len(lcl_vect_t* vect, size_t len);

/**
 * @brief Free all memory used by vect
 * @param vectptr vect to free
 */
#define lcl_vect_free(vectptr) __lcl_vect_free( (lcl_vect_t*) vectptr )
lcl_err_t __lcl_vect_free(lcl_vect_t* vect);

/**
 * @brief Debug internal values of vect
 * 
 * @param vect vect to debug
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_vect_debug(const lcl_vect_t vect);


/**
 * @brief Display each element of the vect with a printf format specifier
 * 
 * @param vect vect to printf
 * @param fmt format specifier to apply to each element
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_vect_display(const lcl_vect_t vect, const char* fmt);

#endif 