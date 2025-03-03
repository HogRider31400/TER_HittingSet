//
// Created by Alex on 25/01/2025.
//

#include <stdio.h>
#include "naive.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

//Ici on regarde si l1 contient l2, donc si l2 C l1
int is_subset(iList* l1, iList* l2) {
    Node* current = l2->head;

    while (current != NULL) {
        if (contains(l1, current->value) == 0) return 0;;
        current = current->next;
    }

    return 1;
}

//Maintenant on regarde si la famille list contient un élément qui est un sous ensemble de slist
int has_subset(iListList* list, iList* slist) {
    NodeList* current = list->head;
    while (current != NULL) {
        if (is_subset(slist, current->value) == 1) return 1;
        current = current->next;
    }
    return 0;
}

int covers(Graph* graph, iList* edges) {

    if (graph->nb_edges == edges->size) return 1;

    return 0;
}

void enum_covers_recursive(Graph* graph, iList* cur_covered_edges, iList* cur_used_vertices, iListList* all_covers) {
    //printf("%d %d\n",graph->nb_vertices, cur_covered_vertices->size);
    if (covers(graph, cur_covered_edges) == 1) {

        //Check de minimalité
        int min = 1;
        for (Node* cur = cur_used_vertices->head; cur != NULL; cur = cur->next) {

            iList* new_covered =  create_list();

            for (Node* current = cur_used_vertices->head; current != NULL; current = current->next) {
                if (current->value == cur->value) continue;
                Vertex* cur_vertex = NULL;
                for (int i = 0; i < graph->nb_vertices; i++) {
                    if (graph->vertices[i]->id == current->value) {
                        cur_vertex = graph->vertices[i];
                        break;
                    }
                }

                for (Node* current_edge = cur_vertex->edges->head; current_edge != NULL; current_edge = current_edge->next) {
                    append_unique(new_covered, current_edge->value);
                }
            }

            if (covers(graph, new_covered)) {
                min = 0;
                break;
            }
        }

        if (!has_subset(all_covers, cur_used_vertices) && min==1) {
            append_list(all_covers, deep_copy(cur_used_vertices));
        }
        //printf("Affichage coverage\n");
        //print_list(cur_used_vertices);
        //printf("\nFin\n");
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
        iList* new_edges = deep_copy(cur_covered_edges);
        for (Node* cur = graph->vertices[i]->edges->head; cur != NULL; cur = cur->next) {
            //merge_unique(new_vertices, graph->edges[cur->value-1]->vertices);
            append_unique(new_edges, cur->value);
        }
        append(cur_used_vertices, graph->vertices[i]->id);
        enum_covers_recursive(graph, new_edges, cur_used_vertices, all_covers);
        remove_value(cur_used_vertices,graph->vertices[i]->id);
    }
}

void enum_covers_iterative(Graph* graph) {
    //On init la queue
    //List contenant toutes les couvertures minimales
    iListList* all_covers = create_list_list();

    Queue* queue = queue_create();
    //On init l'élément initial
    iList* base_covered_edges = create_list();
    iList* base_used_vertices = create_list();
    queue_add(queue,base_covered_edges,base_used_vertices);

    //Tant qu'on a des états à explorer
    while (queue->head != NULL) {
        QueueElem* cur = queue_pop(queue);

        if (covers(graph, cur->covered_edges) == 1) {
            //print_list(cur->used_vertices);
            //On vérifie qu'on en casse pas la minimalité
            if (has_subset(all_covers, cur->used_vertices) == 0) {
                append_list(all_covers, deep_copy(cur->used_vertices));
            }
            //printf(" est un coverage\n");
            //printf("On a \n");
            //print_list_list(all_covers);
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
            iList* new_edges = deep_copy(cur->covered_edges);
            iList* new_used = deep_copy(cur->used_vertices);

            for (Node* cur = graph->vertices[i]->edges->head; cur != NULL; cur = cur->next) {
                //merge_unique(new_vertices, graph->edges[cur->value-1]->vertices);
                append_unique(new_edges, cur->value);
            }
            append(new_used, graph->vertices[i]->id);
            queue_add(queue,new_edges,new_used);
        }
        free_list(cur->used_vertices);
        free_list(cur->covered_edges);
        free(cur);
    }
    //printf("Affichage coverage");
    print_list_list(all_covers);
    //printf("Fin\n");
}


