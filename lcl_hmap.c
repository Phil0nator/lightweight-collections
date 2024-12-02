#include "lcl_hmap.h"

#include <stdbool.h>

#include "lcl_vect.h"

#include "private/lcl_util.h"

typedef struct lcl_hmap_node {
    lcl_hash_t hash;
    void* key;
    void* value;
    bool present;
} lcl_hmap_node_t;

typedef struct lcl_hmap {

    lcl_hashfn_t hfn;
    lcl_deallocfn_t keydealloc;

    lcl_hmap_node_t* nodes;
    size_t size;

} lcl_hmap_t;



lcl_err_t lcl_hmap_init(lcl_hmap_t **dest, lcl_hashfn_t hfn, lcl_deallocfn_t keydealloc)
{

    if (!dest || !hfn) return LCL_BAD_ARGUMENT;

    lcl_hmap_t* hmap = (lcl_hmap_t*) malloc (sizeof(lcl_hmap_t));
    hmap->hfn = hfn;
    hmap->keydealloc = keydealloc;
    hmap->size = 0;
    LCL_ERRPASS(lcl_vect_init(&hmap->nodes, 16));
    LCL_ERRPASS(lcl_vect_set_len(&hmap->nodes, 16));
    memset( hmap->nodes, 0, lcl_vect_bytes(hmap->nodes) );
    
    *dest = hmap;

    return LCL_OK;
}


/**
 * @brief 
 * 
 * @param nodes 
 * @param nodesc 
 * @param key 
 * @param data 
 * @param hash 
 * @return void* NULL if no previous, void* ptr if there was already something there
 */
static void* lcl_hmap_nodes_insert( lcl_hmap_node_t* nodes, size_t nodesc, void* key, void* data, lcl_hash_t hash ) {
    

    // length must ALWAYS be power of 2
    size_t hindex = hash & (nodesc-1);

    if (nodes[hindex].present) {
        if (nodes[hindex].hash == hash) {
            void* temp = nodes[hindex].value;
            nodes[hindex].value = data;
            return temp;
        } else {
            do {
                hindex ++;
                if (hindex >= nodesc) hindex = 0;
            } while (nodes[hindex].present);
        }
    }

    nodes[hindex].key = key;
    nodes[hindex].value = data;
    nodes[hindex].hash = hash;
    nodes[hindex].present = true;

    return NULL;

}

static lcl_err_t lcl_hmap_expand( lcl_hmap_t* dest ) {
    lcl_hmap_node_t* new_nodes;
    size_t nodescount = lcl_vect_len(dest->nodes);
    LCL_ERRPASS(lcl_vect_init(&new_nodes, nodescount*2));
    LCL_ERRPASS(lcl_vect_set_len(&new_nodes, nodescount*2));
    memset( new_nodes, 0, lcl_vect_bytes(new_nodes) );
    for (size_t i = 0; i < nodescount; i++) {
        lcl_hmap_nodes_insert( new_nodes, nodescount * 2, dest->nodes[i].key, dest->nodes[i].value, dest->nodes[i].hash );
    }

    
    lcl_vect_free( dest->nodes );

    dest->nodes = new_nodes;

    return LCL_OK;

}

lcl_err_t lcl_hmap_insert(lcl_hmap_t *dest, void *key, void *data, void** old)
{
    
    if (!dest) return LCL_BAD_ARGUMENT;

    size_t nodescount = lcl_vect_len(dest->nodes);

    if (dest->size * 2 >= nodescount) {
        LCL_ERRPASS(lcl_hmap_expand(dest));
    }
    
    lcl_hash_t hash = dest->hfn(key);
    void* old_value;
    if ((old_value = lcl_hmap_nodes_insert( dest->nodes, nodescount, key, data, hash )) == NULL) {
        dest->size ++;
    } else {
        lcl_dealloc(dest->keydealloc, &key);
    }

    if (old) *old = old_value;

    return LCL_OK;

}

lcl_hmap_iter_t lcl_hmap_find(lcl_hmap_t *hmap, const void *key)
{
    lcl_hash_t hash = hmap->hfn(key);
    size_t nodesc = lcl_vect_len(hmap->nodes);
    size_t hindex = hash & (nodesc-1);

    if (hmap->nodes[hindex].present) {
        if (hmap->nodes[hindex].hash != hash) {
            do {
                hindex ++;
                if (hindex >= nodesc) hindex = 0;
            } while (hmap->nodes[hindex].hash != hash);
        }
    } else return lcl_hmap_end(hmap);
    return (lcl_hmap_iter_t) { ._hmap = hmap, ._index = hindex };

}

lcl_err_t lcl_hmap_get(lcl_hmap_t *hmap, const void *key, void **data)
{
    if (!hmap) return LCL_BAD_ARGUMENT;

    lcl_hmap_iter_t i = lcl_hmap_find(hmap, key);
    if (i._index < lcl_vect_len(hmap->nodes)) {
        *data = hmap->nodes[i._index].value;
        return LCL_OK;
    } else return LCL_NOT_FOUND;

}

bool lcl_hmap_contains(lcl_hmap_t *hmap, const void *key)
{
    return lcl_hmap_find(hmap, key)._index < lcl_vect_len(hmap->nodes);
}

lcl_err_t lcl_hmap_remove(lcl_hmap_t *hmap, const void *key)
{
    lcl_hmap_iter_t i = lcl_hmap_find( hmap, key );
    return lcl_hmap_remove_it(hmap, i);
}

lcl_err_t lcl_hmap_remove_it(lcl_hmap_t *hmap, lcl_hmap_iter_t i)
{
    if (i._index < lcl_vect_len(hmap->nodes)) {
        hmap->nodes[i._index].present = false;   
        lcl_dealloc( hmap->keydealloc, &hmap->nodes[i._index].key );
    }
    return LCL_OK;
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

void *lcl_hmap_iter_key(lcl_hmap_iter_t i)
{
    return i._hmap->nodes[i._index].key;
}

void *lcl_hmap_iter_value(lcl_hmap_iter_t i)
{
    return i._hmap->nodes[i._index].value;
}

lcl_err_t lcl_hmap_display(lcl_hmap_t *hmap, const char *keyfmt, const char *valuefmt)
{
    printf("{");
    for lcl_hmap_each(it, hmap) {
        const void* key = lcl_hmap_iter_key(it);
        const void* value = lcl_hmap_iter_value(it);
        if (!lcl_hmap_iter_eq(it, lcl_hmap_begin(hmap))) printf(", ");
        printf_unkown( &key, sizeof(key), keyfmt );
        printf(" : ");
        printf_unkown( &value, sizeof(value), valuefmt );
    }
    printf("}\n");
    return LCL_OK;
}

lcl_err_t lcl_hmap_free(lcl_hmap_t **hmap, lcl_deallocfn_t vdealloc)
{
    if (!hmap || !(*hmap)) return LCL_BAD_ARGUMENT;
    lcl_hmap_t* h = *hmap;

    for (size_t i = 0; i < lcl_vect_len(h->nodes); i++) {
        if (h->nodes[i].present) {
            lcl_dealloc(h->keydealloc, &h->nodes[i].key);
            lcl_dealloc(vdealloc, &h->nodes[i].value);
        }
    }

    LCL_ERRPASS(lcl_vect_free(&h->nodes));
    free(h);
    *hmap = NULL;
}
