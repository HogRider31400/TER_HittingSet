from utils import load_hypergraph
from BMR import BMR_algorithm

if __name__ == "__main__":
    filename = "../C/data/SDTH62.txt"
    hypergraph = load_hypergraph(filename)

    print("Hypergraphe chargé :")
    for edge in hypergraph:
        print(edge)

    transversals = BMR_algorithm(hypergraph, set(), hypergraph)

    print("\n--- Transversaux minimaux trouvés ---")
    for t in transversals:
        print(sorted(t))
