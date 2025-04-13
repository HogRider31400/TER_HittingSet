from typing import List, Set
from collections import Counter
from DL import DL_algorithm
from utils import is_superset_of_any


def is_transversal(hypergraph: List[Set[int]], candidate: Set[int]) -> bool:
    return all(edge & candidate for edge in hypergraph)


def is_minimal(candidate: Set[int], full_graph: List[Set[int]]) -> bool:
    for v in candidate:
        subset = candidate - {v}
        if all(subset & edge for edge in full_graph):
            return False
    return True


def order_vertices_by_occurrences(hypergraph: List[Set[int]]) -> List[int]:
    counter = Counter()
    for edge in hypergraph:
        counter.update(edge)
    return [v for v, _ in counter.most_common()[::-1]]


def build_Epartition(hypergraph: List[Set[int]], vi: int, Vpartition: Set[int]) -> List[Set[int]]:
    new_edges = []
    for edge in hypergraph:
        if vi not in edge:
            new_edge = edge - Vpartition
            if not new_edge:
                continue
            if not any(new_edge > e for e in new_edges):
                new_edges = [e for e in new_edges if not e > new_edge]
                new_edges.append(new_edge)
    return new_edges


def average_edge_cardinality(edges: List[Set[int]]) -> float:
    return sum(len(e) for e in edges) / len(edges) if edges else 0.0


def BMR_algorithm(graph: List[Set[int]], Vpartition: Set[int], full_graph: List[Set[int]]) -> List[Set[int]]:
    all_vertices = order_vertices_by_occurrences(graph)
    Tr = []

    for vi in all_vertices:
        Epartition = build_Epartition(graph, vi, Vpartition)
        Vpartition.add(vi)

        a = average_edge_cardinality(Epartition) * len(Epartition)

        if len(Epartition) >= 2 and a >= 5000:
            print(f"[BMR] depth={len(Vpartition)} | vi={vi} | Epartition={len(Epartition)} | a={a:.2f}")
            Tr_rec = BMR_algorithm(Epartition, Vpartition, full_graph)
            for t in Tr_rec:
                full_t = t | Vpartition
                if not is_transversal(full_graph, full_t):
                    continue
                if not is_minimal(full_t, full_graph):
                    continue

                Tr = [existing for existing in Tr if not existing > full_t]
                if full_t not in Tr and not any(full_t > existing for existing in Tr):
                    Tr.append(full_t)
        else:
            Tr_local = DL_algorithm(Epartition)
            for t in Tr_local:
                full_t = t | Vpartition
                if not is_transversal(full_graph, full_t):
                    continue
                if not is_minimal(full_t, full_graph):
                    continue

                Tr = [existing for existing in Tr if not existing > full_t]
                if full_t not in Tr and not any(full_t > existing for existing in Tr):
                    Tr.append(full_t)

        Vpartition.remove(vi)

    return Tr
