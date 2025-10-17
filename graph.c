#include <stddef.h>
#include <stdlib.h>
#include "graph.h"

#define ENTRY_SIZE sizeof(src_entry)
#define MAP_DEF_CAPACITY 16
#define REHASH_THRESHOLD 0.7f


hash_map* map_create(size_t capacity) {
    if (!capacity) { capacity = MAP_DEF_CAPACITY; }
    hash_map* map = malloc(sizeof(hash_map));
    if (!map) { return NULL; }

    map->entries = calloc(capacity, ENTRY_SIZE);
    if (!map->entries) { free(map); return NULL; }

    map->count = 0;
    map->capacity = capacity;
    return map;
}

void map_destroy(hash_map* map) {
    if (!map) { return; }
    if (map->entries) { free(map->entries); }
    free(map);
}

int graph_read_edges(hash_map* map, const edge* edges, size_t num_edges) {
    if (!map) { return ERR_INPUT; }
    int result;

    for (size_t i = 0; i < num_edges; ++i) {
        // calling add_connection should add
        // the edges (u -> v) & (v -> u)
        const edge* e = &edges[i];
        result = graph_add_connection(map, e->u, e->v, e->w);
        if (result == ERR_ALLOC) { return ERR_ALLOC; }
    }
    return ERR_NONE;
}

size_t compute_size_t_hash(size_t n) {
    #if SIZE_MAX == UINT64_MAX
    // 64-bit version (SplitMix64)
    n += 0x9e3779b97f4a7c15ULL;
    n = (n ^ (n >> 30)) * 0xbf58476d1ce4e5b9ULL;
    n = (n ^ (n >> 27)) * 0x94d049bb133111ebULL;
    return n ^ (n >> 31);        
    #else
    // 32-bit version (Knuth)
    return n * 2654435761u;
    #endif
}

hash_map* graph_get_or_create_adj(hash_map* graph, size_t u) {
    const size_t idx = compute_size_t_hash(u) % graph->capacity;

    for (size_t i = 0; i < graph->capacity; ++i) {
        // Search key in the table: iterate if collisions
        const size_t pos = (idx + i) % graph->capacity;
        src_entry* entry = (src_entry*) &graph->entries[pos];

        if (!entry->used) {
            entry->key = u;
            entry->value = map_create(0);
            if (!entry->value) { return NULL; }

            entry->used = 1;
            ++graph->count;
            return entry->value;

        } else if (entry->key == u) {
            return entry->value;
        }
    }
    // full
    return NULL;
}

int map_adj_insert_edge(hash_map* adj, size_t v, size_t w) {
    const size_t idx = compute_size_t_hash(v) % adj->capacity;

    for (size_t i = 0; i < adj->capacity; ++i) {
        // Search key in the table: iterate if collisions
        const size_t pos = (idx + i) % adj->capacity;
        dst_entry* entry = (dst_entry*) &adj->entries[pos];

        if (!entry->used) {
            entry->key = v;
            entry->value = w;
            entry->used = 1;
            ++adj->count;
            return ERR_NONE;

        } else if (entry->key == v) {
            entry->value = w;
            return ERR_NONE;            
        }
    }
    return ERR_REHASH;
}

int graph_add_connection(hash_map* graph, size_t u, size_t v, size_t w) {
    if (!graph) { return ERR_INPUT; }

    hash_map* adj_u = graph_get_or_create_adj(graph, u);
    hash_map* adj_v = graph_get_or_create_adj(graph, v);
    if (!adj_u || !adj_v) { return ERR_ALLOC; }

    int c1 = map_adj_insert_edge(adj_u, v, w);        
    int c2 = map_adj_insert_edge(adj_v, u, w);    
    return (c1 == ERR_REHASH || c2 == ERR_REHASH)? ERR_REHASH : ERR_NONE;
}
