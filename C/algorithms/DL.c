#include <stdio.h>
#include <stdlib.h>
#include "DL.h"

void DL_algorithm(Graph* graph) {
    //printf("\nDL algorithm\n");
    iListList* Tr_H1 = create_list_list();
    // On initialise le dual avec tous les sommets de la première hyperarête
    for (Node* cur = graph->edges[0]->vertices->head; cur != NULL; cur = cur->next) {
        iList* singleton = create_list();
        append(singleton, cur->value);
        append_list(Tr_H1, singleton);
    }

    iListList* Tr_Hi = Tr_H1;

    // Ici on retiens que les transversaux qui couvrent la nouvelle hyperarête pour construire Trguaranteed
    for (int i = 1; i < graph->nb_edges; i++) {
        iListList* Trguaranteed = create_list_list();
        for (NodeList* cur = Tr_Hi->head; cur != NULL; cur = cur->next) {
            if (contains_any(cur->value, graph->edges[i]->vertices)) {
                append_list(Trguaranteed, deep_copy(cur->value));
            }
        }

        // On récupère les sommets couverts par Trguaranteed
        iList* ecovered_i = create_list();
        for (Node* cur = graph->edges[i]->vertices->head; cur != NULL; cur = cur->next) {
            iList* singleton = create_list();
            append(singleton, cur->value);
            if (contains_list(Trguaranteed, singleton)) {
                append(ecovered_i, cur->value);
            }
            free_list(singleton);
        }

        // Ici on retiens les transversaux qui ne couvrent pas la nouvelle hyperarête pour construire Tr_Hi_prime
        iListList* Tr_Hi_prime = create_list_list();
        for (NodeList* cur = Tr_Hi->head; cur != NULL; cur = cur->next) {
            if (!contains_list(Trguaranteed, cur->value)) {
                append_list(Tr_Hi_prime, deep_copy(cur->value));
            }
        }

        // On récupère les sommets de la nouvelle hyperarête qui ne sont pas couverts par Trguaranteed
        iList* e_prime_i = create_list();
        for (Node* cur = graph->edges[i]->vertices->head; cur != NULL; cur = cur->next) {
            if (!contains(ecovered_i, cur->value)) {
                append(e_prime_i, cur->value);
            }
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
    }

    print_list_list(Tr_Hi);
    free_list_list(Tr_Hi);
}

// Vérifie si une liste contient un élément
int contains_any(iList* list, iList* sublist) {
    for (Node* cur = sublist->head; cur != NULL; cur = cur->next) {
        if (contains(list, cur->value)) {
            return 1;
        }
    }
    return 0;
}

// Vérifie si une liste de listes contient une liste spécifique
int contains_list(iListList* list, iList* sublist) {
    for (NodeList* cur = list->head; cur != NULL; cur = cur->next) {
        if (is_equal(cur->value, sublist)) {
            return 1;
        }
    }
    return 0;
}

// Vérifie si une liste de listes contient un superset d'une liste spécifique
int contains_superset(iListList* list, iList* sublist) {
    for (NodeList* cur = list->head; cur != NULL; cur = cur->next) {
        if (is_superset(sublist, cur->value)) {
            return 1;
        }
    }
    return 0;
}

// Vérifie si deux listes sont égales
int is_equal(iList* l1, iList* l2) {
    if (l1->size != l2->size) return 0;
    for (Node* cur = l1->head; cur != NULL; cur = cur->next) {
        if (!contains(l2, cur->value)) {
            return 0;
        }
    }
    return 1;
}

// Vérifie si une liste est un superset d'une autre
int is_superset(iList* l1, iList* l2) {
    for (Node* cur = l2->head; cur != NULL; cur = cur->next) {
        if (!contains(l1, cur->value)) {
            return 0;
        }
    }
    return 1;
}
