#include "units.h"
#include "unity.h"
#include "lcl_hmap.h"


static void hmap_int_str() {

    lcl_hmap_t* hmap;
    TEST_LCL_OK(lcl_hmap_init(&hmap, lcl_hash_any, lcl_ipeq));


    TEST_LCL_OK(lcl_hmap_insert( hmap, (lcl_any_t) 5, "Five", NULL, NULL ));
    TEST_LCL_OK(lcl_hmap_insert( hmap, (lcl_any_t) 6, "Six", NULL, NULL ));
    TEST_LCL_OK(lcl_hmap_insert( hmap, (lcl_any_t) 7, "Seven", NULL, NULL ));
    TEST_LCL_OK(lcl_hmap_insert( hmap, (lcl_any_t) 3, "Three", NULL, NULL ));
    TEST_LCL_OK(lcl_hmap_insert( hmap, (lcl_any_t) 2, "huh?", NULL, NULL ));
    TEST_LCL_OK(lcl_hmap_insert( hmap, (lcl_any_t) 2, "Two", NULL, NULL ));

    const char* getter;
    TEST_LCL_OK(lcl_hmap_get(hmap, (lcl_any_t)5, (lcl_any_t*) &getter));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( "Five", getter, "Could not get [5]" );
    TEST_LCL_OK(lcl_hmap_get(hmap, (lcl_any_t)6, (lcl_any_t*) &getter));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( "Six", getter, "Could not get [6]" );
    TEST_LCL_OK(lcl_hmap_get(hmap, (lcl_any_t)7, (lcl_any_t*) &getter));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( "Seven", getter, "Could not get [7]" );
    TEST_LCL_OK(lcl_hmap_get(hmap, (lcl_any_t)3, (lcl_any_t*) &getter));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( "Three", getter, "Could not get [3]" );
    TEST_LCL_OK(lcl_hmap_get(hmap, (lcl_any_t)2, (lcl_any_t*) &getter));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( "Two", getter, "Could not get [2]" );


    TEST_LCL_OK(lcl_hmap_free(&hmap, NULL, NULL));

}


static void hmap_str_int() {

    lcl_hmap_t* hmap;
    TEST_LCL_OK(lcl_hmap_init(&hmap, lcl_hash_djb2, lcl_streq));

    TEST_LCL_OK(lcl_hmap_insert( hmap, "Five", (lcl_any_t) 5, NULL, NULL ));
    TEST_LCL_OK(lcl_hmap_insert( hmap, "Six", (lcl_any_t) 6, NULL, NULL ));
    TEST_LCL_OK(lcl_hmap_insert( hmap, "Seven", (lcl_any_t) 7, NULL, NULL ));
    TEST_LCL_OK(lcl_hmap_insert( hmap, "Twelve", (lcl_any_t) 12, NULL, NULL ));
    TEST_LCL_OK(lcl_hmap_insert( hmap, "Three", (lcl_any_t) 3, NULL, NULL ));
    TEST_LCL_OK(lcl_hmap_insert( hmap, "One", (lcl_any_t) 1, NULL, NULL ));


    lcl_any_t getter;
    TEST_LCL_OK(lcl_hmap_get(hmap, "Five",  &getter));
    TEST_ASSERT_MESSAGE( ((size_t) getter) == 5, "Could not get [Five]" );
    TEST_LCL_OK(lcl_hmap_get(hmap, "Six", &getter));
    TEST_ASSERT_MESSAGE( ((size_t) getter) == 6, "Could not get [Six]" );
    TEST_LCL_OK(lcl_hmap_get(hmap, "Seven", &getter));
    TEST_ASSERT_MESSAGE( ((size_t) getter) == 7, "Could not get [Seven]" );
    TEST_LCL_OK(lcl_hmap_get(hmap, "Twelve", &getter));
    TEST_ASSERT_MESSAGE( ((size_t) getter) == 12, "Could not get [Twelve]" );
    TEST_LCL_OK(lcl_hmap_get(hmap, "Three", &getter));
    TEST_ASSERT_MESSAGE( ((size_t) getter) == 3, "Could not get [Three]" );
    TEST_LCL_OK(lcl_hmap_get(hmap, "One", &getter));
    TEST_ASSERT_MESSAGE( ((size_t) getter) == 1, "Could not get [One]" );

    TEST_LCL_OK(lcl_hmap_free(&hmap, NULL, NULL));

}


static void hmap_malicious() {
    lcl_hmap_t* hmap;
    TEST_LCL_OK(lcl_hmap_init(&hmap, lcl_hash_any, lcl_ipeq));


    TEST_LCL_OK(lcl_hmap_insert( hmap, (lcl_any_t) 1, "One", NULL, NULL ));
    TEST_LCL_OK(lcl_hmap_insert( hmap, (lcl_any_t) 17, "Seventeen", NULL, NULL ));
    TEST_LCL_OK(lcl_hmap_insert( hmap, (lcl_any_t) 3, "Three", NULL, NULL ));
    TEST_LCL_OK(lcl_hmap_insert( hmap, (lcl_any_t) 19, "Nineteen", NULL, NULL ));
    TEST_LCL_OK(lcl_hmap_insert( hmap, (lcl_any_t) 18, "Eighteen", NULL, NULL ));
    TEST_LCL_OK(lcl_hmap_insert( hmap, (lcl_any_t) 2, "Two", NULL, NULL ));

    const char* getter;
    TEST_LCL_OK(lcl_hmap_get(hmap, (lcl_any_t)1, (lcl_any_t*) &getter));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( "One", getter, "Could not get [5]" );
    TEST_LCL_OK(lcl_hmap_get(hmap, (lcl_any_t)17, (lcl_any_t*) &getter));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( "Seventeen", getter, "Could not get [6]" );
    TEST_LCL_OK(lcl_hmap_get(hmap, (lcl_any_t)3, (lcl_any_t*) &getter));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( "Three", getter, "Could not get [7]" );
    TEST_LCL_OK(lcl_hmap_get(hmap, (lcl_any_t)19, (lcl_any_t*) &getter));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( "Nineteen", getter, "Could not get [3]" );
    TEST_LCL_OK(lcl_hmap_get(hmap, (lcl_any_t)2, (lcl_any_t*) &getter));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( "Two", getter, "Could not get [2]" );
    TEST_LCL_OK(lcl_hmap_get(hmap, (lcl_any_t)18, (lcl_any_t*) &getter));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( "Eighteen", getter, "Could not get [2]" );


    TEST_LCL_OK(lcl_hmap_free(&hmap, NULL, NULL));
}


static void hmap_growth() {
    
}


void unit_hmap_main() {

    RUN_TEST(hmap_int_str);
    RUN_TEST(hmap_str_int);
    RUN_TEST(hmap_malicious);

}