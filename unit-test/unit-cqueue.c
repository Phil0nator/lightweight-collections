#include "units.h"
#include "unity.h"

#include "lcl_cqueue.h"

static void cqueue_test() {


    lcl_cqueue_t* cqueue;
    TEST_LCL_OK(lcl_cqueue_alloc(&cqueue, sizeof(int), 32 ));
    for (int i = 1; i < 10; i++)
        TEST_LCL_OK(lcl_cqueue_enqueue( cqueue, &i ));

    for (int i = 1; i < 10; i++) {
        int test;
        TEST_LCL_OK(lcl_cqueue_dequeue( cqueue, &test ));
        TEST_ASSERT_MESSAGE( test == i, "cqueue_dequeue() invalid" );
    }

    lcl_cqueue_free(&cqueue);

}


    
void unit_cqueue_main() {

    RUN_TEST(cqueue_test);


}