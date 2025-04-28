#include <stdio.h>
#include <stdlib.h>
#include "DL_opti.h"

// Cette variante est une optimisation de l'implementation de DL
void DL_opti(Graph* graph) {
    //printf("\nDL algorithm\n");
    iListList* Tr_Hi = create_list_list();
    // On initialise le dual avec tous les sommets de la première hyperarête
    for (Node* cur = graph->edges[0]->vertices->head; cur != NULL; cur = cur->next) {
        iList* singleton = create_list();
        append(singleton, cur->value);
        append_list(Tr_Hi, singleton);
    }


    // Ici on retiens les transversaux qui couvrent la nouvelle hyperarête pour construire Trguaranteed
    // et on retiens les transversaux qui ne couvrent pas la nouvelle hyperarête pour construire Tr_Hi_prime
    for (int i = 1; i < graph->nb_edges; i++) {
        iListList* Trguaranteed = create_list_list();
        iListList* Tr_Hi_prime = create_list_list();
        for (NodeList* cur = Tr_Hi->head; cur != NULL; cur = cur->next) {
            if (contains_any(cur->value, graph->edges[i]->vertices)) {
                append_list(Trguaranteed, deep_copy(cur->value));
            } else {
                append_list(Tr_Hi_prime, deep_copy(cur->value));
            }
        }

        // On récupère les sommets couverts par leur singleton dans Trguaranteed
        // On récupère les sommets de la nouvelle hyperarête qui ne sont pas couverts par Trguaranteed
        iList* ecovered_i = create_list();
        iList* e_prime_i = create_list();
        for (Node* cur = graph->edges[i]->vertices->head; cur != NULL; cur = cur->next) {
            iList* singleton = create_list();
            append(singleton, cur->value);
            if (contains_list_singl(Trguaranteed, singleton)) {
                append(ecovered_i, cur->value);
            } else {
                append(e_prime_i, cur->value);
            }
            free_list(singleton);
        }
        free_list(ecovered_i);

        // On ajoute l'hyperarête si elle n'est pas un superset d'une hyperarête déjà présente dans Trguaranteed
        bubble_sort_list(Tr_Hi_prime); // Trie Tr_Hi_prime par cardinalité croissante
        for (NodeList* t_prime = Tr_Hi_prime->head; t_prime != NULL; t_prime = t_prime->next) {
            for (Node* v = e_prime_i->head; v != NULL; v = v->next) {
                iList* union_set = deep_copy(t_prime->value);
                append(union_set, v->value);
                if (!contains_superset(Trguaranteed, union_set)) {
                    append_list(Trguaranteed, union_set);
                } else {
                    free_list(union_set);
                }
            }
        }
        free_list_list(Tr_Hi_prime);
        free_list(e_prime_i);

        // Actualise le dual
        free_list_list(Tr_Hi);
        Tr_Hi = Trguaranteed;
        //print_list_list(Tr_Hi);
        //printf("---------%d--------- DL_opti\n", i);
    }

    print_list_list(Tr_Hi);
    free_list_list(Tr_Hi);
}
