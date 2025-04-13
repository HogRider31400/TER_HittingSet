#include <stdio.h>
#include <stdlib.h>
#include "DL_BMR.h"

iListList* DL_BMR(Graph* graph) {
    iListList* Tr_H1 = create_list_list();
    for (Node* cur = graph->edges[0]->vertices->head; cur != NULL; cur = cur->next) {
        iList* singleton = create_list();
        append(singleton, cur->value);
        append_list(Tr_H1, singleton);
    }
    iListList* Tr_Hi = Tr_H1;

    for (int i = 1; i < graph->nb_edges; i++) {
        iListList* Trguaranteed = create_list_list();
        for (NodeList* cur = Tr_Hi->head; cur != NULL; cur = cur->next) {
            if (dl_bmr_contains_any(cur->value, graph->edges[i]->vertices)) {
                append_list(Trguaranteed, deep_copy(cur->value));
            }
        }
        iList* ecovered_i = create_list();
        for (Node* cur = graph->edges[i]->vertices->head; cur != NULL; cur = cur->next) {
            iList* singleton = create_list();
            append(singleton, cur->value);
            if (dl_bmr_contains_list(Trguaranteed, singleton)) {
                append(ecovered_i, cur->value);
            }
            free_list(singleton);
        }
        iListList* Tr_Hi_prime = create_list_list();
        for (NodeList* cur = Tr_Hi->head; cur != NULL; cur = cur->next) {
            if (!dl_bmr_contains_list(Trguaranteed, cur->value)) {
                append_list(Tr_Hi_prime, deep_copy(cur->value));
            }
        }
        iList* e_prime_i = create_list();
        for (Node* cur = graph->edges[i]->vertices->head; cur != NULL; cur = cur->next) {
            if (!contains(ecovered_i, cur->value)) {
                append(e_prime_i, cur->value);
            }
        }
        free_list(ecovered_i);
        bubble_sort_list(Tr_Hi_prime);
        for (NodeList* t_prime = Tr_Hi_prime->head; t_prime != NULL; t_prime = t_prime->next) {
            for (Node* v = e_prime_i->head; v != NULL; v = v->next) {
                iList* union_set = deep_copy(t_prime->value);
                append(union_set, v->value);
                if (!dl_bmr_contains_superset(Trguaranteed, union_set)) {
                    append_list(Trguaranteed, union_set);
                } else {
                    free_list(union_set);
                }
            }
        }
        free_list_list(Tr_Hi_prime);
        free_list(e_prime_i);
        free_list_list(Tr_Hi);
        Tr_Hi = Trguaranteed;
    }
    return Tr_Hi;
}

int dl_bmr_contains_any(iList* list, iList* sublist) {
    for (Node* cur = sublist->head; cur != NULL; cur = cur->next) {
        if (contains(list, cur->value)) {
            return 1;
        }
    }
    return 0;
}

int dl_bmr_contains_list(iListList* list, iList* sublist) {
    for (NodeList* cur = list->head; cur != NULL; cur = cur->next) {
        if (dl_bmr_is_equal(cur->value, sublist)) {
            return 1;
        }
    }
    return 0;
}

int dl_bmr_contains_superset(iListList* list, iList* sublist) {
    for (NodeList* cur = list->head; cur != NULL; cur = cur->next) {
        if (dl_bmr_is_superset(sublist, cur->value)) {
            return 1;
        }
    }
    return 0;
}

int dl_bmr_is_equal(iList* l1, iList* l2) {
    if (l1->size != l2->size) return 0;
    for (Node* cur = l1->head; cur != NULL; cur = cur->next) {
        if (!contains(l2, cur->value)) {
            return 0;
        }
    }
    return 1;
}

int dl_bmr_is_superset(iList* l1, iList* l2) {
    for (Node* cur = l2->head; cur != NULL; cur = cur->next) {
        if (!contains(l1, cur->value)) {
            return 0;
        }
    }
    return 1;
}