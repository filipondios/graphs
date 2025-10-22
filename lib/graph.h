#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <hash_map.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct edge edge;
typedef struct src_entry src_entry;
typedef struct dst_entry dst_entry;
typedef struct hash_map graph;

struct src_entry { size_t key; hash_map* value; unsigned char used; };
struct dst_entry { size_t key; size_t value; unsigned char used; };
struct edge { size_t u; size_t v; size_t w; };

int graph_read_edges(hash_map* map, const edge* edges, size_t num_edges);
int graph_load_edges(hash_map* map, const char* file_path);
int graph_add_connection(hash_map* map, size_t u, size_t v, size_t w);

#ifdef __cplusplus
}
#endif
#endif
