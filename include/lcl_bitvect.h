#ifndef LCL_BITVECT_H
#define LCL_BITVECT_H

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t* lcl_bitvect_t;

#include "lcl_vect.h"

/**
 * @brief Extend a vector (bitvector) with a certain byte value repeated
 * 
 * @param bvec bit vect
 * @param value value to repeat
 * @param count number of bytes to add
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_bitvect_extend_with( lcl_bitvect_t* bvec, uint8_t value, size_t count );

/**
 * @brief Find the first bit of a certain value
 * 
 * @param bvec bit vect to search
 * @param bit bit to find
 * @param start index to start search from
 * @param bitcount output written here, bit index
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_bitvect_find_first( const lcl_bitvect_t bvec, bool bit, size_t start, size_t* bitcount );
lcl_err_t lcl_bitvect_find_last( const lcl_bitvect_t bvec, bool bit, size_t* bitcount );

/**
 * @brief Assign a bit value
 * 
 * @param bvec bitvector
 * @param index bit index
 * @param bit bit value
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_bitvect_assign( const lcl_bitvect_t bvec, size_t index, bool bit );

/**
 * @brief Toggle a bit value
 * 
 * @param bvec bitvector
 * @param index bit index
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_bitvect_toggle( const lcl_bitvect_t bvec, size_t index );

/**
 * @brief Get a bit value
 * 
 * @param bvec bitvector
 * @param index bit index
 * @param bit output written here
 * @return lcl_err_t any errors
 */
lcl_err_t lcl_bitvect_getbit( const lcl_bitvect_t bvec, size_t index, bool* bit );



#endif