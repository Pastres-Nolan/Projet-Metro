# Liste de tâches

## README.md
- Faire un README de qualité.

## Makefile
- Compiler dans le dossier `builds` à créer si inexistant.
- OK : Commandes Make :
    - `make`        : compile
    - `make clean`  : supprime les `.o`
    - `make fclean` : supprime tout
    - `make re`     : recompilation propre
- OK : Lancement de l'exécutable avec `./metro nom_fichier` ou juste `./metro`

## menu.c
- Ajouter un `default` dans le `switch`.
- Implémenter un test pour :
    - `scanf("%d", &entry);`
    - `struct Graph *g = charger_graphe(nom_fichier);`
        - Si le fichier n'est pas reconnu, ne pas afficher le menu et quitter.

## Global
- Rédiger le rapport.
- Effectuer des tests.
- Ajouter des commentaires et vérifier que tous les `malloc` sont `free`.
- Attention à la gestion d'erreurs.