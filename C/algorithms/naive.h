//
// Created by Alex on 25/01/2025.
//

#ifndef NAIVE_H
#define NAIVE_H
#include "../HGraph.h"
#include "../structures/Queue.h"

void enum_covers_recursive(Graph* graph, iList* cur_covered_vertices, iList* cur_used_vertices);
void enum_covers_iterative(Graph* graph);
void enum_covers_iterative_a(Graph* graph);
int has_subset(iListList* list, iList* slist);
#endif //NAIVE_H
