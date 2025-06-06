#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <string.h>
#include "algorithms/algos.h"
#include "graph_reader.h"
#include "structures/Queue.h"
#include "structures/Bitmap.h"

iListList* globalTransversals = NULL;
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

double test_launcher(char* algo, Graph* graph, a_Graph* agraph) {
    if (strcmp(algo,"naive_recursive") == 0) return chrono_func(launch_naive_empty, graph);
        if (strcmp(algo,"naive_iterative") == 0) return chrono_func(enum_covers_iterative, graph);
        if (strcmp(algo,"berge") == 0) return chrono_func(berge_algorithm, graph);
        if (strcmp(algo, "naive_iterative_array") == 0) return chrono_func(enum_covers_iterative_array, graph);
        if (strcmp(algo, "dong_li") == 0) return chrono_func(DL_algorithm, graph);
        if (strcmp(algo, "naive_iterative_array_2") == 0) return chrono_func_2(enum_covers_iterative_array_2, agraph);
        if (strcmp(algo, "berge_bitmap") == 0) return chrono_func(berge_bitmap_algorithm, graph);
        if (strcmp(algo, "DL_opti") == 0) return chrono_func(DL_opti, graph);
        if (strcmp(algo, "DL_singl") == 0) return chrono_func(DL_singl, graph);
        if (strcmp(algo, "DL_Trgf") == 0) return chrono_func(DL_Trgf, graph);
        if (strcmp(algo, "DL_Trgn") == 0) return chrono_func(DL_Trgn, graph);
        if (strcmp(algo, "DL_fopti") == 0) return chrono_func(DL_fopti, graph);
        if (strcmp(algo, "DL_foptiv") == 0) return chrono_func(DL_foptiv, graph);
}

int main(int argc, char *argv[]) {
    /*bitmap_list* bitmaps = create_bitmap_list();

    bitmap* b = create_bitmap(1);
    add_bitmap(bitmaps, copy_bitmap(b));

    set_bit(b,9,1);
    add_bitmap(bitmaps, copy_bitmap(b));
    //print_bitmap(b);
    set_bit(b,9,0);
    add_bitmap(bitmaps, copy_bitmap(b));
    //print_bitmap(b);
    set_bit(b,9,1);
    add_bitmap(bitmaps, copy_bitmap(b));
    set_bit(b,0,1);
    add_bitmap(bitmaps, copy_bitmap(b));
    set_bit(b,4,1);
    add_bitmap(bitmaps, copy_bitmap(b));

    bitmap* b2 = create_bitmap(1);
    set_bit(b2,0, 1);
    set_bit(b2,1, 1);


    //print_bitmap(bitmap_and(b,b2));
    print_bitmaps(bitmaps);
    print_as_ints(bitmaps);
    printf("%d\n", b->nb_bits);
    bubble_sort_bitmap(bitmaps);
    print_bitmaps(bitmaps);
    print_as_ints(bitmaps);
    fflush(stdout);
     */
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
        time_spent = test_launcher(cur, graph, agraph);

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