#include "lcl_hash.h"
#include "lcl_any.h"
#include <assert.h>
#include <stddef.h>

lcl_hash_t lcl_hash_any(const lcl_any_t ptr) {
    return (lcl_hash_t)( lcl_any2u32( (lcl_any_t) ptr) );
}

lcl_hash_t lcl_hash_djb2(const lcl_any_t ptr)
{
    const char* s = ptr;
    uint32_t hash = 5381; // Initialize hash to a magic number
    int c;
    while ((c = *s++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

lcl_hash_t lcl_hash_FNV1a(const lcl_any_t ptr)
{
    const char* s = ptr;
    uint32_t hash = 2166136261U; // FNV offset basis
    while (*s) {
        hash ^= (unsigned char)(*s++);
        hash *= 16777619U; // FNV prime
    }
    return hash;
}

lcl_hash_t lcl_hash_murmur3(const lcl_any_t ptr)
{
    // TODO
    assert(0);
    return (lcl_hash_t) -1;
}
