#ifndef LCL_HMAP_H
#define LCL_HMAP_H

#include "lcl_err.h"
#include "lcl_hash.h"
#include "lcl_allocators.h"
#include "lcl_any.h"
#include "lcl_alg.h"

#include <stdbool.h>

typedef struct lcl_hmap lcl_hmap_t;

typedef struct {
    lcl_hmap_t* _hmap;
    size_t _index;
} lcl_hmap_iter_t;


/**
 * @brief Initialize a hashmap
 * 
 * @param dest hmap pointer to init
 * @param hfn hashing function (see lcl_hash.h)
 * @param keyeq binary predicate to compare keys (see lcl_alg.h)
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_hmap_init( lcl_hmap_t** dest, lcl_hashfn_t hfn, lcl_binary_predicate_t keyeq );

/**
 * @brief Insert or update values in the hashmap
 * 
 * @param dest hmap to insert into
 * @param key key to insert at
 * @param data data to insert
 * @param old Any old data that may have been 
 *  in this location is written here
 * @param old_written true is written if old was written
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_hmap_insert( lcl_hmap_t* dest, lcl_any_t key, lcl_any_t data, lcl_any_t* old, bool* old_written);


/**
 * @brief Find an iterator to the given key in the table
 * 
 * @param hmap map
 * @param key key to find
 * @return lcl_hmap_iter_t an iterator to this key, or hmap_end() if not found 
 */
lcl_hmap_iter_t lcl_hmap_find( lcl_hmap_t* hmap, lcl_any_t key );

/**
 * @brief Get a value from the hashmap
 * 
 * @param hmap map
 * @param key key to search
 * @param data output to write value to
 * @return lcl_err_t any errors - NOT_FOUND if the key cannot be found
 */
lcl_err_t lcl_hmap_get(lcl_hmap_t* hmap, lcl_any_t key, lcl_any_t* data );

/**
 * @brief Check if the hmap contains the given key
 * 
 * @param hmap hmap to search
 * @param key key to search for
 * @return true 
 * @return false 
 */
bool lcl_hmap_contains( lcl_hmap_t* hmap, lcl_any_t key );

/**
 * @brief Remove an item from the hmap
 * 
 * @param hmap hmap to remove from
 * @param key key to remove
 * @param keyout internal key output of key removed
 * @param valout value output of value removed
 * @return true: value existed and was removed
 * @return false: value did not exist and could not be removed 
 */
bool lcl_hmap_remove( lcl_hmap_t* hmap, lcl_any_t key, lcl_any_t *keyout, lcl_any_t* valout );

/**
 * @brief Remove an item from the hmap at a given iterator
 * 
 * @param iter iterator to remove at
 * @param keyout internal key output of key removed
 * @param valout value output of value removed
 * @return true: iterator was valid and key-value was removed
 * @return false iterator was hmap_end() so no action was taken
 */
bool lcl_hmap_remove_it(lcl_hmap_iter_t iter, lcl_any_t *keyout, lcl_any_t* valout);

/**
 * @brief Get an iterator to the beginning of the map
 * 
 * @param hmap hmap to iterate
 * @return lcl_hmap_iter_t iterator to the first key-value pair
 *  in the hash table
 */
lcl_hmap_iter_t lcl_hmap_begin(lcl_hmap_t* hmap);

/**
 * @brief Get an iterator to the end of a map,
 *  i.e just after the last element.
 * 
 * @param hmap hmap to iterate
 * @return lcl_hmap_iter_t iterator just past the last
 *  element
 */
lcl_hmap_iter_t lcl_hmap_end( lcl_hmap_t* hmap );

/**
 * @brief Check if two iterators are equal
 * 
 * @param a iterator
 * @param b iterator
 * @return true they are equal in index and map
 * @return false they are not equal
 */
bool lcl_hmap_iter_eq( lcl_hmap_iter_t a, lcl_hmap_iter_t b );

/**
 * @brief Advance iterator to the next index
 * 
 * @param i iterator
 * @return lcl_hmap_iter_t advanced iterator 
 */
lcl_hmap_iter_t lcl_hmap_iter_next( lcl_hmap_iter_t i );

/**
 * @brief Step back an iterator to the previous index
 * 
 * @param i iterator
 * @return lcl_hmap_iter_t previous index iterator 
 */
lcl_hmap_iter_t lcl_hmap_iter_prev( lcl_hmap_iter_t i );

/**
 * @brief Get the key pointed to by an iterator
 * 
 * @param i iterator
 * @return lcl_any_t key pointed to 
 */
lcl_any_t lcl_hmap_iter_key( lcl_hmap_iter_t i );

/**
 * @brief Get the value pointed to by an iterator
 * 
 * @param i iterator
 * @return lcl_any_t value pointed to 
 */
lcl_any_t lcl_hmap_iter_value( lcl_hmap_iter_t i );

/**
 * @brief Iterate through everything in a hash map
 *  
 *
 * Use in-place in a for loop structure, for example,
 * 
 * for lcl_hmap_each(it, my_hash_map) {
 *      lcl_any_t key = lcl_hmap_iter_key(it);
 *      lcl_any_t value = lcl_hmap_iter_value(it);
 *      printf("Pair: { %s: %s }\n", key, value);
 * }
 * 
 * 
 */
#define lcl_hmap_each(it, hmap)   ( lcl_hmap_iter_t it = lcl_hmap_begin(hmap);\
                                    !lcl_hmap_iter_eq(it, lcl_hmap_end(hmap));\
                                    it = lcl_hmap_iter_next(it) )

/**
 * @brief Display a hash map containing printf-able types
 * 
 * @param hmap hmap to display
 * @param keyfmt printf format specifier for keys
 * @param valuefmt printf format specifier for values
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_hmap_display( lcl_hmap_t* hmap, const char* keyfmt, const char* valuefmt );


/**
 * @brief Clone a hashmap
 * 
 * @param src hmap to clone
 * @return lcl_hmap_t* cloned hashmap (NULL if not enough memory)
 */
lcl_hmap_t* lcl_hmap_clone(lcl_hmap_t* src);


/**
 * @brief Free a hash map
 * 
 * @warning if different types that require different deallocators
 * are used, these must be freed first by removing these elements
 * from the hashmap using hmap_remove, and calling the respective
 * deallocator on the retrieved keys or values. Otherwise memory
 * leaks will occur.
 * 
 * 
 * @param hmap hashmap pointer to free
 * @param kdealloc deallocator function for keys (see lcl_allocators)
 * @param vdealloc deallocator function for values (see lcl_allocators)
 * @return lcl_err_t 
 */
lcl_err_t lcl_hmap_free( lcl_hmap_t** hmap, lcl_deallocfn_t kdealloc, lcl_deallocfn_t vdealloc );

#endif