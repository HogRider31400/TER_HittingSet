//
// Created by Alex on 25/01/2025.
//

#ifndef LIST_H
#define LIST_H
#include <stddef.h>

typedef struct iList {
    struct Node* head;
} iList;

typedef struct Node {
    int value;
    struct Node* next;
} Node;

iList* create_list();
void append(iList* list, int value);
void remove_value(iList* list, int value);
int get(iList* list, int index);
void print_list(iList* list);
int contains(iList* elems, int elem_value);
iList* deep_copy(iList* list);
void merge_unique(iList* l1, iList* l2);
void append_unique(iList* list, int elem_value);
#endif //LIST_H