void enum_covers_iterative_array(Graph* graph) {
    //On init la queue
    //List contenant toutes les couvertures minimales
    //printf("saluting...\n");
    iListList* all_covers = create_list_list();
    //printf("saluting...\n");
    QueueO* queue = queue_o_create();
    //printf("saluting...\n");
    //On init l'élément initial
    iList* base_used_vertices = create_list();
    short int* covered_edges = (short int*)malloc(sizeof(short int) * (graph->nb_edges+1));
    for (int i = 0; i < graph->nb_edges; i++) {
        covered_edges[i] = 0;
    }

    //printf("saluting...\n");

    queue_o_add(queue,covered_edges,base_used_vertices,0,0);
    //printf("saluting.23..\n");
    //Tant qu'on a des états à explorer
    while (queue->head != NULL) {
        QueueOElem* cur = queue_o_pop(queue);
        //for (int i = 0; i < cur->max_covered; i++) {
        //    printf("%d ",cur->covered_edges[i]);
        //}
        //printf("\n");
        if (cur->nb_covered == graph->nb_edges) {//covers(graph, cur->covered_edges) == 1) {
            //print_list(cur->used_vertices);
            //On vérifie qu'on en casse pas la minimalité
            if (has_subset(all_covers, cur->used_vertices) == 0) {
                append_list(all_covers, deep_copy(cur->used_vertices));
            }
            //printf(" est un coverage\n");
            //printf("On a \n");
            //print_list_list(all_covers);
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
            short int * new_edges = (short int*)malloc(sizeof(short int) *  (graph->nb_edges+1));;
            int new_covered = cur->nb_covered;
            int max_covered = cur->max_covered;
            memcpy(new_edges, cur->covered_edges, sizeof(short int) *  (graph->nb_edges+1));
            iList* new_used = deep_copy(cur->used_vertices);
            //long long int new_mask = cur->used_mask;

            for (Node* cur = graph->vertices[i]->edges->head; cur != NULL; cur = cur->next) {
                //merge_unique(new_vertices, graph->edges[cur->value-1]->vertices);
                //append_unique(new_edges, cur->value);
                //new_mask = new_mask | (1 << (cur->value-1));
                if (new_edges[cur->value-1] == 0) {
                    new_edges[cur->value-1] = 1;
                    new_covered++;
                    if (cur->value-1 > max_covered) {
                        max_covered = cur->value-1;
                    }

                }

            }
            append(new_used, graph->vertices[i]->id);
            queue_o_add(queue,new_edges,new_used, new_covered, max_covered);

        }
        free(cur->covered_edges);
        free_list(cur->used_vertices);
        free(cur);
    }
    //printf("Affichage coverage");
    print_list_list(all_covers);
    //printf("Fin\n");

}

