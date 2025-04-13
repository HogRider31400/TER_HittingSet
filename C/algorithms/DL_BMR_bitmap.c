/***************************************************************
 * DL_BMR_bitmap.c
 * Implémentation de l'algorithme DL_BMR en utilisant des bitmaps
 * pour une meilleure efficacité en termes de mémoire et de calcul.
 *
 * AUTEUR : Leandre GIAMMONA
 ***************************************************************/

 #include <stdio.h>
 #include <stdlib.h>
 #include "DL_BMR_bitmap.h"
 #include "../structures/Bitmap.h"

/* -----------------------------------------------------------------
 * Vérifie si un bitmap contient au moins un élément d'un ensemble.
 * ----------------------------------------------------------------- */
int dl_bmr_bitmap_contains_any(bitmap* bm, iList* vertices) {
    for (Node* cur = vertices->head; cur != NULL; cur = cur->next) {
        if (get_bit(bm, cur->value - 1)) {
            return 1;
        }
    }
    return 0;
}
 
 /* -----------------------------------------------------------------
  * Algorithme DL_BMR avec bitmaps
  * ----------------------------------------------------------------- */
 bitmap_list* DL_BMR_bitmap(Graph* graph) {
    bitmap_list* Tr_H1 = create_bitmap_list();
    Edge* first_edge = graph->edges[0];

    for (Node* cur = first_edge->vertices->head; cur != NULL; cur = cur->next) {
        bitmap* singleton = create_bitmap(graph->nb_vertices);
        set_bit(singleton, cur->value - 1, 1);
        add_bitmap(Tr_H1, singleton);
        printf("Ajout du singleton dans Tr_H1 : ");
        print_bitmap(singleton);
    }

    bitmap_list* Tr_Hi = Tr_H1;

    for (int i = 1; i < graph->nb_edges; i++) {
        bitmap_list* Trguaranteed = create_bitmap_list();
        Edge* edge = graph->edges[i];

        printf("\nTraitement de l'arête %d : ", i);
        print_list(edge->vertices);

        for (bitmap_node* cur = Tr_Hi->head; cur != NULL; cur = cur->next) {
            bitmap* bm = cur->value;
            if (dl_bmr_bitmap_contains_any(bm, edge->vertices)) {
                add_bitmap(Trguaranteed, copy_bitmap(bm));
                printf("Ajout à Trguaranteed : ");
                print_bitmap(bm);
            }
        }

        bitmap* ecovered_i = create_bitmap(graph->nb_vertices);
        for (Node* cur = edge->vertices->head; cur != NULL; cur = cur->next) {
            set_bit(ecovered_i, cur->value - 1, 1);
        }

        bitmap_list* Tr_Hi_prime = create_bitmap_list();
        for (bitmap_node* cur = Tr_Hi->head; cur != NULL; cur = cur->next) {
            bitmap* bm = cur->value;
            if (!has_bsubset(Trguaranteed, bm)) {
                add_bitmap(Tr_Hi_prime, copy_bitmap(bm));
                printf("Ajout à Tr_Hi_prime : ");
                print_bitmap(bm);
            }
        }

        bitmap* e_prime_i = create_bitmap(graph->nb_vertices);
        for (Node* cur = edge->vertices->head; cur != NULL; cur = cur->next) {
            if (!get_bit(ecovered_i, cur->value - 1)) {
                set_bit(e_prime_i, cur->value - 1, 1);
            }
        }

        free_bitmap(ecovered_i);

        for (bitmap_node* t_prime = Tr_Hi_prime->head; t_prime != NULL; t_prime = t_prime->next) {
            for (Node* v = edge->vertices->head; v != NULL; v = v->next) {
                bitmap* union_set = copy_bitmap(t_prime->value);
                set_bit(union_set, v->value - 1, 1);
                if (!has_bsubset(Trguaranteed, union_set)) {
                    add_bitmap(Trguaranteed, union_set);
                    printf("Ajout de l'ensemble d'union à Trguaranteed : ");
                    print_bitmap(union_set);
                } else {
                    printf("Non ajouté (redondant ou superflu) : ");
                    print_bitmap(union_set);
                    free_bitmap(union_set);
                }
            }
        }

        free_bitmap(e_prime_i);
        free_bitmap_list(Tr_Hi_prime);
        free_bitmap_list(Tr_Hi);
        Tr_Hi = Trguaranteed;
    }

    return Tr_Hi;
}
