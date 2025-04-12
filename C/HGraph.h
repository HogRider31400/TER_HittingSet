//
// Created by Alex on 25/01/2025.
//

#ifndef HGRAPH_H
#define HGRAPH_H

#include "structures/List.h"
#include <stddef.h>
#define MAX_EDGES 1000000
#define MAX_VERTICES 10000

typedef struct Vertex {
    int id;
    int official_id;
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


typedef struct a_Vertex {
    int id;
    int edges[MAX_EDGES];
    int nb_edges;
} a_Vertex;

typedef struct a_Edge {
    int id;
    int vertices[MAX_VERTICES];
    int nb_vertices;
} a_Edge;

typedef struct a_Graph {
    a_Vertex* vertices[MAX_VERTICES];
    a_Edge* edges[MAX_EDGES];
    int nb_edges;
    int nb_vertices;
} a_Graph;

//Graph avec listes
Vertex* create_vertex();
Edge* create_edge();
Graph* create_graph();
void free_graph(Graph* graph);
void add_edge(Graph* graph, Edge* edge);
void add_vertex(Graph* graph, Vertex* vertex);

//Graph avec tableaux
a_Vertex* create_avertex();
a_Edge* create_aedge();
a_Graph* create_agraph();
void add_aedge(a_Graph* graph, a_Edge* edge);
void add_avertex(a_Graph* graph, a_Vertex* vertex);
void add_edge_to_vertex(a_Graph* graph, int id_vertex, int id_edge);
void add_vertex_to_edge(a_Graph* graph, int id_vertex, int id_edge);

a_Graph* convert_graph_to_agraph(Graph* graph);
#endif //HGRAPH_H
