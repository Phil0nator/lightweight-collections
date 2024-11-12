#include "lcl_list.h"


#include <stdlib.h>
#include <memory.h>

#include "private/lcl_util.h"

#define lcl_list_node_content( node ) (&(node)[1])


typedef struct lcl_list_it {
    struct lcl_list_it* prev, *next;
} lcl_list_node_t;

typedef struct lcl_list {

    lcl_list_node_t* head;
    size_t len;
    size_t isize;

} lcl_list_t;


static lcl_list_node_t* lcl_list_last_node(lcl_list_t* list) {
    if (list->head) {
        if (list->head->prev) return list->head->prev;
        return list->head;
    } else return NULL;
}

static lcl_list_node_t* lcl_list_make_node( size_t isize, const void* data ) {
    lcl_list_node_t *node = malloc( sizeof(lcl_list_node_t) + isize );
    if (!node) return NULL;
    node->next = NULL;
    node->prev = NULL;
    memcpy( lcl_list_node_content( node ), data, isize );
    return node;
}

lcl_err_t lcl_list_init( lcl_list_t** list, size_t itemsize) {
    lcl_list_t* l = (lcl_list_t*) malloc(sizeof(lcl_list_t));
    if (!l) return LCL_NO_MEMORY;
    l->len = 0;
    l->isize = itemsize;
    l->head = NULL;
    *list = l;
    return LCL_OK;
}

size_t lcl_list_len(lcl_list_t * list)
{
    return list->len;
}

size_t lcl_list_isize(lcl_list_t *list)
{
    return list->isize;
}



void *lcl_list_last(lcl_list_t *list)
{
    return lcl_list_node_content( lcl_list_last_node( list ) );
}

lcl_list_it_t *lcl_list_begin(lcl_list_t *list)
{
    return list->head;
}

lcl_list_it_t *lcl_list_end(lcl_list_t *list)
{
    return (lcl_list_it_t*) lcl_list_last_node( list );
}

lcl_list_it_t *lcl_list_it_next(lcl_list_it_t *it)
{
    return it->next;
}

lcl_list_it_t *lcl_list_it_prev(lcl_list_it_t *it)
{
    return it->prev;
}

void *lcl_list_it_get(lcl_list_it_t *it)
{
    return lcl_list_node_content(it);
}

lcl_err_t lcl_list_it_advance_by(lcl_list_it_t **it, size_t by)
{
    lcl_list_it_t* x = *it;
    for(size_t i = 0; i < by; i++) {
        x = x->next;
        if (!x) return LCL_INDEX_OUT_OF_BOUNDS;
    }
    *it = x;
    return LCL_OK;
}

lcl_list_it_t *lcl_list_index(lcl_list_t *list, size_t idx)
{
    lcl_list_it_t *i = lcl_list_begin(list);
    if (lcl_list_it_advance_by( &i, idx ) != LCL_OK) return NULL;
    return i;
}

lcl_err_t lcl_list_extend(lcl_list_t *list, const void *data, size_t items)
{
    if (list) {
        lcl_list_node_t* last = lcl_list_last_node( list );
        const char* bytereader = (const char*) data;
        if (!last) {
            last = lcl_list_make_node( list->isize, data );
            if (!last) return LCL_NO_MEMORY;
            list->head = last;
            bytereader += list->isize;
            items --;
        }

        for (size_t i = 0; i < items; i++) {
            last->next = lcl_list_make_node( list->isize, bytereader );
            if(!last->next) return LCL_NO_MEMORY;
            last->next->prev = last;
            last = last->next;
            list->head->prev = last;
            bytereader += list->isize;
        }

    } else return LCL_BAD_ARGUMENT;
    return LCL_OK;
}

