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

lcl_err_t lcl_list_from( lcl_list_t** list, const void* elements, size_t count, size_t itemsize ) {
    LCL_ERRPASS(lcl_list_init(list, itemsize));
    LCL_ERRPASS(lcl_list_extend(*list, elements, count));
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

lcl_list_it_t *lcl_list_middle(lcl_list_t *list)
{
    if (!list) return NULL;
    if (!list->head) return NULL;
    if (list->head->prev == NULL) return list->head;
    
    return lcl_list_it_middle(list->head);

}

lcl_list_it_t *lcl_list_it_middle(lcl_list_it_t *it)
{
    lcl_list_it_t* slow = it;
    lcl_list_it_t* fast = it;

    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
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
            list->len ++;
            items --;
        }

        for (size_t i = 0; i < items; i++) {
            last->next = lcl_list_make_node( list->isize, bytereader );
            if(!last->next) return LCL_NO_MEMORY;
            last->next->prev = last;
            last = last->next;
            list->head->prev = last;
            bytereader += list->isize;
            list->len++;
        }

    } else return LCL_BAD_ARGUMENT;
    return LCL_OK;
}

lcl_err_t lcl_list_truncate(lcl_list_t *list, void *dest, size_t count)
{
    if (list) {
        lcl_list_node_t* last = lcl_list_last_node(list);
        char* bytewriter = (char*) dest + ((count-1) * list->isize);
        bool do_write = dest != NULL;

        for (size_t i = 0; i < count; i ++) {
            if (!last) return LCL_COLLECTION_EMPTY;

            if (do_write) {
                memcpy(bytewriter, lcl_list_node_content(last), list->isize);
            }
            
            if (last != list->head) {
                last = last->prev;
                if (last) {
                    free(last->next);
                    last->next = NULL;
                }
            } else {
                free(last);
                last = NULL;
                list->head = NULL;
            }
            bytewriter -= list->isize;
            list->len --;
        }
        if (list->head != last) list->head->prev = last;
        else if (list->head) {
            list->head->next = NULL;
            list->head->prev = NULL;
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
            list->len ++;
        }

        prev->next = ender;
        if (ender) ender->prev = prev;
        else {
            list->head->prev = prev;
        }

    } else return LCL_BAD_ARGUMENT;
    return LCL_OK;
}
lcl_err_t lcl_list_take(lcl_list_t *list, lcl_list_it_t *at, lcl_list_t **other) {
    if (!list || !at || !other || !*other) {
        return LCL_BAD_ARGUMENT;  // Check for valid arguments
    }

    lcl_list_t* source = *other;
    *other = NULL;  // Nullify `*other` to indicate the source list is being consumed

    if (!source->head) {
        free(source);  // If the source list is empty, just free it
        return LCL_OK;
    }

    lcl_list_node_t* source_head = source->head;
    lcl_list_node_t* source_tail = source->head->prev;

    if (at == list->head) {
        // If `at` is the head, insert the source list at the beginning
        lcl_list_node_t* target_head = list->head;

        source_tail->next = target_head;
        if (target_head) {
            target_head->prev = source_tail;
        }

        list->head = source_head;  // Update head pointer to the new first node
        source_head->prev = NULL;
    } else {
        // Insert source list before `at`
        lcl_list_node_t* prev_node = at->prev;

        prev_node->next = source_head;
        source_head->prev = prev_node;

        source_tail->next = at;
        at->prev = source_tail;
    }

    free(source);  // Free the source list structure
    return LCL_OK;
}

