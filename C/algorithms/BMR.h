#ifndef BMR_H
#define BMR_H

#include "../HGraph.h"   // ou "../HGraph.h" selon votre organisation
#include "../structures/List.h"    // pour iList, iListList

void BMR_algorithm(Graph* graph, iList* Vpartition);
void launch_bmr_empty(Graph* graph);

#endif // BMR_H
