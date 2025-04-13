#ifndef BMR_H
#define BMR_H

#include "../HGraph.h"   // ou "../HGraph.h" selon votre organisation
#include "../structures/List.h"    // pour iList, iListList
extern iListList* globalTransversals;

void BMR_algorithm(Graph* graph, iList* Vpartition, int depth, Graph* fullgraph);

#endif // BMR_H