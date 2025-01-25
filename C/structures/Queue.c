//
// Created by Alex on 25/01/2025.
//

#include "Queue.h"

#include <stdlib.h>

Queue* queue_create() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

void queue_add(Queue* queue, iList* covered_vertices, iList* used_vertices) {
    QueueElem* new_elem = (QueueElem*)malloc(sizeof(QueueElem));
    new_elem->covered_vertices = covered_vertices;
    new_elem->used_vertices = used_vertices;
    new_elem->next = NULL;
    if (queue->tail == NULL) {
        queue->head = new_elem;
        queue->tail = new_elem;
    }
    else {
        queue->tail->next = new_elem;
        queue->tail = new_elem;
    }
}
QueueElem* queue_pop(Queue* queue) {
    if (queue->head == NULL) {
        return NULL;
    }
    QueueElem* pop_elem = queue->head;
    queue->head = pop_elem->next;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }

    return pop_elem;
}