lcl_err_t lcl_list_truncate(lcl_list_t *list, void *dest, size_t count)
{
    if (list) {
        lcl_list_node_t* last = lcl_list_last_node(list);
        char* bytewriter = (char*) dest;
        bool do_write = dest != NULL;

        for (size_t i = 0; i < count; i ++) {
            if (!last) return LCL_COLLECTION_EMPTY;

            if (do_write) {
                memcpy(bytewriter, lcl_list_node_content(last), list->isize);
            }
            
            last = last->prev;
            if (last) {
                free(last->next);
                last->next = NULL;
            }
            bytewriter += list->isize;
        }


    } else return LCL_BAD_ARGUMENT;
    return LCL_OK;
}

lcl_err_t lcl_list_inserts(lcl_list_t *list, lcl_list_it_t *at, const void *data, size_t count)
{
    if (list && at) {
        lcl_list_node_t* ender = at;
        lcl_list_node_t* prev = at->prev;

        const char* bytereader = data;
        for (size_t i = 0; i < count; i++) {
            lcl_list_node_t* newn = lcl_list_make_node( list->isize, bytereader );
            newn->prev = prev;
            newn->prev->next = newn;
            bytereader += list->isize;
            prev = newn;
        }

        prev->next = ender;
        if (ender) ender->prev = prev;
        else {
            list->head->prev = prev;
        }

    } else return LCL_BAD_ARGUMENT;
    return LCL_OK;
}

lcl_err_t lcl_list_splice(lcl_list_t *list, lcl_list_it_t *at, void *data, size_t count)
{
    if (list && at) {
        lcl_list_node_t* node = at;
        lcl_list_node_t* before = at->prev;
        lcl_list_node_t* prev = at;

        char* bytewriter = (char*) data;
        bool do_write = data != NULL;

        for (size_t i = 0; i < count; i ++) {
            if (!node) return LCL_COLLECTION_EMPTY;

            if (do_write) {
                memcpy(bytewriter, lcl_list_node_content(node), list->isize);
            }
            
            prev=node;
            node = node->next;
            if (node) {
                free(node->prev);
                node->prev = NULL;
            }
            bytewriter += list->isize;
        }

        if (!node) {
            list->head = prev;
        }

        if (before) {
            node->prev = before;
            before->next = node;
        } else {
            list->head = before;
        }

    } else return LCL_BAD_ARGUMENT;
    return LCL_OK;
}

lcl_err_t lcl_list_swap(lcl_list_t *list, lcl_list_it_t *a, lcl_list_it_t *b)
{
    if (!(a && b && list)) return LCL_BAD_ARGUMENT;
    if (a == b) return LCL_OK;

    lcl_list_node_t* ap = a->prev;
    lcl_list_node_t* an = a->next;
    lcl_list_node_t* bp = b->prev;
    lcl_list_node_t* bn = b->next;

    // Swap pointers, handle adjacency case
    if (an == b) {  // a is immediately before b
        a->next = bn;
        a->prev = b;
        b->next = a;
        b->prev = ap;

        if (bn) bn->prev = a;
        if (ap) ap->next = b;
    } else if (bn == a) {  // b is immediately before a
        b->next = an;
        b->prev = a;
        a->next = b;
        a->prev = bp;

        if (an) an->prev = b;
        if (bp) bp->next = a;
    } else {  // Nodes are not adjacent
        a->prev = bp;
        a->next = bn;
        b->prev = ap;
        b->next = an;

        if (bn) bn->prev = a;
        if (an) an->prev = b;
        if (ap) ap->next = b;
        if (bp) bp->next = a;
    }



    // Update head if necessary
    if (list->head == a) list->head = b;
    else if (list->head == b) list->head = a;

    // Update tail if necessary
    if (list->head->prev == a) list->head->prev = b;
    else if (list->head->prev == b) list->head->prev = a;
    

    return LCL_OK;
}

lcl_err_t lcl_list_display(lcl_list_t *list, const char *fmt)
{

    printf("[");
    for lcl_list_each( it, list ) {
        if (it != lcl_list_begin( list )) printf(", ");
        printf_unkown( lcl_list_it_get( it ), list->isize, fmt );
    }
    printf("]\n");
    return LCL_OK;
}
