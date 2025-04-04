#include "lcl_hmap.h"

#include <stdbool.h>

#include "lcl_vect.h"

#include "private/lcl_util.h"

typedef struct lcl_hmap_node {
    lcl_hash_t hash;
    lcl_any_t  key;
    lcl_any_t  value;
    bool present;
} lcl_hmap_node_t;

typedef struct lcl_hmap {

    lcl_hashfn_t hfn;
    lcl_binary_predicate_t keyeq;

    lcl_hmap_node_t* nodes;
    size_t size;

} lcl_hmap_t;



lcl_err_t lcl_hmap_init(lcl_hmap_t **dest, lcl_hashfn_t hfn, lcl_binary_predicate_t keyeq)
{

    if (!dest || !hfn) return LCL_BAD_ARGUMENT;

    lcl_hmap_t* hmap = (lcl_hmap_t*) malloc (sizeof(lcl_hmap_t));
    if (!hmap) return LCL_NO_MEMORY;
    hmap->hfn = hfn;
    hmap->size = 0;
    hmap->keyeq = keyeq;
    LCL_ERRPASS(lcl_vect_init(&hmap->nodes, 16));
    LCL_ERRPASS(lcl_vect_set_len(&hmap->nodes, 16));
    memset( hmap->nodes, 0, lcl_vect_bytes(hmap->nodes) );
    
    *dest = hmap;

    return LCL_OK;
}


/**
 * @brief insert node raw helper function
 * 
 * @param keyeq binary predicate to compare keys
 * @param nodes nodes array to insert into
 * @param nodesc nodes count
 * @param key key to insert at
 * @param data data to insert
 * @param old old data write output
 * @param hash hash of key
 * @return true There was something
 * @return false There was nothing
 */
static bool lcl_hmap_nodes_insert( 
    lcl_binary_predicate_t keyeq, 
    lcl_hmap_node_t* nodes, 
    size_t nodesc, 
    lcl_any_t key, 
    lcl_any_t data, 
    lcl_any_t* old, 
    lcl_hash_t hash 
) {
    

    // length must ALWAYS be power of 2
    size_t hindex = hash & (nodesc-1);

    while (nodes[hindex].present) {
        if ( nodes[hindex].hash == hash && keyeq( nodes[hindex].key, key ) ) {
            if (old) *old = nodes[hindex].value;
            nodes[hindex].value = data;
            return true;
        } else {
            hindex ++;
            if (hindex >= nodesc) hindex = 0;
        }
    }

    nodes[hindex].key = key;
    nodes[hindex].value = data;
    nodes[hindex].hash = hash;
    nodes[hindex].present = true;

    return false;

}

static lcl_err_t lcl_hmap_expand( lcl_hmap_t* dest, size_t new_size ) {
    lcl_hmap_node_t* new_nodes;
    LCL_ERRPASS(lcl_vect_init(&new_nodes, new_size));
    LCL_ERRPASS(lcl_vect_set_len(&new_nodes, new_size));
    memset( new_nodes, 0, lcl_vect_bytes(new_nodes) );


    for (size_t i = 0; i < lcl_vect_len(dest->nodes); i++) {
        if (dest->nodes[i].present) 
            lcl_hmap_nodes_insert(dest->keyeq, new_nodes, new_size, dest->nodes[i].key, dest->nodes[i].value, NULL, dest->nodes[i].hash );
    }

    
    lcl_vect_free( &dest->nodes );

    dest->nodes = new_nodes;

    return LCL_OK;

}

lcl_err_t lcl_hmap_insert(lcl_hmap_t *dest, lcl_any_t key, lcl_any_t data, lcl_any_t * old, bool *old_written)
{
    
    if (!dest) return LCL_BAD_ARGUMENT;

    size_t nodescount = lcl_vect_len(dest->nodes);

    if (dest->size * 2 >= nodescount) {
        LCL_ERRPASS(lcl_hmap_expand(dest, nodescount * 2));
    }
    
    if (old_written) *old_written = false;
    
    lcl_hash_t hash = dest->hfn(key);
    lcl_any_t old_value;
    if (lcl_hmap_nodes_insert( dest->keyeq, dest->nodes, nodescount, key, data, &old_value, hash) == false) {
        dest->size ++;
    } else {
        if (old) {
            *old = old_value;
            if (old_written) *old_written = true;
        }
    }

    
    return LCL_OK;

}

lcl_hmap_iter_t lcl_hmap_find(lcl_hmap_t *hmap, lcl_any_t key)
{
    lcl_hash_t hash = hmap->hfn(key);
    size_t nodesc = lcl_vect_len(hmap->nodes);
    size_t hindex = hash & (nodesc-1);

    if (hmap->nodes[hindex].present) {
        while (hmap->nodes[hindex].present) {
            if ( hmap->nodes[hindex].hash == hash && hmap->keyeq( hmap->nodes[hindex].key, key ) ) {
                return (lcl_hmap_iter_t) { ._hmap = hmap, ._index = hindex };
            } else {
                hindex ++;
                if (hindex >= nodesc) hindex = 0;
            }
        }
    } 
    return lcl_hmap_end(hmap);

}

