/***************************************************************
 * BMR.c
 * Implémentation de l'algorithme BMR pour trouver les transversales
 * minimales d'un hypergraphe, en s'appuyant sur l'algorithme DL_BMR.
 *
 * AUTEUR : Leandre GIAMMONA
 ***************************************************************/

 #include <stdio.h>
 #include <stdlib.h>
 #include "BMR.h"
 #include "DL_BMR.h"
 #include "List.h"
 #include "Queue.h"
 #include "../HGraph.h"
 
 // Variable globale pour accumuler les transversaux minimaux finaux
 iListList* globalTransversals = NULL;
static int is_transversal(Graph* fullGraph, iList* candidate);
static int is_minimal_transversal(Graph* fullGraph, iList* candidate);

 
 /* -----------------------------------------------------------------
  * merge_transversals:
  * Fusionne les transversaux locaux (local) dans l'accumulateur global (global),
  * en vérifiant que l'on ne rajoute pas de doublons ou de supersets.
  * ----------------------------------------------------------------- */
static void merge_transversals(iListList* global, iListList* local, Graph* fullGraph) {
    for (NodeList* cur = local->head; cur != NULL; cur = cur->next) {
         if (is_transversal(fullGraph, cur->value) && is_minimal_transversal(fullGraph, cur->value)) {
             if (!dl_bmr_contains_list(global, cur->value)) {
                 append_list(global, deep_copy(cur->value));
             }
         }
    }
}


 
 /* -----------------------------------------------------------------
  * Déclarations de fonctions utilitaires internes
  * ----------------------------------------------------------------- */
 static iList* get_all_vertices(Graph* graph);
 static int count_occurrences(Graph* graph, int vertex);
 static iList* order_vertices_by_occurrences(Graph* graph);
 static Graph* create_empty_graph();
 static void free_graph_shallow(Graph* g);
 static void build_subgraph_excluding_vertex(Graph* original, Graph* subgraph, int excludedV);
 static double average_edge_cardinality(Graph* g);
 static void add_edge_minimal(Graph* subgraph, iList* newVertices);
 
 /* -----------------------------------------------------------------
  * BMR_algorithm:
  * Appelle récursivement l'algorithme BMR et, pour les sous-problèmes
  * jugés "petits", utilise DL_BMR pour récupérer les transversaux
  * minimaux qui sont alors fusionnés dans globalTransversals.
  * ----------------------------------------------------------------- */
 void BMR_algorithm(Graph* graph, iList* Vpartition)
 {
     if (!graph || graph->nb_edges == 0) {
         return;
     }
 
     iList* sortedVertices = order_vertices_by_occurrences(graph);
     if (!sortedVertices) {
         fprintf(stderr, "[BMR] Erreur: impossible de récupérer la liste des sommets.\n");
         return;
     }
 
     Node* cur = sortedVertices->head;
     while (cur) {
         int vi = cur->value;
         Graph* Epartition = create_empty_graph();
         build_subgraph_excluding_vertex(graph, Epartition, vi);
 
         append(Vpartition, vi);
  
         double avgCard = average_edge_cardinality(Epartition);
         double a = avgCard * Epartition->nb_edges;
 
         if (Epartition->nb_edges >= 2 && a >= 50.0) {
             BMR_algorithm(Epartition, Vpartition);
         } else {
            iListList* subTransversals = DL_BMR(Epartition);
            merge_transversals(globalTransversals, subTransversals, graph);
            free_list_list(subTransversals);
        }        
 
         remove_value(Vpartition, vi);
         free_graph_shallow(Epartition);
  
         cur = cur->next;
     }
  
     free_list(sortedVertices);
 }
 
 /* -----------------------------------------------------------------
  * get_all_vertices:
  * Récupère la liste de tous les sommets apparaissant dans 'graph'
  * (sans doublons).
  * ----------------------------------------------------------------- */
 static iList* get_all_vertices(Graph* graph)
 {
     iList* vertices = create_list();
     if (!graph) return vertices;
  
     for (int e = 0; e < graph->nb_edges; e++) {
         Edge* ed = graph->edges[e];
         if (!ed) continue;
         Node* n = ed->vertices->head;
         while (n) {
             if (!contains(vertices, n->value)) {
                 append(vertices, n->value);
             }
             n = n->next;
         }
     }
     return vertices;
 }
 
 /* -----------------------------------------------------------------
  * count_occurrences:
  * Retourne le nombre d'arêtes de 'graph' qui contiennent 'vertex'
  * ----------------------------------------------------------------- */
 static int count_occurrences(Graph* graph, int vertex)
 {
     int count = 0;
     for (int i = 0; i < graph->nb_edges; i++) {
         Edge* e = graph->edges[i];
         if (e && contains(e->vertices, vertex)) {
             count++;
         }
     }
     return count;
 }
 
 /* -----------------------------------------------------------------
  * order_vertices_by_occurrences:
  * - Récupère tous les sommets du graph,
  * - Les trie par nombre d'occurrences croissant,
  * - Renvoie la iList triée.
  * ----------------------------------------------------------------- */
 static iList* order_vertices_by_occurrences(Graph* graph)
 {
     iList* allV = get_all_vertices(graph);
     if (!allV) return NULL;
  
     int n = allV->size;
     if (n <= 1) {
         return allV;
     }
 
     typedef struct { int v; int occ; } Pair;
     Pair* arr = (Pair*)malloc(n * sizeof(Pair));
  
     Node* c = allV->head;
     int idx = 0;
     while (c) {
         arr[idx].v = c->value;
         arr[idx].occ = count_occurrences(graph, c->value);
         idx++;
         c = c->next;
     }
 
     for (int i = 0; i < n - 1; i++) {
         for (int j = 0; j < n - 1 - i; j++) {
             if (arr[j].occ > arr[j+1].occ) {
                 Pair tmp = arr[j];
                 arr[j] = arr[j+1];
                 arr[j+1] = tmp;
             }
         }
     }
  
     free_list(allV); 
     iList* sorted = create_list();
     for (int i = 0; i < n; i++) {
         append(sorted, arr[i].v);
     }
  
     free(arr);
     return sorted;
 }
 
 /* -----------------------------------------------------------------
  * create_empty_graph:
  * Crée un Graph alloué dynamiquement, sans arêtes, nb_edges=0.
  * ----------------------------------------------------------------- */
 static Graph* create_empty_graph()
 {
     Graph* g = (Graph*)malloc(sizeof(Graph));
     g->nb_edges = 0;
     g->nb_vertices = 0;
     for (int i = 0; i < MAX_EDGES; i++) {
          g->edges[i] = NULL;
     }
     return g;
 }
 
 /* -----------------------------------------------------------------
  * free_graph_shallow:
  * Libère un Graph (libère uniquement les arêtes et leurs listes de sommets).
  * ----------------------------------------------------------------- */
 static void free_graph_shallow(Graph* g)
 {
     if (!g) return;
     for (int i = 0; i < g->nb_edges; i++) {
         if (g->edges[i]) {
             free_list(g->edges[i]->vertices);
             free(g->edges[i]);
         }
     }
     free(g);
 }
 
 /* -----------------------------------------------------------------
  * build_subgraph_excluding_vertex:
  * Construit dans 'subgraph' toutes les arêtes de 'original' qui ne contiennent pas 'excludedV'
  * en maintenant la minimalité.
  * ----------------------------------------------------------------- */
 static void build_subgraph_excluding_vertex(Graph* original, Graph* subgraph, int excludedV)
 {
     if (!original || !subgraph) return;
  
     for (int i = 0; i < original->nb_edges; i++) {
         Edge* e = original->edges[i];
         if (!e) continue;
         if (!contains(e->vertices, excludedV)) {
             iList* copyVertices = deep_copy(e->vertices);
             add_edge_minimal(subgraph, copyVertices);
         }
     }
 }
 
 /* -----------------------------------------------------------------
  * average_edge_cardinality:
  * Retourne la moyenne des tailles d'arêtes dans g.
  * ----------------------------------------------------------------- */
 static double average_edge_cardinality(Graph* g)
 {
     if (!g || g->nb_edges == 0) return 0.0;
  
     long total = 0;
     for (int i = 0; i < g->nb_edges; i++) {
         Edge* e = g->edges[i];
         if (e) {
             total += e->vertices->size;
         }
     }
     return (double)total / (double)g->nb_edges;
 }
 
 /* -----------------------------------------------------------------
  * add_edge_minimal:
  * Ajoute l'arête décrite par 'newVertices' dans 'subgraph', en maintenant la minimalité.
  * ----------------------------------------------------------------- */
 static void add_edge_minimal(Graph* subgraph, iList* newVertices)
{
    int toAdd = 1;
    Edge** newArray = malloc((subgraph->nb_edges + 1) * sizeof(Edge*));
    int newCount = 0;
 
    for (int i = 0; i < subgraph->nb_edges; i++) {
        Edge* existing = subgraph->edges[i];
        if (!existing) continue;
 
        if (dl_bmr_is_superset(existing->vertices, newVertices)) {
            continue;
        }
 
        if (dl_bmr_is_superset(newVertices, existing->vertices)) {
            toAdd = 0;
        }
 
        newArray[newCount] = existing;
        newCount++;
    }
 
    for (int i = 0; i < newCount; i++) {
        subgraph->edges[i] = newArray[i];
    }
    subgraph->nb_edges = newCount;
    free(newArray);
 
    if (toAdd) {
        if (subgraph->nb_edges < MAX_EDGES) {
            Edge* newEdge = (Edge*)malloc(sizeof(Edge));
            newEdge->vertices = newVertices;
            subgraph->edges[subgraph->nb_edges] = newEdge;
            subgraph->nb_edges++;
        } else {
            free_list(newVertices);
        }
    } else {
        free_list(newVertices);
    }
}

static int is_transversal(Graph* fullGraph, iList* candidate) {
    for (int i = 0; i < fullGraph->nb_edges; i++) {
         Edge* e = fullGraph->edges[i];
         if (!e) continue;
         int found = 0;
         for (Node* cur = e->vertices->head; cur != NULL; cur = cur->next) {
              if (contains(candidate, cur->value)) {
                  found = 1;
                  break;
              }
         }
         if (!found)
             return 0;
    }
    return 1;
}

static int is_minimal_transversal(Graph* fullGraph, iList* candidate) {
    for (Node* cur = candidate->head; cur != NULL; cur = cur->next) {
         int removedValue = cur->value;
         iList* candidateMinus = deep_copy(candidate);
         remove_value(candidateMinus, removedValue);
         if (is_transversal(fullGraph, candidateMinus)) {
             free_list(candidateMinus);
             return 0;
         }
         free_list(candidateMinus);
    }
    return 1;
}
