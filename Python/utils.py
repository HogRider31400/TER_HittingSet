from pathlib import Path
from typing import List, Set

def get_project_root() -> Path:
    """
    Donne le chemin du root du projet.
    """
    return Path(__file__).parent.parent

def get_data_path(filename):
    """
    Fonction qui renvoie le chemin vers un fichier .txt contenant des datas d'un hypergraphe.
    Input : Un nom de fichier
    Output : Le chemin d'accès de l'utilisateur vers le fichier .txt
    """
    return str(get_project_root()).replace("\\", "/") + "/C/data/" + filename

def is_superset_of_any(candidate: set, sets: list[set]) -> bool:
    """
    Renvoie True si `candidate` est un superset strict de 
    n'importe quel ensemble dans `sets`, False sinon.
    """
    for s in sets:
        if candidate > s: 
            return True
    return False

def load_hypergraph(file_path: str) -> List[Set[int]]:
    """

    → Retourne une liste de sets du genre : [{1,2,3}, {4,5}, {2,5,6}]
    """
    with open(file_path, 'r') as f:
        return [set(map(int, line.strip().split())) for line in f if line.strip()]