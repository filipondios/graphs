#include <stdlib.h>
#include <hash_map.h>

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

void* map_get_or_create_entry(hash_map* map, size_t key, size_t entry_size);
void* map_get_entry(hash_map* map, size_t key, size_t entry_size);
