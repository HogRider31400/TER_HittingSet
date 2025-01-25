#include <stdio.h>
#include <time.h>
#include <windows.h>
#include "naive.c"
#include "graph_reader.c"

int main(void) {

    Graph* graph = read_graph_from_file("./example.txt");

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

    iList* cur_c = create_list();
    iList* cur_v = create_list();

    //Ici on s'occupe de chronométrer la fonction en utilisant l'API de Windows
    //QueryPerformanceCounter nous permet de récupérer le nombre de ticks effectués, et QueryPerformanceFrequency nous permet de savoir combien de ticks sont effectués par seconde, afin de pouvoir restituer le calcul
    //Pour plus de renseignements :
    //https://learn.microsoft.com/fr-fr/windows/win32/api/profileapi/nf-profileapi-queryperformancefrequency
    //https://learn.microsoft.com/fr-fr/windows/win32/api/profileapi/nf-profileapi-queryperformancecounter
    LARGE_INTEGER start, end, freq;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    enum_covers(graph, cur_c, cur_v);

    QueryPerformanceCounter(&end);
    double time_taken = (end.QuadPart - start.QuadPart) / (double)freq.QuadPart;
    printf("Time taken: %f seconds\n", time_taken);


    return 0;
}