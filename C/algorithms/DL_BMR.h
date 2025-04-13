#ifndef DL_BMR_H
#define DL_BMR_H

#include "../HGraph.h"
#include "../structures/List.h"

iListList* DL_BMR(Graph* graph);

int dl_bmr_contains_any(iList* list, iList* sublist);
int dl_bmr_contains_list(iListList* list, iList* sublist);
int dl_bmr_contains_superset(iListList* list, iList* sublist);
int dl_bmr_is_equal(iList* l1, iList* l2);
int dl_bmr_is_superset(iList* l1, iList* l2);

#endif