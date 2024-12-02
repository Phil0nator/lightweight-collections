#ifndef LCL_UNITS_H
#define LCL_UNITS_H

#define TEST_LCL_OK( err ) TEST_ASSERT_MESSAGE( (err) == LCL_OK, "Expected LCL_OK" )


void unit_vect_main();
void unit_str_main();
void unit_list_main();
void unit_mpool_main();
void unit_hmap_main();
#endif