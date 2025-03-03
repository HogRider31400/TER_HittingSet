#ifndef DL_H
#define DL_H
#include "../HGraph.h"
#include "../structures/List.h"

void DL_algorithm(Graph* graph);
int contains_any(iList* list, iList* sublist);
int contains_list(iListList* list, iList* sublist);
int contains_superset(iListList* list, iList* sublist);
int is_equal(iList* l1, iList* l2);
int is_superset(iList* l1, iList* l2);
#endif
