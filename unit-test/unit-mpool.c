

#include "unity.h"
#include "lcl_mpool.h"

#include "units.h"

static void mpool_test() {

    lcl_mpool_t *mpool;
    TEST_LCL_OK(lcl_mpool_init(&mpool, sizeof(int)));

    int* my_int = (lcl_mpool_alloc( mpool ));
    *my_int= 0;

    int* next_int = (lcl_mpool_alloc( mpool ));
    *next_int = 1;

    for (int i = 2; i < 27; i++) {
        int* test = lcl_mpool_alloc(mpool) ;
        *test = i;
    }

    for (int i = 0; i < 27; i++) {
        TEST_ASSERT_MESSAGE( my_int[i] == i, "mpool invalid" );
    }

    for (int i = 0; i < 27; i += 2) {
        int* dummy = &my_int[i];
        TEST_LCL_OK(lcl_mpool_dealloc( mpool, (void**) &dummy ));
    }

    for (int i = 0; i < 27; i+= 2) {
        int* test = lcl_mpool_alloc(mpool) ;
        *test = i;
    }

    for (int i = 0; i < 27; i++) {
        TEST_ASSERT_MESSAGE( my_int[i] == i, "mpool invalid" );
    }

    TEST_LCL_OK(lcl_mpool_free(&mpool));
}

void unit_mpool_main()
{
    RUN_TEST(mpool_test);
}