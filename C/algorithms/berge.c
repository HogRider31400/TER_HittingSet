//
// Created by Alex on 26/01/2025.
//
#include "berge.h"

#include <stdio.h>

//Implémentation de l'opérateur V décrit page 17 du papier/de la thèse de M. Matthias Hagen
iListList* V_operator(iListList* l1, iListList* l2) {
    iListList* result = create_list_list();

    //On rajoute l'union des éléments non nuls
    for (NodeList* current_l1 = l1->head; current_l1 != NULL; current_l1 = current_l1->next) {
        for (NodeList* current_l2 = l2->head; current_l2 != NULL; current_l2 = current_l2->next) {
            iList* cur_result = deep_copy(current_l1->value);

            for (Node* current_in_current_l2 = current_l2->value->head; current_in_current_l2 != NULL; current_in_current_l2 = current_in_current_l2-> next) {
                if (contains(cur_result,current_in_current_l2->value) == 0)
                    append(cur_result, current_in_current_l2->value);
            }
            append_list(result,cur_result);
        }
    }

    //for (NodeList* current_l2 = l2->head; current_l2 != NULL; current_l2 = current_l2->next) {
    //    append_list(result, deep_copy(current_l2->value));
    //}
    //for (NodeList* current_l1 = l1->head; current_l1 != NULL; current_l1 = current_l1->next) {
    //    append_list(result, deep_copy(current_l1->value));
    //}

    return result;
}

iListList* create_edge_list_list(Graph* graph, int id_edge) {
    iListList* res = create_list_list();
    //On l'init avec toutes les vertices
    for (Node* current = graph->edges[id_edge]->vertices->head; current != NULL; current = current->next) {
        iList* cur = create_list();
        append(cur, current->value);
        append_list(res, cur);
    }
    return res;
}


void berge_algorithm(Graph* graph) {
    //le cur Tr qu'on build
    iListList* cur_covers = create_edge_list_list(graph, 0);
    //print_list_list(cur_covers);
    for (int i = 1; i < graph->nb_edges; i++) {
        //print_list_list(cur_covers);
        //On construit {{v} : v ∈ ei}
        iListList* vertices_list = create_edge_list_list(graph, i);

        //On applique V
        iListList* new_covers = V_operator(cur_covers, vertices_list);
        //On doit maintenant faire le tri, on commence par les mettre dans l'ordre
        //Puis on regarde
        iListList* filtered_covers = create_list_list();

        bubble_sort_list(new_covers);

        //Pour chaque produit de V, on le minimise autant qu'on peut en enlever
        for (NodeList* current = new_covers->head; current != NULL; current = current->next) {
            //printf("On regarde actuellement : "); print_list(current->value); printf("elle donne %d à la couverture \n",(specialized_covers(graph, current->value,i+1)));
            iList* cur_cover = deep_copy(current->value);

            //Si c'est pas une couverture on fait rien
            //print_list_list(filtered_covers); print_list(cur_cover); printf(" : %d %d\n",has_subset(filtered_covers,cur_cover), specialized_covers(graph, cur_cover,i+1));
            //printf("cur\n");
            if (has_subset(filtered_covers,cur_cover) == 0) {
                append_list(filtered_covers,cur_cover);
            }
        }
        cur_covers = filtered_covers;
    }

    print_list_list(cur_covers);
}