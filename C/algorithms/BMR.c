#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "graph_reader.h"
#include "HGraph.h"

void findMinimalTransversals(a_Graph *hg);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    a_Graph *hg = read_agraph_from_file(argv[1]);
    if (!hg) {
        fprintf(stderr, "Error reading graph from file\n");
        return EXIT_FAILURE;
    }
    
    findMinimalTransversals(hg);
    
    return EXIT_SUCCESS;
}

bool isTransversal(int *subset, int subset_size, a_Graph *hg) {
    for (int i = 0; i < hg->nb_edges; i++) {
        bool covered = false;
        for (int j = 0; j < subset_size; j++) {
            for (int k = 0; k < hg->edges[i]->nb_vertices; k++) {
                if (subset[j] == hg->edges[i]->vertices[k]) {
                    covered = true;
                    break;
                }
            }
            if (covered) break;
        }
        if (!covered) return false;
    }
    return true;
}

void generateMinimalTransversals(a_Graph *hg, int *current, int size, int depth, int *solutions, int *sol_size) {
    if (isTransversal(current, size, hg)) {
        solutions[*sol_size] = size;
        memcpy(solutions + *sol_size + 1, current, size * sizeof(int));
        *sol_size += size + 1;
        return;
    }
    
    for (int i = depth; i < MAX_VERTICES; i++) {
        current[size] = i;
        generateMinimalTransversals(hg, current, size + 1, i + 1, solutions, sol_size);
    }
}

void findMinimalTransversals(a_Graph *hg) {
    int solutions[MAX_VERTICES * MAX_VERTICES] = {0};
    int sol_size = 0;
    int current[MAX_VERTICES] = {0};
    
    generateMinimalTransversals(hg, current, 0, 1, solutions, &sol_size);
    
    printf("Minimal Transversals:\n");
    int i = 0;
    while (i < sol_size) {
        int size = solutions[i];
        printf("[");
        for (int j = 0; j < size; j++) {
            printf("%d", solutions[i + j + 1]);
            if (j < size - 1) printf(", ");
        }
        printf("]\n");
        i += size + 1;
    }
}

