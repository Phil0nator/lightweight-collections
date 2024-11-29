#ifndef LCL_HASH_H
#define LCL_HASH_H

#include <stdint.h>

typedef uint32_t lcl_hash_t;

typedef lcl_hash_t (*lcl_hashfn_t)(const void*);

lcl_hash_t lcl_hash_pass_ptr(const void* ptr);
lcl_hash_t lcl_hash_djb2(const void* ptr);
lcl_hash_t lcl_hash_FNV1a(const void* ptr);
lcl_hash_t lcl_hash_murmur3(const void* ptr);


#endif