#ifndef EDGE_H
#define EDGE_H
#include "vertex.h"
struct Edge {
	struct Vertex* from;
	struct Vertex* to;
};

void destroy_edge(struct Edge edge);
void destroy_edge_array(struct Edge* edges, int num_edges);
#endif
