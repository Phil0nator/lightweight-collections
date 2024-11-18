#include "lcl_vect.h"
#include "unity.h"


#define TEST_LCL_OK( err ) TEST_ASSERT_MESSAGE( (err) == LCL_OK, "Expected LCL_OK" )

static void vect_init() {

    int *my_vec;
    TEST_LCL_OK( lcl_vect_init(&my_vec, 5) );

    TEST_ASSERT_MESSAGE( lcl_vect_len( my_vec ) == 0, "Initial length not 0" );
    TEST_ASSERT_MESSAGE( lcl_vect_cap( my_vec ) == 5, "Initial capacity not set" );
    TEST_ASSERT_MESSAGE( lcl_vect_isize( my_vec ) == sizeof(int), "isize not set" );
    TEST_ASSERT_MESSAGE( lcl_vect_unused( my_vec ) == 5, "Incorrect vect_unused" );

    TEST_LCL_OK(lcl_vect_free(&my_vec));

}


static void vect_push_ext() {

    int *my_vec;
    TEST_LCL_OK(lcl_vect_init(&my_vec, 5));

    int value = 12;
    TEST_LCL_OK(lcl_vect_push(&my_vec, &value));

    TEST_ASSERT_MESSAGE( my_vec[0] == value, "value pushed not found at vec[0]" );
    TEST_ASSERT_MESSAGE( lcl_vect_len(my_vec) == 1, "vect length not incremented after push" );
    TEST_ASSERT_MESSAGE( *(int*)lcl_vect_last(my_vec) == value, "*vect_last() incorrect" );
    TEST_ASSERT_MESSAGE( lcl_vect_last(my_vec) == &my_vec[0], "vect_last() incorrect" );
    TEST_ASSERT_MESSAGE( lcl_vect_end(my_vec) == &my_vec[1], "vect_end() incorrect" );
    TEST_ASSERT_MESSAGE( lcl_vect_cap(my_vec) == 5, "vect_cap() incorrect" );
    TEST_ASSERT_MESSAGE( lcl_vect_unused(my_vec) == 4, "vect_unused() incorrect" );

    value = 15;
    TEST_LCL_OK(lcl_vect_push(&my_vec, &value));
    
    TEST_ASSERT_MESSAGE( my_vec[1] == value, "value pushed not found at vec[1]" );
    TEST_ASSERT_MESSAGE( lcl_vect_len(my_vec) == 2, "vect length not incremented after push" );
    TEST_ASSERT_MESSAGE( *(int*)lcl_vect_last(my_vec) == value, "*vect_last() incorrect" );
    TEST_ASSERT_MESSAGE( lcl_vect_last(my_vec) == &my_vec[1], "vect_last() incorrect" );
    TEST_ASSERT_MESSAGE( lcl_vect_end(my_vec) == &my_vec[2], "vect_end() incorrect" );
    TEST_ASSERT_MESSAGE( lcl_vect_cap(my_vec) == 5, "vect_cap() incorrect" );
    TEST_ASSERT_MESSAGE( lcl_vect_unused(my_vec) == 3, "vect_unused() incorrect" );

    int values[] = {5,6,7,8};
    TEST_LCL_OK(lcl_vect_extend(&my_vec, &values, 4));

    TEST_ASSERT_MESSAGE( my_vec[2] == 5, "value pushed not found at vec[2]" );
    TEST_ASSERT_MESSAGE( my_vec[3] == 6, "value pushed not found at vec[3]" );
    TEST_ASSERT_MESSAGE( my_vec[4] == 7, "value pushed not found at vec[4]" );
    TEST_ASSERT_MESSAGE( my_vec[5] == 8, "value pushed not found at vec[5]" );

    TEST_ASSERT_MESSAGE( lcl_vect_len(my_vec) == 6, "vect length not incremented after push" );
    TEST_ASSERT_MESSAGE( *(int*)lcl_vect_last(my_vec) == values[3], "*vect_last() incorrect" );
    TEST_ASSERT_MESSAGE( lcl_vect_last(my_vec) == &my_vec[5], "vect_last() incorrect" );
    TEST_ASSERT_MESSAGE( lcl_vect_end(my_vec) == &my_vec[6], "vect_end() incorrect" );
    TEST_ASSERT_MESSAGE( lcl_vect_cap(my_vec) > 5, "vect_cap() incorrect" );


    TEST_LCL_OK(lcl_vect_free(&my_vec));

}


