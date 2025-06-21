#include <stdlib.h>
#include <stdio.h>
#include "edge.h"

void destroy_edge(struct Edge edge){
	if(edge.from != NULL) free(edge.from);
	if(edge.to != NULL) free(edge.to);
}

void destroy_edge_array(struct Edge* edges, int num_edges){
	if(edges == NULL) {
		perror("provided struct Edge* was null");
		exit(EXIT_FAILURE); // handle gracefully
	}
	for(int i = 0; i < num_edges; i++){
		destroy_edge(edges[i]);
	}
}
