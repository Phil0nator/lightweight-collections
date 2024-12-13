#ifndef LCL_LIST_H
#define LCL_LIST_H
#include <stddef.h>
#include <stdbool.h>

#include "lcl_err.h"
#include "lcl_alg.h"


/**
 * @brief A doubly linked list
 * 
 */
typedef struct lcl_list lcl_list_t;

/**
 * @brief A list iterator
 * 
 */
typedef struct lcl_list_it lcl_list_it_t;

/**
 * @brief Initialize a linked list
 * 
 * @param list Pointer to fill with new linked list
 * @param itemsize size of items to be stored
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_list_init( lcl_list_t** list, size_t itemsize);

/**
 * @brief Initialize a linked list from contiguous array
 * 
 * @param list list to init
 * @param elements elements to add
 * @param count number of elements to add
 * @param itemsize sizeof an element
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_list_from( lcl_list_t** list, const void* elements, size_t count, size_t itemsize );

/**
 * @brief Get the length of a linked list
 * 
 * This will be O(1) because the length is stored
 * seperately
 * 
 * @param list list to get length of
 * @return size_t number of items in the list
 */
size_t lcl_list_len( lcl_list_t* list );

/**
 * @brief Get the itemsize of a list
 * 
 * @param list list to get itemsize of
 * @return size_t itemsize
 */
size_t lcl_list_isize( lcl_list_t* list );
void* lcl_list_last( lcl_list_t* list );

/**
 * @brief Get an iterator at the first element of a list
 * 
 * @param list 
 * @return lcl_list_it_t* the first element iterator
 */
lcl_list_it_t* lcl_list_begin( lcl_list_t* list );
/**
 * @brief Get an iterator at the last element of a list
 * 
 * @param list 
 * @return lcl_list_it_t* the last element iterator
 */
lcl_list_it_t* lcl_list_end( lcl_list_t* list );


/**
 * @brief Move an iterator forward by 1
 * 
 * @param it 
 * @return lcl_list_it_t* the iterator moved forward by 1, or NULL
 */
lcl_list_it_t* lcl_list_it_next( lcl_list_it_t* it );

/**
 * @brief Move an iterator back by 1
 * 
 * @param it 
 * @return lcl_list_it_t* the iterator moved back by 1, or NULL
 */
lcl_list_it_t* lcl_list_it_prev( lcl_list_it_t* it );

/**
 * @brief A shorthand for loop header to loop through a list
 * 
 * e.g:
 * 
 * for lcl_list_each(i, myList) {
 *      // print each int in the list
 *      printf("%d", lcl_list_it_get(i));
 * }
 * 
 */
#define lcl_list_each(it, list) \
    ( lcl_list_it_t* it = lcl_list_begin( list ); it ; it = lcl_list_it_next(it) )

/**
 * @brief Get the element pointed to by a list iterator
 * 
 * @param it the iterator
 * @return void* a pointer to the element contained in the list node
 */
void* lcl_list_it_get( lcl_list_it_t* it );

/**
 * @brief Advance an iterator by some number of indexes
 * 
 * @param it Iterator to advance
 * @param by number of indexes to advance
 * @return lcl_err_t OK, or LCL_INDEX_OUT_OF_BOUNDS
 */
lcl_err_t lcl_list_it_advance_by( lcl_list_it_t** it, size_t by );

/**
 * @brief Get a list iterator from a list at an index
 * 
 * @param list list to index
 * @param idx index
 * @return lcl_list_it_t* a list iterator at that index, or NULL 
 */
lcl_list_it_t* lcl_list_index( lcl_list_t* list, size_t idx );

/**
 * @brief Get an iterator to the middle of the list
 * 
 * @param list a list
 * @return lcl_list_it_t* the middle of the list, or NULL if error 
 */
lcl_list_it_t* lcl_list_middle( lcl_list_t* list );

/**
 * @brief Get an iterator to the middle of the list starting at
 *  the given iterator
 * 
 * @param it iterator to start at
 * @return lcl_list_it_t* middle of list starting at it
 */
lcl_list_it_t* lcl_list_it_middle( lcl_list_it_t* it );

/**
 * @brief Add to the end of a list
 * 
 */
#define lcl_list_push( list, data ) lcl_list_extend( list, data, 1 )

/**
 * @brief Extend a list with some data
 * 
 * @param list list to extend
 * @param data elements to add
 * @param items number of elements
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_list_extend( lcl_list_t* list, const void* data, size_t items );

/**
 * @brief remove from the end of the list
 * 
 */
#define lcl_list_pop( list, dest ) lcl_list_truncate( list, dest, 1 )
/**
 * @brief Truncate some number of nodes from the end of the list
 * 
 * @param list list to truncate
 * @param dest destination of elements
 * @param count number of elements
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_list_truncate( lcl_list_t* list, void* dest, size_t count );

/**
 * @brief Insert elements into list
 * 
 * @param list list to insert into
 * @param at iterator location to insert after
 * @param data data to insert
 * @param count number of elements to insert
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_list_inserts( lcl_list_t* list, lcl_list_it_t *at, const void* data, size_t count );

/**
 * @brief Absorb the links of another list at a given index
 * 
 * @param list list to insert into
 * @param at iterator to insert after
 * @param other other list to absorb
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_list_take(lcl_list_t* list, lcl_list_it_t *at, lcl_list_t** other);

/**
 * @brief Absorb the links of another list onto the end of the destination list
 * 
 * @param list list to extend
 * @param other list to absorb
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_list_take_extend(lcl_list_t* list, lcl_list_t** other);

/**
 * @brief Splice elements out of list into contiguous array
 * 
 * @param list list to splice
 * @param at iterator to splice at
 * @param data destination
 * @param count number of elements
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_list_splice( lcl_list_t* list, lcl_list_it_t *at, void* data, size_t count );

/**
 * @brief Splice elements out of this list into a new list
 * 
 * @param list list to splice from
 * @param at iterator to splice at
 * @param dest destination list to initialize
 * @param count number of elements to splice
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_list_splice_links( lcl_list_t* list, lcl_list_it_t* at, lcl_list_t** dest, size_t count );

/**
 * @brief Swap to elements of the list
 * 
 * @param list list to swap within
 * @param a iterator a
 * @param b iterator b
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_list_swap( lcl_list_t* list, lcl_list_it_t* a, lcl_list_it_t* b );

/**
 * @brief Clone a list 
 * 
 * @param list list to clone
 * @return lcl_list_t* the cloned list, or NULL if no memory
 */
lcl_list_t* lcl_list_clone( lcl_list_t* list );

/**
 * @brief Sort a list
 * 
 * @param list list to sort
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_list_sort( lcl_list_t* list, lcl_refcomparator_t cmp );
/**
 * @brief Free a list
 * 
 * @param list list to free
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_list_free(lcl_list_t** list);

/**
 * @brief Print contents of a linked list using the format specifier
 * 
 * @param list list to print
 * @param fmt printf style format specifier
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_list_display(lcl_list_t* list, const char* fmt);


#endif