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

    TEST_LCL_OK(lcl_str_insert( &s, 0, ">" ));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( ">Hello World!", s, "str_insert() invalid" );

    TEST_LCL_OK(lcl_str_inserts( &s, 8, "There, " ));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( ">Hello There, World!", s, "str_insert() invalid" );

    TEST_LCL_OK(lcl_str_free(&s));
}
static void str_splice() {

    
    lcl_str_t s;
    TEST_LCL_OK(lcl_str_from( &s, "Hello World!" ));

    char world[6] = {0};
    TEST_LCL_OK(lcl_str_splice( &s, 6, world, 5 ));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( "Hello !", s, "str_splice() invalid" );
    TEST_ASSERT_EQUAL_STRING_MESSAGE( "World", world, "str_splice() invalid" );

    TEST_LCL_OK( lcl_str_splice(&s, 5, NULL, 2) );
    TEST_ASSERT_EQUAL_STRING_MESSAGE( "Hello", s, "str_splice() invalid" );
    
    TEST_LCL_OK(lcl_str_free(&s));

}
static void str_replace() {

    lcl_str_t s;
    TEST_LCL_OK(lcl_str_from( &s, "Hello World!" ));

    TEST_LCL_OK(lcl_str_replace( &s, 0, LCL_STR_REPLACEALL, "l", "< an l char >" ));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( "He< an l char >< an l char >o Wor< an l char >d!", s, "lcl_str_replace() invalid" );

    TEST_LCL_OK(lcl_str_replace( &s, 0, LCL_STR_REPLACEALL, "< an l char >", "l" ));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( "Hello World!", s, "lcl_str_replace() invalid" );

    TEST_LCL_OK(lcl_str_replace( &s, 0, LCL_STR_REPLACEALL, "l", "" ));
    TEST_ASSERT_EQUAL_STRING_MESSAGE( "Heo Word!", s, "lcl_str_replace() invalid" );


    TEST_LCL_OK(lcl_str_free(&s));

}


static void str_join() {

    lcl_str_t s = NULL;

    char* my_arr[] = {
        "This",
        "Is",
        "a",
        "Few Strings",
        "Put",
        "Together"
    };

    TEST_LCL_OK(lcl_str_join(&s, ", ", my_arr, 6));


    TEST_LCL_OK(lcl_str_free(&s));

}



void unit_str_main() {

    RUN_TEST( str_init );
    RUN_TEST( str_concatenate );
    RUN_TEST( str_trunc );
    RUN_TEST( str_splice );
    RUN_TEST( str_replace );
    RUN_TEST( str_join );

}