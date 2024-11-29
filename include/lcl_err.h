
#ifndef LCL_ERR_H
#define LCL_ERR_H

/**
 * @brief Errors returned by LCL functions
 */
typedef enum lcl_err {
    LCL_OK,
    LCL_FAIL,
    LCL_BAD_ARGUMENT,
    LCL_INDEX_OUT_OF_BOUNDS,
    LCL_NO_MEMORY,
    LCL_COLLECTION_EMPTY,
    LCL_NOT_FOUND,
} lcl_err_t;

const char* lcl_strerror(lcl_err_t err);





#endif