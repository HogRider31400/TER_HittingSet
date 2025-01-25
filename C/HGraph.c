//
// Created by Alex on 25/01/2025.
//
//#include "List.c"
#include "List.c"
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

Vertex* create_vertex(){
    Vertex* vertex = malloc(sizeof(Vertex));
    vertex->id = NULL;
    vertex->edges = create_list();
    return vertex;
}
Edge* create_edge(){
    Edge* edge= malloc(sizeof(Edge));
    edge->id = NULL;
    edge->vertices = create_list();
    return edge;
}

Graph* create_graph(){ //potentiellement ajouter vertices et edges en param pour que l'init se fasse fluidement ?
    Graph* graph = malloc(sizeof(Graph)); //je sais plus si il faut cast
    graph->nb_edges = 0;
    graph->nb_vertices = 0;
    for (int i = 0; i < MAX_VERTICES; i++) {
        graph->vertices[i] = NULL;
    }
    for (int i = 0; i < MAX_EDGES; i++) {
        graph->edges[i] = NULL;
    }
    return graph;
}

void free_graph(Graph* graph) {
    for(int i = 0; i < graph->nb_edges; i++) {
        free(graph->edges[i]);
    }
    free(graph->edges);
    for(int i = 0; i < graph->nb_vertices; i++) {
        free(graph->vertices[i]);
    }
    free(graph->vertices);
    free(graph);
}

void add_edge(Graph* graph, Edge* edge) {
    graph->edges[graph->nb_edges] = edge;
    graph->nb_edges++;
}
void add_vertex(Graph* graph, Vertex* vertex) {
    graph->vertices[graph->nb_vertices] = vertex;
    graph->nb_vertices++;
}
