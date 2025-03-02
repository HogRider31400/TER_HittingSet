#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "List.h"
//
// Created by Alex on 25/01/2025.
//
iList* create_list() {
    iList* list = malloc(sizeof(iList));
    list->head = NULL;
    list->size = 0;
    return list;
}
void append(iList* list, int value) {
    Node* new_node = malloc(sizeof(Node));
    new_node->value = value;
    new_node->next = NULL;
    list->size++;
    if (list->head == NULL) {
        list->head = new_node;
        return;
    }

    Node* current = list->head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}
void remove_value(iList* list, int value) {
    if (list->head == NULL) return;

    if (list->head->value == value) {
        Node* to_delete = list->head;
        list->head = list->head->next;
        free(to_delete);
        return;
    }

    Node* current = list->head;
    while (current->next != NULL && current->next->value != value) {
        current = current->next;
    }

    if (current->next != NULL) {
        Node* to_delete = current->next;
        current->next = current->next->next;
        free(to_delete);
    }
    list->size--;
}
int get(iList* list, int index) {
    if (list->head == NULL) return -1;

    Node* current = list->head;
    for (int i = 0; i < index; i++) {
        if (current->next == NULL) return -1;
        current = current->next;
    }
    return current->value;
}

int tail(iList* list) {
    if (list->head == NULL) return -1;
    Node* current = list->head;
    //printf("%d\n", current->value);
    while (current->next != NULL) {
        current = current->next;
    }
    return current->value;
}

void print_list(iList* list) {
    //if (list->head == NULL) return;
    Node* current = list->head;
    while (current != NULL) {
        printf("%d ", current->value);
        current = current->next;
    }
}

//Vérifie qu'une valeur (et non un pointeur !) est dans la liste
int contains(iList* elems, int elem_value) {

    Node* cur = elems->head;
    while (cur != NULL) {
        if (cur->value == elem_value) {
            return 1;
        }
        cur = cur->next;
    }

    return 0;
}

iList* deep_copy(iList* list) {
    iList* new_list = create_list();
    Node* current = list->head;
    while (current != NULL) {
        append(new_list, current->value);
        current = current->next;
    }
    return new_list;
}

void free_list(iList* list) {
    Node* current = list->head;
    Node* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    list->head = NULL;
    list->size = 0;
}

//merge 2 listes sans doublon et met le résultat dans la première
void merge_unique(iList* l1, iList* l2) {
    for (Node* cur = l2->head; cur != NULL; cur = cur->next) {
        if (contains(l1, cur->value) == 0)
            append(l1, cur->value);
    }
}

void append_unique(iList* list, int elem_value) {
    if (contains(list, elem_value) == 1) return;
    append(list,elem_value);
}

//Ici on s'affaire aux listes de listes
iListList* create_list_list() {
    iListList* list = malloc(sizeof(iListList));
    list->head = NULL;
    list->size = 0;
}

void append_list(iListList* list, iList* elem_list) {
    NodeList* new_node = malloc(sizeof(NodeList));
    new_node->value = elem_list;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
        return;
    }

    Node* current = list->head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;

    list->size++;
}

void print_list_list(iListList* list) {
    NodeList* current = list->head;
    while (current != NULL) {
        print_list(current->value);
        printf("\n");
        current = current->next;
    }
}

void swap_list(NodeList* n1, NodeList* n2) {

    iList* temp = n1->value;

    n1->value = n2->value;
    n2->value = temp;

    //free temp ?

}

void bubble_sort_list(iListList* list) {
    if (list == NULL) return;
    //printf("CICICICI\n");
    for (int cur_iter = 0; cur_iter < list->size; cur_iter++) {
        //print_list_list(list);
        NodeList* current = list->head;
        NodeList* previous = NULL;
        while (current != NULL) {
            if (previous != NULL) {
                if (previous->value->size > current->value->size) {
                    swap_list(previous, current);
                }
            }

            previous = current;
            current = current->next;
        }

    }

}