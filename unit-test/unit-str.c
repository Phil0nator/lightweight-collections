#include "lcl_string.h"
#include "unity.h"


#define TEST_LCL_OK( err ) TEST_ASSERT_MESSAGE( (err) == LCL_OK, "Expected LCL_OK" )

static void str_init() {

    lcl_str_t s;
    const char test_string[] = "Hello World!";
    TEST_LCL_OK(lcl_str_from( &s, test_string ));

    TEST_ASSERT_EQUAL_STRING_MESSAGE( s, test_string, "lcl_str_from() invalid" );

    TEST_LCL_OK(lcl_str_free(&s));

    TEST_LCL_OK(lcl_str_froms( &s, test_string, sizeof(test_string)));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( s, test_string, "lcl_str_froms() invalid" );

    TEST_LCL_OK(lcl_str_free(&s));


}


static void str_concatenate() {
    lcl_str_t s;
    const char test_string[] = "Hello World!";
    const char* expected = "Hello World!Hello World!";
    TEST_LCL_OK(lcl_str_from( &s, test_string ));

    TEST_LCL_OK(lcl_str_push( &s, test_string[0] ));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( s, "Hello World!H", "lcl_str_push() invalid" );

    TEST_LCL_OK(lcl_str_extend( &s, &test_string[1] ));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( s, expected, "lcl_str_extend() invalid" );
    
    TEST_LCL_OK(lcl_str_free(&s));

}

static void str_trunc() {

    lcl_str_t s;
    TEST_LCL_OK(lcl_str_from( &s, "Hello World!" ));

    char last;
    TEST_LCL_OK(lcl_str_pop(&s, &last));
    TEST_ASSERT_MESSAGE( last == '!', "str_pop() invalid" );

    char world[6] = {0};
    TEST_LCL_OK( lcl_str_truncate( &s, world, 5 ) );
    TEST_ASSERT_EQUAL_STRING_MESSAGE( "World", world, "str_truncate() invalid" );
    TEST_ASSERT_EQUAL_STRING_MESSAGE( "Hello ", s, "str_truncate() invalid" );


    TEST_LCL_OK(lcl_str_free(&s));
}


static void str_insert() {

    lcl_str_t s;
    TEST_LCL_OK(lcl_str_from( &s, "Hello World!" ));

    TEST_LCL_OK(lcl_str_insert( &s, 0, '>' ));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( ">Hello World!", s, "str_insert() invalid" );

    TEST_LCL_OK(lcl_str_inserts( &s, 8, "There, " ));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( ">Hello There, World!", s, "str_insert() invalid" );

    TEST_LCL_OK(lcl_str_free(&s));
}

static void str_splice() {

    

}

void unit_str_main() {

    RUN_TEST( str_init );
    RUN_TEST( str_concatenate );
    RUN_TEST( str_trunc );
    


}