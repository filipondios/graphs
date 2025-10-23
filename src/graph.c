#include "hash_map.h"
#include <graph.h>
#include <errors.h>

int graph_read_edges(hash_map* map, const edge* edges, size_t num_edges) {
    for (size_t i = 0; i < num_edges; ++i) {
        const edge* e = &edges[i];
        const int ruv = graph_add_connection(map, e->u, e->v, e->w);
        const int rvu = graph_add_connection(map, e->v, e->u, e->w);
        if (ruv == ERR_ALLOC || rvu == ERR_ALLOC) { return ERR_ALLOC; }
    }
    return ERR_NONE;
}

int graph_load_edges(hash_map* map, const char* file_path) {
    return ERR_NONE;
}

int graph_add_connection(hash_map* graph, size_t u, size_t v, size_t w) {
    src_entry* source_entry = map_get_or_create_entry(graph, u,
        sizeof(src_entry), sizeof(hash_map*));
    if (!source_entry) { return ERR_ALLOC;  }

    if (!source_entry->value) {
        // If the entry was just created, initialize neighbours
        source_entry->value = map_create(sizeof(dst_entry), 0);
        if (!source_entry->value) { return ERR_ALLOC; }
    }

    dst_entry* neighbours_entry = map_get_or_create_entry(source_entry->value,
        v, sizeof(dst_entry), sizeof(size_t));
    if (!neighbours_entry) { return ERR_ALLOC; }

    neighbours_entry->value = w;
    return ERR_NONE;
}
