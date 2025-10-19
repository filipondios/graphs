#include <stddef.h>
#include <stdlib.h>
#include "graph.h"

#define MAP_DEF_CAPACITY 16
#define REHASH_THRESHOLD 0.75f


hash_map* map_create(size_t entry_size, size_t capacity) {
    if (!entry_size) { return NULL; }
    if (!capacity) { capacity = MAP_DEF_CAPACITY; }

    hash_map* map = malloc(sizeof(hash_map));
    if (!map) { return NULL; }

    map->entries = calloc(capacity, entry_size);
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

size_t size_t_hash(size_t n) {
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

inline int map_ensure_capacity(hash_map* map, size_t entry_size) {
    if (!map) { return ERR_INPUT; };

    // duplicate capacity and alloc new memory buffer
    const size_t capacity = map->capacity << 1; // *=2
    void* new_entries = calloc(capacity, entry_size);
    if (!new_entries) { return ERR_ALLOC; }

    // TODO rehash old entries into the new map buffer
    
    free(map->entries);
    map->entries = new_entries;
    map->capacity = capacity;
    return ERR_NONE;
}

hash_map* map_get_or_create_entry(hash_map* graph, size_t key, size_t entry_size) {
    const size_t idx = size_t_hash(key) % graph->capacity;
    // TODO loop if collisions and return existent or new entry
    return NULL;
}

int graph_add_connection(hash_map* graph, size_t u, size_t v, size_t w) {
    if (!graph) { return ERR_INPUT; }
    // TODO use map_get_or_create
    return ERR_NONE;
}
