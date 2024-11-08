#include <stdio.h>

#include "lcl_string.h"



int main() {

    lcl_str_t my_string;
    lcl_err_t err;

    lcl_str_from( &my_string, "Hello World!" );
    printf("%s\n", my_string);

    lcl_str_push(&my_string, '\n');
    printf("%s\n", my_string);


    lcl_str_extend(&my_string, "And a happy new year");
    printf("%s\n", my_string);

    lcl_str_pop( &my_string, NULL );
    printf("%s\n", my_string);

    lcl_str_splice(&my_string, 4, NULL, 5);
    printf("%s\n", my_string);

    

    return 0;
}