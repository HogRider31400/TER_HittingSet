from pathlib import Path

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