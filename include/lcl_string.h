#ifndef LCL_STRING_H
#define LCL_STRING_H

#include <string.h>

#include "lcl_vect.h"


typedef char* lcl_str_t;

/**
 * @brief Initialize a string
 * 
 * @param sptr pointer to set string
 * @param capacity initial capacity (including null terminator)
 * @return lcl_err_t any errors
 */
static inline lcl_err_t lcl_str_init(lcl_str_t* sptr, size_t capacity) 
    { return lcl_vect_init( sptr, capacity ); } 

/**
 * @brief Create a string from an existing string
 * 
 * @param sptr string to create
 * @param other string to copy from
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_str_from( lcl_str_t* sptr, const char* other );

/**
 * @brief Create a string from an existing string
 * 
 * @param sptr string to create
 * @param other string to copy from
 * @param len length of original string
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_str_froms( lcl_str_t* sptr, const char* other, size_t len );

/**
 * @brief Get string length
 * 
 * @param sptr string to get length of
 * @returns size_t string length
 */
static inline size_t lcl_str_len( lcl_str_t sptr ) 
    { return lcl_vect_len( sptr ); }
/**
 * @brief Get string capacity
 * 
 * @param sptr string to get cap
 * @return size_t string capacity
 */
static inline size_t lcl_str_cap( lcl_str_t sptr ) 
    { return lcl_vect_cap( sptr ); }

/**
 * @brief Get a pointer to the last char of the string
 * 
 * @param sptr string
 * @return char* last char of string
 */
static inline char* lcl_str_last( lcl_str_t sptr ) 
    { return (char*) lcl_vect_last( sptr ) - 1; }

/**
 * @brief Get a pointer to the null terminator of the string
 * 
 * @param sptr string
 * @return char* pointer to the null terminator
 */
static inline char* lcl_str_end( lcl_str_t sptr ) 
    { return (char*) lcl_vect_end( sptr ) - 1; }


/**
 * @brief Concatenate a string to the end of sptr
 * 
 * @param sptr string to modify
 * @param other other string to concatenate
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_str_extend( lcl_str_t* sptr, const char* other );
/**
 * @brief Concatenate a single char
 * 
 * @param sptr string to modify
 * @param other char to add
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_str_push(lcl_str_t* sptr, char other);

/**
 * @brief Truncate chars off the end of a string
 * 
 * @param sptr string to modify
 * @param dest destination to copy truncated chars to
 *  (May be NULL)
 * @param count number of chars to truncate
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_str_truncate( lcl_str_t* sptr, char* dest, size_t count );

/**
 * @brief Pop a single char off the end of a string
 * @see lcl_str_truncate
 */
#define lcl_str_pop(sptr, dest) lcl_str_truncate( sptr, dest, 1 )


/**
 * @brief Insert a string into the middle of an existing string
 * 
 * @param sptr string to modify
 * @param index index to insert at
 * @param data string to insert
 * @param items number of characters to insert
 * @return lcl_err_t any errors
 */
static inline lcl_err_t lcl_str_inserts( lcl_str_t* sptr, size_t index, const char* data, size_t items )
    { return lcl_vect_inserts( sptr, index, data, items ); }

/**
 * @brief Insert a single char into a string
 * @see lcl_str_inserts
 */
#define lcl_str_insert(sptr, index, data) lcl_str_inserts( sptr, index, data, 1 )

/**
 * @brief remove a string from the middle of a string
 * 
 * @param sptr string to modify
 * @param index index to remove from (inclusive)
 * @param data destination buffer to write removed string
 *  (May be NULL)
 * @param count number of chars to remove
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_str_splice( lcl_str_t* sptr, size_t index, char* data, size_t count );

/**
 * @brief Remove a character
 * @see lcl_str_splice
 */
#define lcl_str_remove(sptr, index, data) lcl_str_splice(sptr, index, data, 1)

/**
 * @brief Swap two indexes of a string
 */
#define lcl_str_swap(sptr, a, b) do { char __lcl_strswap = sptr[a]; sptr[a] = sptr[b]; sptr[b] = __lcl_strswap; } while (0) 

/**
 * @brief Clean unused capacity of a string
 * 
 * @param sptr string to clean
 * @return lcl_err_t any errors
 */
static inline lcl_err_t lcl_str_clean( lcl_str_t* sptr ) 
    { return lcl_vect_clean( sptr ); }

/**
 * @brief see lcl_vect_set_len
 * 
 * @see lcl_vect_set_len
 */
static inline lcl_err_t lcl_str_set_len( lcl_str_t* sptr, size_t len ) 
    { return lcl_vect_set_len( sptr, len ); }

/**
 * @brief free a string
 * 
 * @param sptr string to free
 * @return lcl_err_t any errors
 */
static inline lcl_err_t lcl_str_free( lcl_str_t* sptr ) 
    { return lcl_vect_free(sptr); }


#define lcl_str_debug lcl_vect_debug

#endif