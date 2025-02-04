//
// Created by Alex on 25/01/2025.
//

#ifndef QUEUE_H
#define QUEUE_H
#define DEFAULT_SIZE 10000
#include "List.h"

typedef struct QueueElem {
    iList* covered_edges;
    iList* used_vertices;
    struct QueueElem* next;
} QueueElem;

typedef struct Queue {
    QueueElem* head;
    QueueElem* tail;
    int size;
} Queue;

typedef struct QueueAElem {
    iList* covered_vertices;
    iList* used_vertices;
} QueueAElem;

typedef struct QueueA {
    QueueAElem* elems;
    int cur_size;
    int head;
    int tail;
    int max_size;
} QueueA;

Queue* queue_create();
void queue_add(Queue* queue, iList* l1, iList* l2);
QueueElem* queue_pop(Queue* queue);

QueueA* queue_a_create ();
void queue_a_add(QueueA* queue, iList* l1, iList* l2);
QueueAElem queue_a_pop(QueueA* queue);
#endif //QUEUE_H
