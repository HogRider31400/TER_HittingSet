from utils import load_hypergraph
from BMR import BMR_algorithm
import sys
sys.path.append("..")
from transversal_enumeration import dong_li
if __name__ == "__main__":
    filename = "../C/data/SDTH/SDTH402.dat"
    hypergraph = load_hypergraph(filename)

    print("Hypergraphe chargé :")
    for edge in hypergraph:
        print(edge)

    transversals = BMR_algorithm(hypergraph, set(), hypergraph)
    #transversals = dong_li(hypergraph)
    print("\n--- Transversaux minimaux trouvés ---")
    for t in transversals:
        print(sorted(t))
    print(len(transversals))