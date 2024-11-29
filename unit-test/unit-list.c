#include "lcl_list.h"
#include "unity.h"

#include "units.h"

static void list_init_loop() {

    lcl_list_t* list;
    int arr[] = {1,2,3,4};

    TEST_LCL_OK(lcl_list_from(&list, arr, 4, sizeof(int)));

    TEST_ASSERT_MESSAGE( lcl_list_len(list) == 4, "list_len() invalid");

    lcl_list_it_t *it = lcl_list_begin(list);
    TEST_ASSERT_MESSAGE( *(int*)lcl_list_it_get(it) == 1, "list[0] invalid" );
    it = lcl_list_it_next(it);
    TEST_ASSERT_MESSAGE( *(int*)lcl_list_it_get(it) == 2, "list[1] invalid" );
    it = lcl_list_it_next(it);
    TEST_ASSERT_MESSAGE( *(int*)lcl_list_it_get(it) == 3, "list[2] invalid" );
    it = lcl_list_it_next(it);
    TEST_ASSERT_MESSAGE( *(int*)lcl_list_it_get(it) == 4, "list[3] invalid" );

    TEST_LCL_OK(lcl_list_free( &list ));

}

static void list_trunc() {

    lcl_list_t* list;
    int arr[] = {1,2,3,4,5,6,7,8,9,10};

    TEST_LCL_OK(lcl_list_from(&list, arr, 10, sizeof(int)));

    int removed[5];
    TEST_LCL_OK(lcl_list_truncate( list, removed, 5 ));
    TEST_ASSERT_MESSAGE( removed[0] == 6, "removed[0] invalid" );
    TEST_ASSERT_MESSAGE( removed[1] == 7, "removed[1] invalid" );
    TEST_ASSERT_MESSAGE( removed[2] == 8, "removed[2] invalid" );
    TEST_ASSERT_MESSAGE( removed[3] == 9, "removed[3] invalid" );
    TEST_ASSERT_MESSAGE( removed[4] == 10, "removed[4] invalid" );

    TEST_ASSERT_MESSAGE( lcl_list_len(list) == 5, "list_len() invalid" );
    

    TEST_LCL_OK(lcl_list_free( &list ));

}


static void list_insert() {

    lcl_list_t* list;
    int arr[] = {1,2,3,4};

    TEST_LCL_OK(lcl_list_from(&list, arr, 4, sizeof(int)));

    TEST_LCL_OK(lcl_list_inserts( list, lcl_list_index(list, 2), arr, 4 ));
    
    lcl_list_it_t *it = lcl_list_begin(list);
    TEST_ASSERT_MESSAGE( *(int*)lcl_list_it_get(it) == 1, "list[0] invalid" );
    it = lcl_list_it_next(it);
    TEST_ASSERT_MESSAGE( *(int*)lcl_list_it_get(it) == 2, "list[1] invalid" );
    it = lcl_list_it_next(it);
    TEST_ASSERT_MESSAGE( *(int*)lcl_list_it_get(it) == 1, "list[2] invalid" );
    it = lcl_list_it_next(it);
    TEST_ASSERT_MESSAGE( *(int*)lcl_list_it_get(it) == 2, "list[3] invalid" );
    it = lcl_list_it_next(it);
    TEST_ASSERT_MESSAGE( *(int*)lcl_list_it_get(it) == 3, "list[4] invalid" );
    it = lcl_list_it_next(it);
    TEST_ASSERT_MESSAGE( *(int*)lcl_list_it_get(it) == 4, "list[5] invalid" );
    it = lcl_list_it_next(it);
    TEST_ASSERT_MESSAGE( *(int*)lcl_list_it_get(it) == 3, "list[6] invalid" );
    it = lcl_list_it_next(it);
    TEST_ASSERT_MESSAGE( *(int*)lcl_list_it_get(it) == 4, "list[7] invalid" );
    TEST_LCL_OK(lcl_list_free(&list));
}

static void list_splice() {
    lcl_list_t* list;
    int arr[] = {1,2,3,4,5,6,7,8,9,10};

    TEST_LCL_OK(lcl_list_from(&list, arr, 10, sizeof(int)));

    int removed[3];
    TEST_LCL_OK(lcl_list_splice( list, lcl_list_index(list, 2), removed, 3 ));
    TEST_ASSERT_MESSAGE( removed[0] == 3, "removed[0] invalid" );
    TEST_ASSERT_MESSAGE( removed[1] == 4, "removed[1] invalid" );
    TEST_ASSERT_MESSAGE( removed[2] == 5, "removed[2] invalid" );

    lcl_list_t* rmlist;
    TEST_LCL_OK(lcl_list_splice_links( list, lcl_list_index(list, 2), &rmlist, 3 ));
    lcl_list_it_t *it = lcl_list_begin(rmlist);
    TEST_ASSERT_MESSAGE( *(int*)lcl_list_it_get(it) == 6, "rmlist[0] invalid" );
    it = lcl_list_it_next(it);
    TEST_ASSERT_MESSAGE( *(int*)lcl_list_it_get(it) == 7, "rmlist[1] invalid" );
    it = lcl_list_it_next(it);
    TEST_ASSERT_MESSAGE( *(int*)lcl_list_it_get(it) == 8, "rmlist[2] invalid" );

    it = lcl_list_begin(list);
    TEST_ASSERT_MESSAGE( *(int*)lcl_list_it_get(it) == 1, "list[0] invalid" );
    it = lcl_list_it_next(it);
    TEST_ASSERT_MESSAGE( *(int*)lcl_list_it_get(it) == 2, "list[1] invalid" );
    it = lcl_list_it_next(it);
    TEST_ASSERT_MESSAGE( *(int*)lcl_list_it_get(it) == 9, "list[2] invalid" );
    it = lcl_list_it_next(it);
    TEST_ASSERT_MESSAGE( *(int*)lcl_list_it_get(it) == 10, "list[3] invalid" );

    TEST_ASSERT_MESSAGE( lcl_list_len(list) == 4, "list_len() invalid");
    TEST_ASSERT_MESSAGE( lcl_list_len(rmlist) == 3, "list_len() invalid");


    TEST_LCL_OK(lcl_list_free(&rmlist));
    TEST_LCL_OK(lcl_list_free(&list));

}



void unit_list_main() {
    RUN_TEST(list_init_loop);
    RUN_TEST(list_trunc);
    RUN_TEST(list_insert);
    RUN_TEST(list_splice);
}