lcl_err_t lcl_list_take_extend(lcl_list_t *dest, lcl_list_t **src) {
    if (!dest || !src || !*src) {
        return LCL_BAD_ARGUMENT;  // Check for valid arguments
    }

    lcl_list_t *source = *src;   // Take ownership of the source list
    *src = NULL;                 // Nullify the source pointer to indicate it's consumed

    if (!source->head) {
        free(source);            // If the source list is empty, free its structure
        return LCL_OK;
    }

    dest->len += source->len;

    if (!dest->head) {
        // If the destination list is empty, make the source list the new destination
        dest->head = source->head;
        dest->head->prev = (source->head->prev) ? source->head->prev : source->head; // Handle single node
        free(source);            // Free the source structure
        return LCL_OK;
    }

    // Both lists are non-empty; append the source list to the destination
    lcl_list_node_t *dest_tail = dest->head->prev ? dest->head->prev : dest->head; // Handle single node
    lcl_list_node_t *source_head = source->head;
    lcl_list_node_t *source_tail = source->head->prev ? source->head->prev : source->head; // Handle single node
    // Connect the destination's tail to the source's head
    dest_tail->next = source_head;
    source_head->prev = dest_tail;
    
    // Update the destination's tail to the source's tail
    dest->head->prev = source_tail;
    source_tail->next = NULL; // Explicitly terminate the combined list

    dest->head->prev->next = NULL;

    free(source);                // Free the source structure
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
            list->len --;
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

lcl_err_t lcl_list_splice_links(lcl_list_t *list, lcl_list_it_t *at, lcl_list_t **dest, size_t count)
{
    if (list && at && dest && count) {
        lcl_list_node_t* first = at;
        lcl_list_node_t* last = at;
        LCL_ERRPASS(lcl_list_init( dest, list->isize ));
        LCL_ERRPASS(lcl_list_it_advance_by(&last, count-1));

        
        if (first->prev) 
            first->prev->next = last->next;
        if (last->next)
            last->next->prev = first->prev;
        
        if (first == list->head) {
            list->head = last->next;
            list->head->prev = first->prev;
        }

        if (last == list->head->prev) {
            list->head->prev = first->prev;
            first->prev->next = NULL;
        }

        

        (*dest)->head = first;
        (*dest)->head->prev = last;
        last->next = NULL;
        (*dest)->len = count;
        list->len -= count;

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
    lcl_list_node_t* tail = list->head ? list->head->prev : NULL;

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
    if (list->head == a) {
        list->head = b;
        list->head->prev = tail;
    }
    else if (list->head == b) {
        list->head = a;
        list->head->prev = tail;
    }

    // Update tail if necessary
    if (list->head->prev == a) list->head->prev = b;
    else if (list->head->prev == b) list->head->prev = a;
    
    return LCL_OK;
}

lcl_list_t *lcl_list_clone(lcl_list_t *list)
{
    lcl_list_t* output;
    if (lcl_list_init( &output, list->isize ) != LCL_OK) return NULL;
    
    for lcl_list_each(i, list) {
        if (lcl_list_push( output, lcl_list_it_get(i) ) != LCL_OK) return NULL;
    }

    return output;
}

static lcl_list_it_t* sorted_merge(lcl_list_it_t* left, lcl_list_it_t* right, lcl_refcomparator_t cmp) {
    
    if (!left) return right;
    if (!right) return left;

    lcl_list_node_t* result;
    if (cmp( lcl_list_node_content(left), lcl_list_node_content(right) ) > 0) 
    {
        result = left;
        result->next = sorted_merge(left->next, right, cmp);
        result->next->prev = result;
        result->prev = NULL;
    }
    else 
    {
        result = right;
        result->next = sorted_merge(left, right->next, cmp);
        result->next->prev = result; 
        result->prev = NULL;
    }

    return result;
}

static lcl_list_it_t* merge_sort(lcl_list_it_t* list, lcl_refcomparator_t cmp) {

    if (!list || !list->next) return list;

    lcl_list_it_t* middle = lcl_list_it_middle(list);
    lcl_list_it_t* midnext = middle->next;
    
    middle->next = NULL;
    if (midnext) midnext->prev = NULL;

    lcl_list_it_t* left = merge_sort(list, cmp);
    lcl_list_it_t* right = merge_sort(midnext, cmp);

    return sorted_merge(left, right, cmp);

}

lcl_err_t lcl_list_sort(lcl_list_t *list, lcl_refcomparator_t cmp)
{
    if (!list || !cmp) return LCL_BAD_ARGUMENT;
    if (list->head == NULL) return LCL_OK;          // empty
    if (list->head->prev == NULL) return LCL_OK;    // 1 item

    list->head = merge_sort(list->head, cmp);
    lcl_list_it_t* it = list->head;
    while (it->next) it = it->next;
    list->head->prev = it;

    return LCL_OK;

}

lcl_err_t lcl_list_free(lcl_list_t **list)
{
    lcl_list_it_t* next = NULL;
    for (lcl_list_it_t *it = (*list)->head; it; it = next) {
        next = it->next;
        free(it);
    }
    free(*list);
    *list = NULL;
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



