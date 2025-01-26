//
// Created by Alex on 26/01/2025.
//
#include "berge.h"

//Implémentation de l'opérateur V décrit pare 17 du papier/de la thèse de M. Matthias Hagen
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

int specialized_covers(Graph* graph, iList* vertices,int id_edge) {

    //On nous a donné une couverture et on doit reconstruire les vertices qu'elle couvre
    iList* covered_vertices = create_list();

    for (Node* current = vertices->head; current != NULL; current = current->next) {
        //Pour la continuité du tableau on doit chercher la vertex au bon id, pas forcément au bon emplacement dans le tableau
        Vertex* cur_vertex = NULL;
        for (int i = 0; i < graph->nb_vertices; i++) {
            if (graph->vertices[i]->id == current->value) {
                cur_vertex = graph->vertices[i];
                break;
            }
        }
        if (cur_vertex == NULL) return 0;

        for (Node* current_edge = cur_vertex->edges->head; current_edge != NULL; current_edge = current_edge->next) {
            for (Node* cur = graph->edges[current_edge->value-1]->vertices->head; cur != NULL; cur = cur->next) {
                append_unique(covered_vertices, cur->value);
            }
        }
    }
    //printf("On : "); print_list(vertices); printf("qui couvre : ");print_list(covered_vertices);printf("\n");
    //Une fois fait on fait comme dans covers.
    for (int i = 0; i < graph->nb_vertices; i++) {
        //On fait attention à ce que le vertex actuel ait bien un edge d'id <= id_edge
        int is_fine = 0;
        for (Node* cur = graph->vertices[i]->edges->head; cur != NULL; cur = cur->next) {
            if (cur->value <= id_edge) {
                is_fine = 1;
            }
        }
        if (is_fine == 0) continue;


        if (contains(covered_vertices, graph->vertices[i]->id) == 0) {
            return 0;
        }
    }

    return 1;
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

        //Pour chaque produit de V, on le minimise autant qu'on peut en enlever
        for (NodeList* current = new_covers->head; current != NULL; current = current->next) {
            //printf("On regarde actuellement : "); print_list(current->value); printf("elle donne %d à la couverture \n",(specialized_covers(graph, current->value,i+1)));
            iList* cur_cover = deep_copy(current->value);
            //Tant qu'on trouve un sommet à supprimer, on le fait
            while (1) {
                int found = 0;
                iList* cur_cover_copy = deep_copy(cur_cover);
                for (Node* cur = cur_cover->head; cur != NULL; cur = cur->next) {
                    remove_value(cur_cover_copy, cur->value);
                    if (specialized_covers(graph, cur_cover_copy,i+1) == 1) {
                        found = 1;
                        cur_cover = deep_copy(cur_cover_copy);
                        break;
                    }
                    cur_cover_copy = deep_copy(cur_cover);
                }

                if (found == 0) break;
            }


            if (specialized_covers(graph, cur_cover,i+1) == 0) continue;
            if (has_subset(filtered_covers,cur_cover) == 0) {
                append_list(filtered_covers,cur_cover);
            }
        }
        cur_covers = filtered_covers;
    }

    print_list_list(cur_covers);
}