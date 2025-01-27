#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include "HGraph.h"

// Vérifie si l'ensemble donné couvre toutes les arêtes du graphe
int covers(Graph* graph, iList* vertices) {
    for (int i = 0; i < graph->nb_edges; i++) {
        int edge_covered = 0;
        for (Node* cur = graph->edges[i]->vertices->head; cur != NULL; cur = cur->next) {
            if (contains(vertices, cur->value)) {
                edge_covered = 1;
                break;
            }
        }
        if (!edge_covered) return 0;  // Une arête n'est pas couverte
    }
    return 1;  // Toutes les arêtes couvertes
}

// Fonction récursive pour énumérer toutes les couvertures minimales
void enum_covers_recursive(Graph* graph, iList* cur_covered_vertices, iList* cur_used_vertices) {
    for (Node* vertice_node = cur_used_vertices->head; vertice_node != NULL; vertice_node = vertice_node->next) {
        int vertice = vertice_node->value;

        // Crée une nouvelle liste sans le sommet actuel
        iList* new_used_vertices = deep_copy(cur_used_vertices);
        remove_value(new_used_vertices, vertice);

        // Recalcule les sommets couverts par le nouvel ensemble
        iList* new_covered_vertices = create_list();
        for (Node* cur = new_used_vertices->head; cur != NULL; cur = cur->next) {
            for (Node* edge_node = graph->vertices[cur->value - 1]->edges->head; edge_node != NULL; edge_node = edge_node->next) {
                merge_unique(new_covered_vertices, graph->edges[edge_node->value - 1]->vertices);
            }
        }

        // Vérifie si le nouvel ensemble reste une couverture
        if (covers(graph, new_covered_vertices)) {
            // Affiche la couverture trouvée
            print_list(new_used_vertices);
            printf(" est un transversal minimal\n");

            enum_covers_recursive(graph, new_covered_vertices, new_used_vertices);
        }

        // Free memory
        free(new_used_vertices);
        free(new_covered_vertices);
    }
}

// Fonction principale pour énumérer toutes les couvertures minimales
void enum_covers(Graph* graph) {
    // Init avec tous les sommets pour couverture
    iList* initial_covered_vertices = create_list();
    iList* initial_used_vertices = create_list();

    // Tous les sommets sont initialement utilisés
    for (int i = 0; i < graph->nb_vertices; i++) {
        append(initial_covered_vertices, graph->vertices[i]->id);
        append(initial_used_vertices, graph->vertices[i]->id);
    }

    enum_covers_recursive(graph, initial_covered_vertices, initial_used_vertices);

    // Free memory
    free(initial_covered_vertices);
    free(initial_used_vertices);
}

