//
// Created by Alex on 25/01/2025.
//

#include <stdio.h>
#include "naive.h"
int covers(Graph* graph, iList* vertices) {

    for (int i = 0; i < graph->nb_vertices; i++) {
        if (graph->vertices[i] == NULL) continue;
        if (contains(vertices, graph->vertices[i]->id) == 0) {
            return 0;
        }
    }

    return 1;
}

void enum_covers(Graph* graph, iList* cur_covered_vertices, iList* cur_used_vertices) {
    //print_list(cur_covered_vertices);
    //printf("\n");
    if (covers(graph, cur_covered_vertices) == 1) {
        print_list(cur_used_vertices);
        printf(" est un coverage\n");
        return;
    }

    for (int i = 0; i < graph->nb_vertices; i++) {
        if (graph->vertices[i] == NULL) continue;
        if (contains(cur_used_vertices, graph->vertices[i]->id) == 1) continue;
        /*printf("Avant deepcopy");
        print_list(cur_covered_vertices);
        printf("\n"); */
        iList* new_vertices = deep_copy(cur_covered_vertices);
        /*printf("Après deepcopy");
        print_list(new_vertices);
        printf("\n"); */
        for (Node* cur = graph->vertices[i]->edges->head; cur != NULL; cur = cur->next) {
            if (graph->edges[i] == NULL) continue;
            /*printf("On a :");
            print_list(new_vertices);
            printf(", on veut y ajouter :");
            print_list(graph->edges[cur->value-1]->vertices);
            printf("\n"); */
            merge_unique(new_vertices, graph->edges[cur->value-1]->vertices);
            /*p0rintf("ça donne :");
            print_list(new_vertices);
            printf("\n");*/
        }
        /*printf("aled : ");
        print_list(cur_used_vertices);
        printf("\n"); */
        append(cur_used_vertices, graph->vertices[i]->id);
        /*printf("aled2 :");
        print_list(cur_used_vertices);
        printf("\n"); */
        enum_covers(graph, new_vertices, cur_used_vertices);
        /*printf("On remove %d ", graph->vertices[i]->id);
        print_list(cur_used_vertices);
        printf("\n"); */
        remove_value(cur_used_vertices,graph->vertices[i]->id);
        //print_list(cur_used_vertices);
        //printf("\n");

    }

}