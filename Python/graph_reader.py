import os
import utils

def graph_reader(filename):
    """
    Renvoie les données d'un hypergraphes
    Input : ficher .txt contenant un hypergraphe
    Output : en [0] : la liste de sommets de l'hypergraphe.
             en [1] : la liste de liste des hyperarêtes de l'hypergraphe.
    """
    vertices = []
    edges = []
    filename = utils.get_data_path(filename)
    with open (filename, "r") as f:
        for line in f:
            line_data = line.split()
            for vertice in line_data:
                if int(vertice) not in vertices:
                    vertices.append(int(vertice))
            edge = list(map(int, line_data))
            edges.append(edge)
    return vertices, edges