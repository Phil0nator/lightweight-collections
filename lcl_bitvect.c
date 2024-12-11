#include "lcl_bitvect.h"
#include <memory.h>

#include "private/lcl_util.h"

lcl_err_t lcl_bitvect_extend_with(lcl_bitvect_t *bvec, uint8_t value, size_t count)
{
    if (!bvec || !*bvec) return LCL_BAD_ARGUMENT;


    size_t len = lcl_vect_len(*bvec);
    size_t newlen = len + count;
    LCL_ERRPASS(lcl_vect_set_len( bvec, newlen ));
    memset( &(*bvec)[len], value, count );

    return LCL_OK;

}

lcl_err_t lcl_bitvect_find_first(const lcl_bitvect_t bvec, bool bit, size_t start, size_t* bitcount)
{
    if (!bvec || !bitcount) return LCL_BAD_ARGUMENT;
    size_t len = lcl_vect_len(bvec);
    size_t i;
                
    for (i = start; i < len; i++) {
        if (bit && bvec[i]) break;
        if (!bit && bvec[i] != 0xff) break;
    }

    if (i < len) {
        uint8_t byte = bvec[i];
        size_t bitn = i * 8;
        for (size_t andmask = 1; andmask; andmask <<= 1, bitn++) {
            if ((!!(byte & andmask)) == bit){
                *bitcount = bitn;
                return LCL_OK;
            }
        }
    } 
    return LCL_NOT_FOUND;
}


lcl_err_t lcl_bitvect_assign(const lcl_bitvect_t bvec, size_t index, bool bit)
{
    if (!bvec) return LCL_BAD_ARGUMENT;
    size_t byte = index / 8;
    size_t bitn = index % 8;
    bit ? (bvec[byte] |= (1 << bitn) ) : (bvec[byte] &= ~(1 << bitn));
    return LCL_OK;
}

lcl_err_t lcl_bitvect_toggle(const lcl_bitvect_t bvec, size_t index)
{
    if (!bvec) return LCL_BAD_ARGUMENT;
    size_t byte = index / 8;
    size_t bitn = index % 8;
    bvec[byte] ^= (1 << bitn); 
    return LCL_OK;
}

lcl_err_t lcl_bitvect_getbit(const lcl_bitvect_t bvec, size_t index, bool *bit)
{
    if (!bvec) return LCL_BAD_ARGUMENT;
    size_t byte = index / 8;
    size_t bitn = index % 8;
    *bit = (bvec[byte] & (1 << bitn)) ? true : false; 
    return LCL_OK;
}
