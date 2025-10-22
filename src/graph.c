#include <stddef.h>
#include <stdlib.h>
#include <graph.h>
#include <errors.h>

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

int graph_load_edges(hash_map* map, const char* file_path) {
    return ERR_NONE;
}

int graph_add_connection(hash_map* graph, size_t u, size_t v, size_t w) {
    if (!graph) { return ERR_INPUT; }
    // TODO use map_get_or_create
    return ERR_NONE;
}
