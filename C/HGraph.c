//
// Created by Alex on 25/01/2025.
//
//#include "List.c"
#include "structures/List.h"
#include "HGraph.h"
#include <stdlib.h>

Vertex* create_vertex(){
    Vertex* vertex = malloc(sizeof(Vertex));
    vertex->id = -1;
    vertex->edges = create_list();
    return vertex;
}
Edge* create_edge(){
    Edge* edge= malloc(sizeof(Edge));
    edge->id = -1;
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
    // Libérer les arêtes et leurs listes de sommets
    for (int i = 0; i < graph->nb_edges; i++) {
        if (graph->edges[i]) {
            free_list(graph->edges[i]->vertices);
            free(graph->edges[i]);
        }
    }

    // Libérer les sommets et leurs listes d'arêtes
    for (int i = 0; i < graph->nb_vertices; i++) {
        if (graph->vertices[i]) {
            free_list(graph->vertices[i]->edges);
            free(graph->vertices[i]);
        }
    }

    // Libérer le graphe lui-même
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

a_Graph* convert_graph_to_agraph(Graph* graph) {
    if (!graph) {
        return NULL;
    }

    a_Graph* agraph = create_agraph();

    // Convertir les sommets
    for (int i = 0; i < MAX_VERTICES; i++) {
        if (graph->vertices[i]) {
            a_Vertex* avertex = create_avertex();
            avertex->id = graph->vertices[i]->id;

            // Convertir la liste d'arêtes en tableau
            int edge_idx = 0;
            for (Node* edge_node = graph->vertices[i]->edges->head;
                 edge_node != NULL && edge_idx < MAX_EDGES;
                 edge_node = edge_node->next) {
                avertex->edges[edge_idx] = edge_node->value;
                edge_idx++;
                 }
            avertex->nb_edges = edge_idx;

            // Ajouter le sommet au bon indice dans agraph
            agraph->vertices[i] = avertex;
            agraph->nb_vertices = (i + 1 > agraph->nb_vertices) ? i + 1 : agraph->nb_vertices;
        }
    }

    // Convertir les arêtes
    for (int i = 0; i < graph->nb_edges; i++) {
        if (graph->edges[i]) {
            a_Edge* aedge = create_aedge();
            aedge->id = graph->edges[i]->id;

            // Convertir la liste de sommets en tableau
            int vertex_idx = 0;
            for (Node* vertex_node = graph->edges[i]->vertices->head;
                 vertex_node != NULL && vertex_idx < MAX_VERTICES;
                 vertex_node = vertex_node->next) {
                aedge->vertices[vertex_idx] = vertex_node->value;
                vertex_idx++;
                 }
            aedge->nb_vertices = vertex_idx;

            // Ajouter l'arête au graphe
            agraph->edges[i] = aedge;
            agraph->nb_edges++;
        }
    }

    return agraph;
}