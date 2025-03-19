import random

FILENAME = "random.txt"

# Paramètres
MIN_SOMMETS = 5
MAX_SOMMETS = 15
MIN_HYPERARETES = 3

def generer_hypergraphe():
    """Génère un hypergraphe aléatoire et l'écrit dans random.txt"""

    n = random.randint(MIN_SOMMETS, MAX_SOMMETS)
    
    sommets = list(range(1, n + 1))
    
    m = random.randint(max(MIN_HYPERARETES, n // 2), 2 * n)

    
    hyperaretes = set() 
    
    while len(hyperaretes) < m:
        taille = random.randint(2, n)
        hyperarete = tuple(sorted(random.sample(sommets, taille)))
        hyperaretes.add(hyperarete)

    with open(FILENAME, "w") as file:
        for hyperarete in hyperaretes:
            file.write(" ".join(map(str, hyperarete)) + "\n")
    
    print(f"Hypergraphe généré avec {n} sommets et {m} hyperarêtes dans {FILENAME}")

def main():
    """Vérifie si le fichier est vide et génère un nouvel hypergraphe."""
    try:
        with open(FILENAME, "r") as file:
            contenu = file.read().strip()
    except FileNotFoundError:
        contenu = ""

    if contenu: 
        print("On supprime ce qu'il y a dans l'ancien fichier txt")
    generer_hypergraphe()

if __name__ == "__main__":
    main()
