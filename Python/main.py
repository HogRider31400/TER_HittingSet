from DL import DL_algorithm, load_hypergraph

if __name__ == "__main__":
    file_path = "../C/data/example.txt" 
    hypergraph = load_hypergraph(file_path)

    print("Hypergraphe chargé :")
    for edge in hypergraph:
        print(edge)

    print("\nTransversaux minimaux :")
    transversals = DL_algorithm(hypergraph)
    for t in transversals:
        print(sorted(t))
