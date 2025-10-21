#include <stddef.h>
#include <stdlib.h>
#include <hash_map.h>
#include <errors.h>

#define MAP_DEF_CAPACITY 16
#define REHASH_THRESHOLD(count, capacity) (count * 10 >= capacity * 7)

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
    #ifdef BUILD_64BIT
    // 64-bit version (SplitMix64)
    n += 0x9e3779b97f4a7c15ULL;
    n = (n ^ (n >> 30)) * 0xbf58476d1ce4e5b9ULL;
    n = (n ^ (n >> 27)) * 0x94d049bb133111ebULL;
    return n ^ (n >> 31);        
    #elifdef BUILD_32BIT
    // 32-bit version (Knuth)
    return n * 2654435761u;
    #else
    #error "Neither BUILD_64BIT or BUILD_32BIT is defined" 
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


void* map_get_entry(hash_map* map, size_t key, size_t entry_size, size_t value_size) {   
    const size_t idx = size_t_hash(key) % map->capacity;
    const unsigned char* base = (const unsigned char*) map->entries;
    
    for (size_t i = 0; i < map->capacity; ++i) {
        const size_t pos = (idx + i) % map->capacity;
        const unsigned char* entry = base + (entry_size * pos);
        
        const unsigned char entry_used = *((unsigned char*)
            (entry + sizeof(size_t) + value_size));
        if (!entry_used) { return (void*)entry; }
        
        const size_t entry_key = *((size_t*)entry);
        if (key == entry_key) { return (void*)entry; }
    }
    return NULL;
}

void* map_get_or_create_entry(hash_map* map, size_t key, size_t entry_size, size_t value_size) {
    unsigned char* entry = map_get_entry(map, key, entry_size, value_size);
    
    unsigned char* used_ptr = (unsigned char*)
        (entry + sizeof(size_t) + value_size);
    size_t* key_ptr = (size_t*)entry;

    if (!*used_ptr) {
        if (REHASH_THRESHOLD(map->count, map->capacity)) {
            // rehash the hash_map and then insert <key, ?> 
            int code = map_ensure_capacity(map, entry_size);
            if (code == ERR_ALLOC) { return NULL; }

            entry = map_get_entry(map, key, entry_size, value_size);
            used_ptr = (unsigned char*)(entry + sizeof(size_t) + value_size);
            key_ptr = (size_t*) entry;
        }       
        *key_ptr = key;
        *used_ptr = 1;
        ++map->count;
    }
    return (void*)entry;
}
