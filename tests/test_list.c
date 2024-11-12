#include "lcl_list.h"


#include <stdio.h>



int main() {

    lcl_list_t* list;
    lcl_list_init(&list, sizeof(int) );

    int my_stuff[] = { 1,2,3,4 };
    if (lcl_list_extend( list, my_stuff, 4 ) != LCL_OK) printf("Ouch!");
    lcl_list_display( list, "%d" );

    lcl_list_inserts( list, lcl_list_index(list, 2), my_stuff, 4 );
    lcl_list_display( list, "%d" );


    lcl_list_truncate( list, NULL, 3 );
    lcl_list_display( list, "%d" );


    lcl_list_splice( list, lcl_list_index( list, 1 ), NULL, 2 );
    lcl_list_display( list, "%d" );

    lcl_list_swap( list, lcl_list_index( list, 1 ), lcl_list_index(list, 0) );
    lcl_list_display( list, "%d" );





    return 0;
}