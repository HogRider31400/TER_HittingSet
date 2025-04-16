from typing import List, Set
from collections import Counter
from utils import is_superset_of_any
import copy
import sys
sys.path.append("..")
from transversal_enumeration import dong_li

def normalize(inp):
    corresp = {}
    icorresp = {}
    out = []
    cid = 1
    for elem in inp:
        cur = []
        for celem in elem:
            if celem in corresp:
                cur.append(corresp[celem])
            else:
                corresp[celem] = cid
                icorresp[cid] = celem
                cur.append(cid)
                cid += 1
        out.append(cur)
    return icorresp, out

def unnormalize(inp, co):
    out = []

    for elem in inp:
        cur = []
        for celem in elem:
            cur.append(co[celem])
        out.append(cur)
    return out


def is_transversal(hypergraph, candidate):
    return all(edge & candidate for edge in hypergraph)


def is_minimal(candidate, full_graph):
    for v in candidate:
        subset = candidate - {v}
        if all(subset & edge for edge in full_graph):
            return False
    return True


def order_vertices_by_occurrences(hypergraph):
    counter = Counter()
    for edge in hypergraph:
        counter.update(edge)
    return [v for v, _ in counter.most_common()[::-1]]


def build_Epartition(hypergraph, vi, Vpartition, V):
    new_edges = []
    for edge in hypergraph:
        if vi not in edge:
            new_edge = edge - V
            if not new_edge:
                continue
            if not any(new_edge >= e for e in new_edges):
                new_edges = [e for e in new_edges if not e >= new_edge]
                new_edges.append(new_edge)
    return new_edges


def average_edge_cardinality(edges) :
    return sum(len(e) for e in edges) / len(edges) if edges else 0.0


def BMR_algorithm(graph, Vpartition, full_graph):
    all_vertices = order_vertices_by_occurrences(graph)
    all_vertices_c = copy.deepcopy(all_vertices)
    Tr = []
    all_vertices = set(all_vertices)
    for vi in all_vertices_c:
        all_vertices.remove(vi)
        Epartition = build_Epartition(graph, vi, Vpartition, all_vertices)
        Vpartition.add(vi)
        a = average_edge_cardinality(Epartition) * len(Epartition)
        if len(Epartition) >= 2 and a >= 55:
            #print(f"[BMR] depth={len(Vpartition)} | vi={vi} | Epartition={len(Epartition)} | a={a:.2f}")
            Tr_rec = BMR_algorithm(Epartition, Vpartition, full_graph)
        else:
            if len(Epartition) != 0:
                inp = list(map(list, Epartition))
                inp.sort()
                co, Epartition = normalize(inp)
                res = dong_li(Epartition)
                Tr_rec = list(map(set, unnormalize(res, co)))
            else:
                Tr_rec = [set()]
            #Tr_local.sort()
        for t in Tr_rec:
            full_t = t | Vpartition
            if not is_transversal(full_graph, full_t):
                continue
            Tr = [existing for existing in Tr if not existing >= full_t]
            if not any(full_t >= existing for existing in Tr):
                Tr.append(full_t)

        Vpartition.remove(vi)

    return Tr