//Version où le graph est une array aussi
void enum_covers_iterative_array_2(a_Graph* graph) {
    //On init la queue
    //List contenant toutes les couvertures minimales
    iListList* all_covers = create_list_list();

    QueueO* queue = queue_o_create();
    //printf("saluting...\n");
    //On init l'élément initial
    iList* base_used_vertices = create_list();
    short int* covered_edges = (short int*)malloc(sizeof(short int) * (graph->nb_edges+1));
    for (int i = 0; i < graph->nb_edges; i++) {
        covered_edges[i] = 0;
    }

    queue_o_add(queue,covered_edges,base_used_vertices,0,0);

    //Tant qu'on a des états à explorer
    while (queue->head != NULL) {
        QueueOElem* cur = queue_o_pop(queue);
        //print_list(cur->used_vertices); printf("\n");
        //fflush(stdout);
        //printf("%d %f %d \n", cur->used_mask, pow(2, graph->nb_edges)-1, graph->nb_edges);
        if (cur->nb_covered == graph->nb_edges) {//covers(graph, cur->covered_edges) == 1) {
            //print_list(cur->used_vertices);
            //On vérifie qu'on en casse pas la minimalité
            if (has_subset(all_covers, cur->used_vertices) == 0) {
                append_list(all_covers, deep_copy(cur->used_vertices));
            }
            //printf(" est un coverage\n");
            //printf("On a \n");
            //print_list_list(all_covers);
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
            //printf("%d \n",i );
            if (contains(cur->used_vertices, graph->vertices[i]->id) == 1) continue;
            short int * new_edges = (short int*)malloc(sizeof(short int) *  (graph->nb_edges+1));;
            int new_covered = cur->nb_covered;
            int max_covered = cur->max_covered;
            memcpy(new_edges, cur->covered_edges, sizeof(short int) *  (graph->nb_edges+1));
            iList* new_used = deep_copy(cur->used_vertices);
            //long long int new_mask = cur->used_mask;

            for (int j = 0; j < graph->vertices[i]->nb_edges; j++) {
                int id = graph->vertices[i]->edges[j];
                //merge_unique(new_vertices, graph->edges[cur->value-1]->vertices);
                //append_unique(new_edges, cur->value);
                //new_mask = new_mask | (1 << (cur->value-1));
                if (new_edges[id-1] == 0) {
                    new_edges[id-1] = 1;
                    new_covered++;
                    if (id-1 > max_covered) {
                        max_covered = id-1;
                    }
                }
            }
            //print_list(new_used);
            //printf("%d \n", graph->vertices[i]->id);
            //fflush(stdout);
            append(new_used, graph->vertices[i]->id);
            queue_o_add(queue,new_edges,new_used, new_covered, max_covered);

        }
        free(cur->covered_edges);
        free_list(cur->used_vertices);
        free(cur);
    }
    //printf("Affichage coverage");
    print_list_list(all_covers);
    //printf("Fin\n");
}


void enum_covers_iterative_a(Graph* graph) {
    //On init la queue
    //List contenant toutes les couvertures minimales
    iListList* all_covers = create_list_list();

    QueueA* queue = queue_a_create();
    //On init l'élément initial
    iList* base_covered_vertices = create_list();
    iList* base_used_vertices = create_list();
    queue_a_add(queue,base_covered_vertices,base_used_vertices);

    //Tant qu'on a des états à explorer
    while (queue->cur_size != 0) {
        QueueAElem cur = queue_a_pop(queue);
        //printf("aled\n");
        if (covers(graph, cur.covered_vertices) == 1) {
            print_list(cur.used_vertices);
            //On vérifie qu'on en casse pas la minimalité
            if (has_subset(all_covers, cur.used_vertices) == 0) {
                append_list(all_covers, deep_copy(cur.used_vertices));
            }
            //printf(" est un coverage\n");
            //printf("On a \n");
            //print_list_list(all_covers);
            continue;
        }
        //On détermine à partir de quel indice on commence
        //Tant qu'on a pas vu le dernier sommet ajouté on ne fait rien
        //Si on a pas encore ajouté d'arête on ignore cette partie
        int last_seen = (cur.used_vertices->head != NULL ? 0 : 1);
        int last_used = tail(cur.used_vertices);
        for (int i = 0; i < graph->nb_vertices; i++) {
            //On vérifie qu'on n'a pas encore vu le dernier sommet
            if (last_seen == 0) {
                if (graph->vertices[i]->id == last_used) {
                    last_seen = 1;
                }
                continue;
            }
            if (contains(cur.used_vertices, graph->vertices[i]->id) == 1) continue;
            iList* new_vertices = deep_copy(cur.covered_vertices);
            iList* new_used = deep_copy(cur.used_vertices);

            for (Node* cur = graph->vertices[i]->edges->head; cur != NULL; cur = cur->next) {
                merge_unique(new_vertices, graph->edges[cur->value-1]->vertices);
            }
            append(new_used, graph->vertices[i]->id);
            queue_a_add(queue,new_vertices,new_used);
            //printf("aaaaa\n");
        }
    }
    //printf("Affichage coverage");
    print_list_list(all_covers);
    //printf("Fin\n");
}
