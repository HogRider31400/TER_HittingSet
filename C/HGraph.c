//
// Created by Alex on 25/01/2025.
//
//#include "List.c"
#include "structures/List.h"
#include "HGraph.h"
#include <stdlib.h>

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



//Graph avec arrays

a_Vertex* create_avertex() {
    a_Vertex* vertex = (a_Vertex*)malloc(sizeof(a_Vertex));
    vertex->id = -1;
    vertex->nb_edges = 0;
    return vertex;
}

a_Edge* create_aedge() {
    a_Edge* edge = (a_Edge*)malloc(sizeof(a_Edge));
    edge->id = -1;
    edge->nb_vertices = 0;
    return edge;
}

// Fonction pour créer un nouveau graphe
a_Graph* create_agraph() {
    a_Graph* graph = (a_Graph*)malloc(sizeof(a_Graph));
    graph->nb_edges = 0;
    graph->nb_vertices = 0;
    return graph;
}

void add_aedge(a_Graph* graph, a_Edge* edge) {
    graph->edges[graph->nb_edges] = edge;
    graph->nb_edges++;
}

void add_avertex(a_Graph* graph, a_Vertex* vertex) {
    graph->vertices[graph->nb_vertices] = vertex;
    graph->nb_vertices++;
}

void add_edge_to_vertex(a_Graph* graph, int id_vertex, int id_edge) {
    a_Vertex* vertex = graph->vertices[id_vertex];
    vertex->edges[vertex->nb_edges] = id_edge;
    vertex->nb_edges++;
}

void add_vertex_to_edge(a_Graph* graph, int id_vertex, int id_edge) {
    a_Edge* edge = graph->edges[id_edge];
    edge->vertices[edge->nb_vertices] = id_vertex;
    edge->nb_vertices++;
}