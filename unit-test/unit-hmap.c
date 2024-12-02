#include "units.h"
#include "unity.h"
#include "lcl_hmap.h"


static void hmap_int_str() {

    lcl_hmap_t* hmap;
    TEST_LCL_OK(lcl_hmap_init(&hmap, lcl_hash_pass_ptr, LCL_DEALLOC_NONE));


    TEST_LCL_OK(lcl_hmap_insert( hmap, (void*) 5, "Five", NULL ));
    TEST_LCL_OK(lcl_hmap_insert( hmap, (void*) 6, "Six", NULL ));
    TEST_LCL_OK(lcl_hmap_insert( hmap, (void*) 7, "Seven", NULL ));
    TEST_LCL_OK(lcl_hmap_insert( hmap, (void*) 3, "Three", NULL ));
    TEST_LCL_OK(lcl_hmap_insert( hmap, (void*) 2, "huh?", NULL ));
    TEST_LCL_OK(lcl_hmap_insert( hmap, (void*) 2, "Two", NULL ));

    const char* getter;
    TEST_LCL_OK(lcl_hmap_get(hmap, (void*)5, (void**) &getter));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( "Five", getter, "Could not get [5]" );
    TEST_LCL_OK(lcl_hmap_get(hmap, (void*)6, (void**) &getter));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( "Six", getter, "Could not get [6]" );
    TEST_LCL_OK(lcl_hmap_get(hmap, (void*)7, (void**) &getter));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( "Seven", getter, "Could not get [7]" );
    TEST_LCL_OK(lcl_hmap_get(hmap, (void*)3, (void**) &getter));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( "Three", getter, "Could not get [3]" );
    TEST_LCL_OK(lcl_hmap_get(hmap, (void*)2, (void**) &getter));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( "Two", getter, "Could not get [2]" );


    lcl_hmap_free(&hmap, NULL);

}


static void hmap_str_int() {

    lcl_hmap_t* hmap;
    TEST_LCL_OK(lcl_hmap_init(&hmap, lcl_hash_djb2, LCL_DEALLOC_NONE));

    TEST_LCL_OK(lcl_hmap_insert( hmap, "Five", (void*) 5, NULL ));
    TEST_LCL_OK(lcl_hmap_insert( hmap, "Six", (void*) 6, NULL ));
    TEST_LCL_OK(lcl_hmap_insert( hmap, "Seven", (void*) 7, NULL ));
    TEST_LCL_OK(lcl_hmap_insert( hmap, "Twelve", (void*) 12, NULL ));
    TEST_LCL_OK(lcl_hmap_insert( hmap, "Three", (void*) 3, NULL ));
    TEST_LCL_OK(lcl_hmap_insert( hmap, "One", (void*) 1, NULL ));


    void* getter;
    TEST_LCL_OK(lcl_hmap_get(hmap, "Five", (void**) &getter));
    TEST_ASSERT_MESSAGE( ((size_t) getter) == 5, "Could not get [Five]" );
    TEST_LCL_OK(lcl_hmap_get(hmap, "Six", (void**) &getter));
    TEST_ASSERT_MESSAGE( ((size_t) getter) == 6, "Could not get [Six]" );
    TEST_LCL_OK(lcl_hmap_get(hmap, "Seven", (void**) &getter));
    TEST_ASSERT_MESSAGE( ((size_t) getter) == 7, "Could not get [Seven]" );
    TEST_LCL_OK(lcl_hmap_get(hmap, "Twelve", (void**) &getter));
    TEST_ASSERT_MESSAGE( ((size_t) getter) == 12, "Could not get [Twelve]" );
    TEST_LCL_OK(lcl_hmap_get(hmap, "Three", (void**) &getter));
    TEST_ASSERT_MESSAGE( ((size_t) getter) == 3, "Could not get [Three]" );
    TEST_LCL_OK(lcl_hmap_get(hmap, "One", (void**) &getter));
    TEST_ASSERT_MESSAGE( ((size_t) getter) == 1, "Could not get [One]" );

    lcl_hmap_free(&hmap, NULL);

}

void unit_hmap_main() {

    RUN_TEST(hmap_int_str);
    RUN_TEST(hmap_str_int);

}