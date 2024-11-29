#include "lcl_err.h"


static const char* lcl_strerrors[] = {
    [LCL_OK] = "Success",
    [LCL_FAIL] = "Fail",
    [LCL_BAD_ARGUMENT] = "Bad Argument",
    [LCL_INDEX_OUT_OF_BOUNDS] = "Index Out of Bounds",
    [LCL_NO_MEMORY] = "No Memory",
    [LCL_COLLECTION_EMPTY] = "Collection Empty"
};



const char *lcl_strerror(lcl_err_t err)
{
    
    if ((unsigned)err < (sizeof(lcl_strerrors) / sizeof(lcl_strerrors[0]))) {
        return lcl_strerrors[err];
    } else return "Unkown LCL Error";
}