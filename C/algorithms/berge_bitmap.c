//
// Created by Alex on 13/03/2025.
//
#include "berge_bitmap.h"

#include <stdio.h>

#include "naive.h"


bitmap_list* bV_operator(bitmap_list* b1, iList* vertices) {
    bitmap_list* result_no_new = create_bitmap_list();
    bitmap_list* result_with_new = create_bitmap_list();

    //On rajoute l'union des éléments non nuls
    for (bitmap_node* current_b1 = b1->head; current_b1 != NULL; current_b1 = current_b1->next) {
        for (Node* current_v = vertices->head; current_v != NULL; current_v = current_v->next) {
            //bitmap* cur_result = bitmap_or(current_b1->value, current_v->value);

            if (get_bit(current_b1->value, current_v->value-1))
                add_bitmap(result_no_new, copy_bitmap(current_b1->value));
            else {
                bitmap* cur_result = copy_bitmap(current_b1->value);
                set_bit(cur_result,current_v->value-1,1);
                cur_result->nb_bits++;
                add_bitmap(result_with_new, cur_result);

            }

        }
    }

    //Là on merge
    bitmap_list* res = create_bitmap_list();
    bitmap_node* current_no_new = result_no_new->head;
    bitmap_node* current_with_new = result_with_new->head;
    while (current_no_new != NULL || current_with_new != NULL) {
        if (current_no_new != NULL && current_with_new != NULL) {
            if (current_no_new->value->nb_bits < current_with_new->value->nb_bits) {
                if (!has_bsubset(res, current_no_new->value)) {
                    add_bitmap(res, current_no_new->value);
                }
                current_no_new = current_no_new->next;
            }
            else {
                if (!has_bsubset(res, current_with_new->value)) {
                    add_bitmap(res, current_with_new->value);
                }
                current_with_new = current_with_new->next;
            }
        }
        else if (current_no_new != NULL) {
            if (!has_bsubset(res, current_no_new->value)) {
                add_bitmap(res, current_no_new->value);
            }
            current_no_new = current_no_new->next;
        }
        else {
            if (!has_bsubset(res, current_with_new->value)) {
                add_bitmap(res, current_with_new->value);
            }
            current_with_new = current_with_new->next;
        }
    }


    return res;
}

bitmap_list* create_edge_bitmaps(Graph* graph, int id_edge) {
    bitmap_list* res = create_bitmap_list();

    for (Node* current = graph->edges[id_edge]->vertices->head; current != NULL; current = current->next) {
        bitmap* cur = create_bitmap((graph->nb_vertices/(sizeof(int)*8))+1);
        set_bit(cur, current->value-1, 1);
        add_bitmap(res, cur);
    }
    return res;


}

void berge_bitmap_algorithm(Graph* graph) {
    //le cur Tr qu'on build
    bitmap_list* cur_covers = create_edge_bitmaps(graph, 0);
    //print_bitmaps(cur_covers);
    for (int i = 1; i < graph->nb_edges; i++) {
        //bitmap_list* vertices_list = create_edge_bitmaps(graph, i);
        bitmap_list* new_covers = bV_operator(cur_covers, graph->edges[i]->vertices);
        //printf("\n");
        //print_bitmaps(new_covers);

        //Pour free je pense que ce sera pas mal
        bitmap_list* previous = cur_covers;
        cur_covers = new_covers;

    }
    print_as_ints(cur_covers);

}