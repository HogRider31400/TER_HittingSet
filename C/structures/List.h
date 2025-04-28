//
// Created by Alex on 25/01/2025.
//

#ifndef LIST_H
#define LIST_H
#include <stddef.h>

typedef struct iList {
    struct Node* head;
    int size;
} iList;

typedef struct Node {
    int value;
    struct Node* next;
} Node;

typedef struct NodeList {
    iList* value;
    struct NodeList* next;
} NodeList;

typedef struct iListList {
    NodeList* head;
    int size;
} iListList;

iList* create_list();
void append(iList* list, int value);
void remove_value(iList* list, int value);
int get(iList* list, int index);
void print_list(iList* list);
int contains(iList* elems, int elem_value);
int contains_any(iList* list1, iList* list2);
int is_equal(iList* list1, iList* list2);
int is_superset(iList* list1, iList* list2);
iList* deep_copy(iList* list);
void merge_unique(iList* l1, iList* l2);
void append_unique(iList* list, int elem_value);
int tail(iList* list);
void free_list(iList* list);


iListList* create_list_list();
void append_list(iListList* list, iList* elem_list);
void print_list_list(iListList* list);
void bubble_sort_list(iListList* list);
void swap_list(NodeList* n1, NodeList* n2);
int contains_list(iListList* list, iList* sublist);
int is_equal_list(iListList* list1, iListList* list2);
int contains_list_singl(iListList* list, iList* sublist);
int contains_superset(iListList* list, iList* sublist);
void free_list_list(iListList* list);
void append_all(iList* dest, iList* source);
#endif //LIST_H
