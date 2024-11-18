#include "lcl_string.h"

#include <assert.h>


#include "private/lcl_util.h"


lcl_err_t lcl_str_from( lcl_str_t* sptr, const char* other ) {
    size_t len = strlen(other);
    return lcl_str_froms(sptr, other, len + 1);
}

lcl_err_t lcl_str_extend(lcl_str_t *sptr, const char *other)
{
    size_t len = lcl_str_len(*sptr);
    size_t otherlen = strlen(other);
    LCL_ERRPASS(lcl_vect_set_len( sptr, len + otherlen ));
    char* copyptr = &((*sptr)[len -1 ]);
    strcpy( copyptr, other );
    return LCL_OK;
}

lcl_err_t lcl_str_push(lcl_str_t *sptr, char other)
{
    size_t len = lcl_str_len(*sptr);
    LCL_ERRPASS(lcl_vect_set_len( sptr, len+1 ));
    (*sptr)[len-1] = other;
    (*sptr)[len] = '\0';
    return LCL_OK;
}

lcl_err_t lcl_str_truncate(lcl_str_t *sptr, char *dest, size_t count)
{
    size_t len = lcl_str_len(*sptr);
    LCL_ERRPASS(lcl_vect_splice( sptr, len-count-1, dest, count ));
    return LCL_OK;
}

lcl_err_t lcl_str_splice(lcl_str_t *sptr, size_t index, char *data, size_t count)
{
    LCL_ERRPASS(lcl_vect_splice( sptr, index, data, count ));
    size_t len = lcl_str_len(*sptr);
    (*sptr)[len-1] = '\0';
    return LCL_OK;
}


lcl_err_t lcl_str_replacesnsn(lcl_str_t *sptr, size_t start, size_t count, const char *find, size_t findlen, const char *replace, size_t replace_len)
{
    assert( findlen && replace_len );
    char* siter  = *sptr;
    char starter = *find;
    do {
        if (*siter == starter) {
            // TODO
        }
        siter ++;
    } while (--count);


    return LCL_OK;
}
