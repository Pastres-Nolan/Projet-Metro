#include <stdio.h>
#include "../include/menu.h"

int main(int argc, char **argv){
    const char *fichier;

    if(argc == 1) {
        fichier = "data/metro.txt";  // fichier par défaut
        printf("Aucun fichier fourni, utilisation de %s\n", fichier);
    } else if (argc == 2) {
        fichier = argv[1];  // fichier fourni
    } else {
        fprintf(stderr, "Erreur : trop d'arguments fournis.\n");
        return 1;
    }

    menu(fichier);
    return 0;
}
