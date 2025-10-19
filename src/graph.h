#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stddef.h>

enum code { ERR_ALLOC, ERR_INPUT, ERR_NONE };

struct entry {
    size_t key;
    unsigned char used;
};

typedef struct edge edge;
typedef struct src_entry src_entry;
typedef struct dst_entry dst_entry;
typedef struct hash_map hash_map;
typedef struct hash_map graph;

struct edge { size_t u; size_t v; size_t w; };
struct src_entry { size_t key; hash_map* value; unsigned char used; };
struct dst_entry { size_t key; size_t value; unsigned char used; };
struct hash_map { size_t capacity; size_t count; void* entries; };

hash_map* map_create(size_t entry_size, size_t capacity);
void map_destroy(hash_map* map);
int graph_read_edges(hash_map* map, const edge* edges, size_t num_edges);
int graph_add_connection(hash_map* map, size_t u, size_t v, size_t w);

#endif
