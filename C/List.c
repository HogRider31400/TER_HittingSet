#include <stddef.h>
#include <stdlib.h>
//
// Created by Alex on 25/01/2025.
//
typedef struct iList {
    struct Node* head;
} iList;

typedef struct Node {
    int value;
    struct Node* next;
} Node;

iList* create_list() {
    iList* list = malloc(sizeof(iList));
    list->head = NULL;
    return list;
}
void append(iList* list, int value) {
    Node* new_node = malloc(sizeof(Node));
    new_node->value = value;
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

void print_list(iList* list) {
    //if (list->head == NULL) return;
    Node* current = list->head;
    while (current != NULL) {
        printf("%d ", current->value);
        current = current->next;
    }
}