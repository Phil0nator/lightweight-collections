#ifndef LCL_HASH_H
#define LCL_HASH_H

#include <stdint.h>
#include "lcl_any.h"

typedef uint32_t lcl_hash_t;

typedef lcl_hash_t (*lcl_hashfn_t)(const lcl_any_t);

/**
 * @brief hash lcl_any as an in-place type (any non-pointer type)
 * 
 * @param ptr any value
 * @return lcl_hash_t hashed value
 */
lcl_hash_t lcl_hash_any(const lcl_any_t ptr);

/**
 * @brief djb2 hash algorithm
 * 
 * @param ptr cstring
 * @return lcl_hash_t hash 
 */
lcl_hash_t lcl_hash_djb2(const lcl_any_t ptr);

/**
 * @brief FNV1a hash algorithm
 * 
 * @param ptr cstring
 * @return lcl_hash_t hash 
 */
lcl_hash_t lcl_hash_FNV1a(const lcl_any_t ptr);

/**
 * @brief murmur3 hash algorithm
 * 
 * @param ptr cstring
 * @return lcl_hash_t hash 
 */
lcl_hash_t lcl_hash_murmur3(const lcl_any_t ptr);


#endif