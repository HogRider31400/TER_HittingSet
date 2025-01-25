#include <stdio.h>
#include <time.h>
#include <windows.h>
#include "naive.h"
#include "graph_reader.h"
//Prend en paramètre une fonction qui prend un graphe un paramètre et l'appelle pour la chronométrer
double chrono_func( void func(Graph*), Graph* graph) {
    LARGE_INTEGER start, end, freq;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    func(graph);

    QueryPerformanceCounter(&end);
    return (end.QuadPart - start.QuadPart) / (double)freq.QuadPart;
}

//Voici un exemple de définition d'une fonction pour appeler le chronométrage, on fait le travail préliminaire pour définir les paramètres supplémentaires puis on appelle la fonction
void launch_naive_empty(Graph* graph) {
    iList* cur_c = create_list();
    iList* cur_v = create_list();

    enum_covers(graph, cur_c, cur_v);
}

int main(void) {
    printf("alo\n");
    Graph* graph = read_graph_from_file("./data/example.txt");
    printf("start\n");
    printf("nbs : %d %d\n",graph->nb_edges,graph->nb_vertices);
    for (int i = 0; i < graph->nb_vertices;i++) {
        printf("Noeud %d : ", graph->vertices[i]->id);
        print_list(graph->vertices[i]->edges);
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < graph->nb_edges; i++) {
        printf("Arete %d : ", graph->edges[i]->id);
        print_list(graph->edges[i]->vertices);
        printf("\n");
    }


    printf("Time taken: %f seconds\n", chrono_func(launch_naive_empty, graph));


    return 0;
}