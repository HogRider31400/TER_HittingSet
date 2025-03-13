//
// Created by Alex on 13/03/2025.
//
#include "berge_bitmap.h"

#include <stdio.h>

#include "naive.h"


bitmap_list* bV_operator(bitmap_list* b1, bitmap_list* b2) {
    bitmap_list* result = create_bitmap_list();

    //On rajoute l'union des éléments non nuls
    for (bitmap_node* current_b1 = b1->head; current_b1 != NULL; current_b1 = current_b1->next) {
        for (bitmap_node* current_b2 = b2->head; current_b2 != NULL; current_b2 = current_b2->next) {
            bitmap* cur_result = bitmap_or(current_b1->value, current_b2->value);

            add_bitmap(result, cur_result);
        }
    }

    return result;
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
        bitmap_list* vertices_list = create_edge_bitmaps(graph, i);
        bitmap_list* new_covers = bV_operator(cur_covers, vertices_list);
        //printf("\n");
        //print_bitmaps(new_covers);
        bitmap_list* filtered_covers = create_bitmap_list();

        bubble_sort_bitmap(new_covers);
        //printf("ayooo\n");
        for (bitmap_node* current = new_covers->head; current != NULL; current = current->next) {
            bitmap* cur = current->value;
            //printf("ok\n");
            if (has_bsubset(filtered_covers,cur) == 0) {
                add_bitmap(filtered_covers, cur);
            }
        }

        //Pour free je pense que ce sera pas mal
        bitmap_list* previous = cur_covers;
        cur_covers = filtered_covers;

    }
    print_as_ints(cur_covers);

}