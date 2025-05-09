#include "lcl_string.h"

#include <assert.h>


#include "private/lcl_util.h"


lcl_err_t lcl_str_from( lcl_str_t* sptr, const char* other ) {
    size_t len = strlen(other);
    return lcl_str_froms(sptr, other, len + 1);
}

lcl_err_t lcl_str_join(lcl_str_t *sptr, const char *delim, char **substrs, size_t count)
{
    if (!sptr || !delim || !substrs) return LCL_BAD_ARGUMENT;

    if (!*sptr) LCL_ERRPASS(lcl_str_init(sptr, count * 2));

    for (size_t i = 0; i < count; i ++) {
        if (i) LCL_ERRPASS(lcl_str_extend(sptr, delim));
        LCL_ERRPASS(lcl_str_extend(sptr, substrs[i]));
    }

    return LCL_OK;

}

lcl_err_t lcl_str_extend(lcl_str_t *sptr, const char *other)
{
    size_t len = lcl_str_len(*sptr);
    if (len) LCL_ERRPASS(lcl_vect_pop( sptr, NULL ));
    LCL_ERRPASS(lcl_vect_extend( sptr, other, strlen(other)+1 ));
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
    assert( findlen );
    char starter = *find;

    // Determine most efficient way to overwrite 
    // to avoid uneccessary splicing and inserting

    size_t overwrite_count = 0;
    size_t insert_len = 0;
    size_t remove_len = 0;
    const char* insert_source = NULL;

    // for equal lengths, just overwrite
    if (findlen == replace_len) {
        overwrite_count = findlen;
    } else if (findlen < replace_len) {

        // for larger replacers, overwrite what you can
        // and insert the rest after
        overwrite_count = findlen;
        insert_len = replace_len - findlen;
        insert_source = &replace[overwrite_count];
    } else {

        // for smaller replacers, overwrite the whole
        // replacer, and splice the rest
        overwrite_count = replace_len;
        remove_len = findlen - replace_len;
    }



    size_t i = start;
    do {
        if ((*sptr)[i] == starter) {
            size_t remaining = lcl_str_len(*sptr) - i - 1;


            if (remaining < findlen) return LCL_OK;

            // if found
            if (strncmp( &(*sptr)[i], find, findlen ) == 0) {
                
                // Overwrite
                memcpy( &(*sptr)[i], replace, overwrite_count );

                // insert or splice as necessary
                if (insert_len) {
                    lcl_str_insertns( sptr, i + overwrite_count, insert_source, insert_len );
                } else if (remove_len) {
                    lcl_str_splice( sptr, i + overwrite_count, NULL, remove_len );
                }

                // move to the end of the modification
                i += replace_len - 1;
            }



        }
        i++;
    } while (--count && i < lcl_str_len(*sptr));


    return LCL_OK;
}





lcl_err_t lcl_str_split(const lcl_str_t sptr, char* delim, lcl_str_t* svect[])
{
    if (!sptr || !svect || !delim) return LCL_BAD_ARGUMENT;

    size_t delimlen = strlen(delim);
    LCL_ERRPASS(lcl_vect_init(svect, 8));

    const char* iter = sptr;
    const char* next;
    while ((next = strstr(iter, delim)) != NULL) {
        lcl_str_t newstr;
        LCL_ERRPASS(lcl_str_froms( &newstr, iter, next-iter ));
        iter = next + delimlen;
        LCL_ERRPASS(lcl_vect_push(svect, newstr));
    }

    lcl_str_t newstr;
    LCL_ERRPASS(lcl_str_from( &newstr, iter ));
    LCL_ERRPASS(lcl_vect_push(svect, newstr));

    return LCL_OK;

}