static void vect_trunc() {

    int* vec;
    int values[] = {1,2,3,4,5,6,7,8,9,10};
    TEST_LCL_OK(lcl_vect_from( &vec, &values, 10 ));

    TEST_ASSERT_MESSAGE( lcl_vect_len(vec) == 10, "vect_len() incorrect" );
    TEST_ASSERT_MESSAGE( lcl_vect_cap(vec) == 10, "vect_cap() incorrect" );
    TEST_ASSERT_MESSAGE( lcl_vect_unused(vec) == 0, "vect_unused() incorrect" );

    TEST_LCL_OK(lcl_vect_pop( &vec, NULL ));

    TEST_ASSERT_MESSAGE( lcl_vect_len(vec) == 9, "vect_len() incorrect" );
    TEST_ASSERT_MESSAGE( lcl_vect_cap(vec) == 10, "vect_cap() incorrect" );
    TEST_ASSERT_MESSAGE( lcl_vect_unused(vec) == 1, "vect_unused() incorrect" );

    int removed[5];
    TEST_LCL_OK(lcl_vect_truncate( &vec, &removed, 5 ));

    TEST_ASSERT_MESSAGE( lcl_vect_len(vec) == 4, "vect_len() incorrect" );
    TEST_ASSERT_MESSAGE( lcl_vect_cap(vec) == 10, "vect_cap() incorrect" );
    TEST_ASSERT_MESSAGE( lcl_vect_unused(vec) == 6, "vect_unused() incorrect" );

    TEST_ASSERT_MESSAGE( removed[0] == 5, "removed[0] incorrect");
    TEST_ASSERT_MESSAGE( removed[1] == 6, "removed[1] incorrect");
    TEST_ASSERT_MESSAGE( removed[2] == 7, "removed[2] incorrect");
    TEST_ASSERT_MESSAGE( removed[3] == 8, "removed[3] incorrect");
    TEST_ASSERT_MESSAGE( removed[4] == 9, "removed[4] incorrect");

    TEST_LCL_OK(lcl_vect_free(&vec));

}


static void vect_insert() {

    int* vec;    
    int values[] = {1,2,3,4,5,6,7,8,9,10};
    TEST_LCL_OK(lcl_vect_from( &vec, &values, 10 ));

    int inserters[] = {101,102,103};
    TEST_LCL_OK( lcl_vect_inserts( &vec, 3, inserters, 3 ) );

    int expected[] = {1,2,3,101,102,103,4,5,6,7,8,9,10};

    TEST_ASSERT_MESSAGE( lcl_vect_len(vec) == 13, "vect_len() incorrect" );
    TEST_ASSERT_MESSAGE( lcl_vect_cap(vec) > 13, "vect_vap() incorrect" );

    for (size_t i = 0; i < 13; i++)
        TEST_ASSERT(vec[i] == expected[i]);

    TEST_LCL_OK(lcl_vect_free(&vec));

}

static void vect_splice() {
    
    int* vec;
    int values[] = {1,2,3,4,5,6,7,8,9,10};
    TEST_LCL_OK(lcl_vect_from( &vec, &values, 10 ));

    int spliced[5];
    TEST_LCL_OK( lcl_vect_splice( &vec, 3, &spliced, 5 ) );

    TEST_ASSERT_MESSAGE( lcl_vect_len(vec) == 5, "vect_len() incorrect");
    
    TEST_ASSERT_MESSAGE( vec[0] == 1, "vec[0] incorrect");
    TEST_ASSERT_MESSAGE( vec[1] == 2, "vec[1] incorrect");
    TEST_ASSERT_MESSAGE( vec[2] == 3, "vec[2] incorrect");
    TEST_ASSERT_MESSAGE( vec[3] == 9, "vec[3] incorrect");
    TEST_ASSERT_MESSAGE( vec[4] == 10, "vec[4] incorrect");

    TEST_ASSERT_MESSAGE( spliced[0] == 4, "spliced[0] incorrect");
    TEST_ASSERT_MESSAGE( spliced[1] == 5, "spliced[1] incorrect");
    TEST_ASSERT_MESSAGE( spliced[2] == 6, "spliced[2] incorrect");
    TEST_ASSERT_MESSAGE( spliced[3] == 7, "spliced[3] incorrect");
    TEST_ASSERT_MESSAGE( spliced[4] == 8, "spliced[4] incorrect");

    TEST_LCL_OK(lcl_vect_free(&vec));

}

static void vect_clone() {

    int *a, *b;

    int values[] = {1,2,3,4,5,6,7,8};

    TEST_LCL_OK(lcl_vect_from(&a, values, 8));
    TEST_ASSERT((b = lcl_vect_clone(a)) != NULL);
    
    TEST_ASSERT_MESSAGE( lcl_vect_len( b ) == lcl_vect_len(a), "len(a) != len(b)" );
    TEST_ASSERT_MESSAGE( lcl_vect_isize( b ) == lcl_vect_isize(a), "isize(a) != isize(b)" );

    TEST_ASSERT_MESSAGE( a[0] == b[0], "a[0] != b[0]" );
    TEST_ASSERT_MESSAGE( a[1] == b[1], "a[1] != b[1]" );
    TEST_ASSERT_MESSAGE( a[2] == b[2], "a[2] != b[2]" );
    TEST_ASSERT_MESSAGE( a[3] == b[3], "a[3] != b[3]" );
    TEST_ASSERT_MESSAGE( a[4] == b[4], "a[4] != b[4]" );
    TEST_ASSERT_MESSAGE( a[5] == b[5], "a[5] != b[5]" );
    TEST_ASSERT_MESSAGE( a[6] == b[6], "a[6] != b[6]" );
    TEST_ASSERT_MESSAGE( a[7] == b[7], "a[7] != b[7]" );

    TEST_LCL_OK(lcl_vect_free(&a));
    TEST_LCL_OK(lcl_vect_free(&b));


}


void unit_vect_main() {

    RUN_TEST( vect_init );
    RUN_TEST( vect_push_ext );
    RUN_TEST( vect_trunc );
    RUN_TEST( vect_insert );
    RUN_TEST( vect_splice );
    RUN_TEST( vect_clone );

}