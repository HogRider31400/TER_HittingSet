#include <stdio.h>
#include <time.h>
#include <windows.h>
#include "algorithms/naive.h"
#include "graph_reader.h"
#include "structures/Queue.h"
#include "algorithms/berge.h"
#include "algorithms/DL.h"
#include <string.h>
#include "algorithms/BMR.h"

extern iListList* globalTransversals;
//Prend en paramètre une fonction qui prend un graphe un paramètre et l'appelle pour la chronométrer
double chrono_func( void func(Graph*), Graph* graph) {
    LARGE_INTEGER start, end, freq;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    func(graph);

    QueryPerformanceCounter(&end);
    return (end.QuadPart - start.QuadPart) / (double)freq.QuadPart;
}

double chrono_func_2( void func(a_Graph*), a_Graph* graph) {
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
    iListList* covers = create_list_list();
    enum_covers_recursive(graph, cur_c, cur_v, covers);
    //Temps d'affichage aussi pris en compte, overhead à supprimer par la suite
    //printf("Affichage coverage\n");
    print_list_list(covers);
    //printf("Fin\n");
}

void launch_bmr_empty(Graph* graph) {
    iList* Vpartition = create_list();
    BMR_algorithm(graph, Vpartition);
    free_list(Vpartition);
}

int main(int argc, char *argv[]) {
    char test_case[] = "./data/example.txt";
    fflush(stdout);
    if (argc == 3) {
        char* cur = argv[2];
        strcpy(test_case, cur);
    }

    Graph* graph = read_graph_from_file(test_case);
    a_Graph* agraph = convert_graph_to_agraph(graph);
    //printf("%s \n", test_case);
    double time_spent = 0;
    if (argc >= 2) {
        char* cur = argv[1];
        if (strcmp(cur,"naive_recursive") == 0) time_spent = chrono_func(launch_naive_empty, graph);
        if (strcmp(cur,"naive_iterative") == 0) time_spent = chrono_func(enum_covers_iterative, graph);
        if (strcmp(cur,"berge") == 0) time_spent = chrono_func(berge_algorithm, graph);
        if (strcmp(cur, "naive_iterative_array") == 0) time_spent = chrono_func(enum_covers_iterative_array, graph);
        if (strcmp(cur, "dong_li") == 0) time_spent = chrono_func(DL_algorithm, graph);
        if (strcmp(cur, "naive_iterative_array_2") == 0) time_spent = chrono_func_2(enum_covers_iterative_array_2, agraph);
        if (strcmp(cur, "bmr") == 0) {
            globalTransversals = create_list_list();
            time_spent = chrono_func(launch_bmr_empty, graph);
            print_list_list(globalTransversals);
            free_list_list(globalTransversals);
        };
        //printf("%s\n", cur);
    }


    /*
    printf("edges et vertices : %d %d\n",graph->nb_edges,graph->nb_vertices);
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
    }*/

    //DL_algorithm(graph);
    free_graph(graph);

    printf("Time\n");
    printf("%f", time_spent);
    //printf("Time taken: %f seconds\n", time_spent);

    return 0;
}