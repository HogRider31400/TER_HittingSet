//
// Created by Alex on 25/01/2025.
//

#include <stdio.h>
#include "naive.h"

int covers(Graph* graph, iList* vertices) {

    for (int i = 0; i < graph->nb_vertices; i++) {
        if (contains(vertices, graph->vertices[i]->id) == 0) {
            return 0;
        }
    }

    return 1;
}

void enum_covers_recursive(Graph* graph, iList* cur_covered_vertices, iList* cur_used_vertices) {
    if (covers(graph, cur_covered_vertices) == 1) {
        print_list(cur_used_vertices);
        printf(" est un coverage\n");
        return;
    }
    //On détermine à partir de quel indice on commence
    //Tant qu'on a pas vu la dernière arête ajoutée on ne fait rien
    //Si on a pas encore ajouté d'arête on ignore cette partie
    int last_seen = (cur_used_vertices->head != NULL ? 0 : 1);
    int last_used = tail(cur_used_vertices);

    for (int i = 0; i < graph->nb_vertices; i++) {
        if (last_seen == 0) {
            if (graph->vertices[i]->id == last_used) {
                last_seen = 1;
            }
            continue;
        }
        if (contains(cur_used_vertices, graph->vertices[i]->id) == 1) continue;
        iList* new_vertices = deep_copy(cur_covered_vertices);
        for (Node* cur = graph->vertices[i]->edges->head; cur != NULL; cur = cur->next) {
            merge_unique(new_vertices, graph->edges[cur->value-1]->vertices);
        }
        append(cur_used_vertices, graph->vertices[i]->id);
        enum_covers_recursive(graph, new_vertices, cur_used_vertices);
        remove_value(cur_used_vertices,graph->vertices[i]->id);
    }
}

void enum_covers_iterative(Graph* graph) {
    //On init la queue
    Queue* queue = queue_create();
    //On init l'élément initial
    iList* base_covered_vertices = create_list();
    iList* base_used_vertices = create_list();
    queue_add(queue,base_covered_vertices,base_used_vertices);

    //Tant qu'on a des états à explorer
    while (queue->head != NULL) {
        QueueElem* cur = queue_pop(queue);

        if (covers(graph, cur->covered_vertices) == 1) {
            print_list(cur->used_vertices);
            printf(" est un coverage\n");
            continue;
        }

        //On détermine à partir de quel indice on commence
        //Tant qu'on a pas vu le dernier sommet ajouté on ne fait rien
        //Si on a pas encore ajouté d'arête on ignore cette partie
        int last_seen = (cur->used_vertices->head != NULL ? 0 : 1);
        int last_used = tail(cur->used_vertices);

        for (int i = 0; i < graph->nb_vertices; i++) {
            //On vérifie qu'on n'a pas encore vu le dernier sommet
            if (last_seen == 0) {
                if (graph->vertices[i]->id == last_used) {
                    last_seen = 1;
                }
                continue;
            }
            if (contains(cur->used_vertices, graph->vertices[i]->id) == 1) continue;
            iList* new_vertices = deep_copy(cur->covered_vertices);
            iList* new_used = deep_copy(cur->used_vertices);

            for (Node* cur = graph->vertices[i]->edges->head; cur != NULL; cur = cur->next) {
                merge_unique(new_vertices, graph->edges[cur->value-1]->vertices);
            }
            append(new_used, graph->vertices[i]->id);
            queue_add(queue,new_vertices,new_used);
        }
    }

}