lcl_err_t lcl_hmap_get(lcl_hmap_t *hmap, lcl_any_t key, lcl_any_t *data)
{
    if (!hmap) return LCL_BAD_ARGUMENT;

    lcl_hmap_iter_t i = lcl_hmap_find(hmap, key);
    if (i._index < lcl_vect_len(hmap->nodes)) {
        *data = hmap->nodes[i._index].value;
        return LCL_OK;
    } else return LCL_NOT_FOUND;

}

bool lcl_hmap_contains(lcl_hmap_t *hmap, lcl_any_t key)
{
    return lcl_hmap_find(hmap, key)._index < lcl_vect_len(hmap->nodes);
}

bool lcl_hmap_remove(lcl_hmap_t *hmap, lcl_any_t key, lcl_any_t *keyout, lcl_any_t* valout)
{
    lcl_hmap_iter_t i = lcl_hmap_find( hmap, key );
    return lcl_hmap_remove_it(i, keyout, valout);
}

bool lcl_hmap_remove_it(lcl_hmap_iter_t i, lcl_any_t *keyout, lcl_any_t* valout)
{
    if (i._index < lcl_vect_len(i._hmap->nodes)) {
        i._hmap->nodes[i._index].present = false;   
        if (keyout) *keyout = i._hmap->nodes[i._index].key;
        if (valout) *valout = i._hmap->nodes[i._index].value;
        i._hmap->size --;
        return true;
    }
    return false;
}



lcl_hmap_iter_t lcl_hmap_begin(lcl_hmap_t *hmap)
{
    for (size_t i = 0; i < lcl_vect_len(hmap->nodes); i++) {
        if (hmap->nodes[i].present) 
            return (lcl_hmap_iter_t) {
                ._hmap = hmap,
                ._index = i
            };
    }
    return lcl_hmap_end(hmap);
}

lcl_hmap_iter_t lcl_hmap_end(lcl_hmap_t *hmap)
{
    return (lcl_hmap_iter_t) {
        ._hmap = hmap,
        ._index = lcl_vect_len(hmap->nodes)
    };
}

bool lcl_hmap_iter_eq(lcl_hmap_iter_t a, lcl_hmap_iter_t b)
{
    return (a._hmap == b._hmap && a._index == b._index);
}

lcl_hmap_iter_t lcl_hmap_iter_next(lcl_hmap_iter_t i)
{
    
    size_t nodesc = lcl_vect_len(i._hmap->nodes);
    if (i._index < nodesc) {
        do {
            i._index ++;
            if (i._hmap->nodes[i._index].present) return i;
        } while (i._index < nodesc );
    }

    return i;

}

lcl_hmap_iter_t lcl_hmap_iter_prev(lcl_hmap_iter_t i)
{
    if (i._index) {
        do {
            i._index --;
            if (i._hmap->nodes[i._index].present) return i;
        } while (i._index);
    }

    return i;
}

lcl_any_t lcl_hmap_iter_key(lcl_hmap_iter_t i)
{
    return i._hmap->nodes[i._index].key;
}

lcl_any_t lcl_hmap_iter_value(lcl_hmap_iter_t i)
{
    return i._hmap->nodes[i._index].value;
}

lcl_err_t lcl_hmap_display(lcl_hmap_t *hmap, const char *keyfmt, const char *valuefmt)
{
    printf("{");
    for lcl_hmap_each(it, hmap) {
        const void* key = lcl_any2ptr(lcl_hmap_iter_key(it));
        const void* value = lcl_any2ptr(lcl_hmap_iter_value(it));
        if (!lcl_hmap_iter_eq(it, lcl_hmap_begin(hmap))) printf(", ");
        printf_unkown( &key, sizeof(key), keyfmt );
        printf(" : ");
        printf_unkown( &value, sizeof(value), valuefmt );
    }
    printf("}\n");
    return LCL_OK;
}

lcl_hmap_t* lcl_hmap_clone(lcl_hmap_t *src)
{

    lcl_hmap_t *dst = (lcl_hmap_t*) malloc (sizeof(lcl_hmap_t));
    if (!dst) return NULL;
    dst->hfn = src->hfn;
    dst->size = src->size;
    dst->keyeq = src->keyeq;
    dst->nodes = lcl_vect_clone(src->nodes);
    if (!dst->nodes) {
        free(dst);
        return NULL;
    }
    return dst;
}

lcl_err_t lcl_hmap_free(lcl_hmap_t **hmap, lcl_deallocfn_t kdealloc, lcl_deallocfn_t vdealloc)
{
    if (!hmap || !(*hmap)) return LCL_BAD_ARGUMENT;
    lcl_hmap_t* h = *hmap;

    for (size_t i = 0; i < lcl_vect_len(h->nodes); i++) {
        if (h->nodes[i].present) {
            lcl_dealloc(kdealloc, &h->nodes[i].key);
            lcl_dealloc(vdealloc, &h->nodes[i].value);
        }
    }

    LCL_ERRPASS(lcl_vect_free(&h->nodes));
    free(h);
    *hmap = NULL;

    return LCL_OK;
}
