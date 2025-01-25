#include <stdio.h>
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

    enum_covers(graph, cur_c, cur_v);

    return 0;
}