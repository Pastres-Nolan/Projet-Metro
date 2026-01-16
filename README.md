# Projet Algorithmique & Structures de Données - Réseau de transport et parcours de graphes
Ce projet modélise un réseau de transport (bus, métro, tram, . . .) sous forme de graphe et propose
des fonctionnalités d’analyse et de recherche d’itinéraires.

## Organisation du projet
- **`src/`** : Code source (`.c`)
  - `main.c` : Point d'entrée et boucle principale
  - `menu.c` : Totalité de l'intéraction avec l'utilisateur
  - `station.c` : Chargement des données stations et edges
  - `graphe.c` : Gestion de la structure du graphe
  - `itineraire.c` : Algorithme de Dijkstra
  - `tri_degre.c` : Algorithmes de tri
- **`include/`** : Fichiers d'en-tête (`.h`)
- **`data/`** : Fichiers de réseau (`metro.txt`, `metro_long.txt`) et un script de génération
- **`builds/`** : Fichiers objets (`.o`) générés lors de la compilation
- **`docs/`** : Rapport et énoncé du projet
- **`makefile`**

## Compilation
| Commande | Description |
| :--- | :--- |
| `make` | Compile le projet et génère l'exécutable `metro`. |
| `make clean` | Supprime les fichiers objets (`.o`) dans le dossier `builds`. |
| `make fclean` | Supprime les fichiers objets et l'exécutable `metro`. |
| `make re` | Effectue un nettoyage complet puis recompile tout (clean + make). |

## Utilisation
Lancement de l'exécutable avec : 
```bash
./metro data/nom_fichier
```
Fichiers disponibles : `data/metro.txt` et `data/metro_log.txt`.

La commande `./metro` sans argument prend le fichier `metro.txt` par défaut.
