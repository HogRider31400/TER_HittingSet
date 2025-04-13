from typing import List, Set


def is_superset_of_any(candidate: Set[int], sets: List[Set[int]]) -> bool:
    """
    Retourne True si 'candidate' est un superset d'au moins un ensemble de la liste.
    """
    for s in sets:
        if s <= candidate: 
            return True
    return False


def load_hypergraph(file_path: str) -> List[Set[int]]:
    """

    → Retourne une liste de sets du genre : [{1,2,3}, {4,5}, {2,5,6}]
    """
    with open(file_path, 'r') as f:
        return [set(map(int, line.strip().split())) for line in f if line.strip()]


def DL_algorithm(hypergraph: List[Set[int]]) -> List[Set[int]]:
    if not hypergraph:
        return []

    Tr = [{v} for v in hypergraph[0]]

    for i in range(1, len(hypergraph)):
        e_i = hypergraph[i]
        Tr_prev = Tr

        Tr_guaranteed = [t for t in Tr_prev if not t.isdisjoint(e_i)]

        e_covered = set()
        for t in Tr_guaranteed:
            e_covered.update(t & e_i)

        Tr_prime = [t for t in Tr_prev if t not in Tr_guaranteed]

        e_i_prime = e_i - e_covered

        for t in sorted(Tr_prime, key=len):
            for v in e_i_prime:
                new_candidate = t | {v}
                if not is_superset_of_any(new_candidate, Tr_guaranteed):
                    Tr_guaranteed.append(new_candidate)

        Tr = Tr_guaranteed

    return Tr
