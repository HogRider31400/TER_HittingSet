#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EDGES 100
#define MAX_VERTICES 50 

// Structure pour représenter une hyperarête
typedef struct {
    int vertices[MAX_VERTICES];
    int size;
} Edge;

// Structure pour représenter un hypergraphe
typedef struct {
    Edge edges[MAX_EDGES];
    int num_edges;
} Hypergraph;

// Fonction pour lire le fichier et construire l'hypergraphe
Hypergraph* read_hypergraph(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Erreur : Impossible d'ouvrir le fichier %s\n", filename);
        exit(1);
    }

    Hypergraph* graph = (Hypergraph*)malloc(sizeof(Hypergraph));
    graph->num_edges = 0;

    char line[256];
    while (fgets(line, sizeof(line), file) && graph->num_edges < MAX_EDGES) {
        Edge* edge = &graph->edges[graph->num_edges];
        edge->size = 0;

        char* token = strtok(line, " \t\n");
        while (token && edge->size < MAX_VERTICES) {
            edge->vertices[edge->size++] = atoi(token);
            token = strtok(NULL, " \t\n");
        }

        graph->num_edges++;
    }

    fclose(file);
    return graph;
}

// Fonction pour afficher un hypergraphe
void print_hypergraph(Hypergraph* graph) {
    printf("Hypergraphe :\n");
    for (int i = 0; i < graph->num_edges; i++) {
        printf("Arete %d: ", i + 1);
        for (int j = 0; j < graph->edges[i].size; j++) {
            printf("%d ", graph->edges[i].vertices[j]);
        }
        printf("\n");
    }
}

void bmr_algorithm(Hypergraph* graph) {

    // Étape 1 : Initialisation du transversal minimal
    int transversal[MAX_VERTICES] = {0}; 
    int trans_size = 0;

    for (int i = 0; i < graph->num_edges; i++) {
        for (int j = 0; j < graph->edges[i].size; j++) {
            int v = graph->edges[i].vertices[j];

            // Si le sommet n'est pas déjà dans le transversal, on l'ajoute
            int already_in = 0;
            for (int k = 0; k < trans_size; k++) {
                if (transversal[k] == v) {
                    already_in = 1;
                    break;
                }
            }
            if (!already_in) {
                transversal[trans_size++] = v;
                break;
            }
        }
    }

    // Affichage du transversal minimal
    printf("Transversal minimal trouve : { ");
    for (int i = 0; i < trans_size; i++) {
        printf("%d ", transversal[i]);
    }
    printf("}\n");
}

// Fonction principale
int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage : %s <fichier_hypergraphe>\n", argv[0]);
        return 1;
    }

    Hypergraph* graph = read_hypergraph(argv[1]);
    print_hypergraph(graph);
    bmr_algorithm(graph);

    free(graph);

    return 0;
}
