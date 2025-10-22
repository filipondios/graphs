#ifndef _HASH_MAP_H_
#define _HASH_MAP_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hash_map hash_map;
struct hash_map { size_t capacity; size_t count; void* entries; };

hash_map* map_create(size_t entry_size, size_t capacity);
void map_destroy(hash_map* map);
void* map_get_or_create_entry(hash_map* map, size_t key, size_t entry_size, size_t value_size);
void* map_get_entry(hash_map* map, size_t key, size_t entry_size, size_t value_size);

#ifdef __cplusplus
}
#endif
#endif
