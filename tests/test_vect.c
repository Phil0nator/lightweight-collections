#include "lcl_vect.h"


#include <stdio.h>


int main() {

    int* my_vect;
    lcl_vect_init( &my_vect, 8 );

    int item = 5;
    lcl_vect_push( &my_vect, &item);

    int others[] = {6, 7, 8};
    lcl_vect_extend( &my_vect, others, 3 );

    lcl_vect_display( my_vect, "%d" );

    lcl_vect_pop( &my_vect, &item );

    lcl_vect_display(my_vect, "%d");
    printf("last: %d\n", item);
    
    lcl_vect_truncate( &my_vect, NULL, 2 );

    lcl_vect_display(my_vect, "%d");


    lcl_vect_extend( &my_vect, others, 3 );

    lcl_vect_display(my_vect, "%d");

    int more[] = {10,11,12};
    lcl_vect_inserts( &my_vect, 2, more, 3 );

    lcl_vect_inserts( &my_vect, 2, more, 3 );

    lcl_vect_inserts( &my_vect, 2, more, 3 );

    lcl_vect_display(my_vect, "%d");

    lcl_vect_splice(&my_vect, 1, NULL, 3);

    lcl_vect_display(my_vect, "%d");


    lcl_vect_swap( my_vect, 0, 3 );

    lcl_vect_display(my_vect, "%d");
    lcl_vect_debug(my_vect);
    lcl_vect_clean(&my_vect);
    lcl_vect_debug(my_vect);

    lcl_vect_set_len(&my_vect, 256);
    lcl_vect_debug(my_vect);
    lcl_vect_free(&my_vect);

    return 0;
}