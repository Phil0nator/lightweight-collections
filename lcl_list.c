#include "lcl_list.h"


#include <stdlib.h>
#include <memory.h>


typedef struct lcl_list_node {
    struct lcl_list_node* prev, *next;
} lcl_list_node_t;

typedef struct lcl_list {

    lcl_list_node_t* head;
    size_t len;

} lcl_list_t;


lcl_err_t lcl_list_init( lcl_list_t** list, size_t initial_capacity, size_t itemsize) {
    lcl_list_t* l = (lcl_list_t*) malloc(sizeof(lcl_list_t));
    if (!l) return LCL_NO_MEMORY;
    l->len = 0;
    l->head = NULL;
    *list = l;
    return LCL_OK;
}

size_t lcl_list_len(lcl_list_t * list)
{
    return list->len;
}
