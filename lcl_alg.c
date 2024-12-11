#include "lcl_alg.h"

#include <string.h>

bool lcl_ipeq(const lcl_any_t a, const lcl_any_t b)
{
    return a == b;
}

bool lcl_streq(const lcl_any_t a, const lcl_any_t b)
{
    return strcmp(a,b) == 0;
}
