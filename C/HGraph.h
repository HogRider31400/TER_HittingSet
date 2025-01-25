//
// Created by Alex on 25/01/2025.
//

#ifndef HGRAPH_H
#define HGRAPH_H

#include "List.h"
#include <stddef.h>
#define MAX_EDGES 100
#define MAX_VERTICES 100

typedef struct Vertex {
    int id;
    iList* edges;
} Vertex;

typedef struct Edge {
    int id;
    iList* vertices;
} Edge;

typedef struct Graph {
    Vertex* vertices[MAX_VERTICES];
    Edge* edges[MAX_EDGES];
    int nb_edges;
    int nb_vertices;
} Graph;

Vertex* create_vertex();
Edge* create_edge();
Graph* create_graph();
void free_graph(Graph* graph);
void add_edge(Graph* graph, Edge* edge);
void add_vertex(Graph* graph, Vertex* vertex);

#endif //HGRAPH_H
