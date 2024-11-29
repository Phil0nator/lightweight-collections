#include "lcl_hash.h"

#include <assert.h>
#include <stddef.h>

lcl_hash_t lcl_hash_pass_ptr(const void* ptr) {
    return (lcl_hash_t)( (size_t) ptr);
}

lcl_hash_t lcl_hash_djb2(const void *ptr)
{
    const char* s = ptr;
    uint32_t hash = 5381; // Initialize hash to a magic number
    int c;
    while ((c = *s++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

lcl_hash_t lcl_hash_FNV1a(const void *ptr)
{
    const char* s = ptr;
    uint32_t hash = 2166136261U; // FNV offset basis
    while (*s) {
        hash ^= (unsigned char)(*s++);
        hash *= 16777619U; // FNV prime
    }
    return hash;
}

lcl_hash_t lcl_hash_murmur3(const void *ptr)
{
    // TODO
    assert(0);
    return (lcl_hash_t) -1;
}
