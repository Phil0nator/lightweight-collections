#ifndef LCL_UNITS_H
#define LCL_UNITS_H
#include "unity.h"
#include "lcl_err.h"

#define TEST_LCL_OK( err )  do { lcl_err_t __err = err; if (__err != LCL_OK) { printf("LCL ERROR(%d): %s\n", __err, lcl_strerror(__err)); TEST_FAIL(); }} while (0)


void unit_vect_main();
void unit_str_main();
void unit_list_main();
void unit_mpool_main();
void unit_hmap_main();
void unit_cqueue_main();

#endif