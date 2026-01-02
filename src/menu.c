#include <stdio.h>
#include "menu.h"
#include "chargement_reseau.h"
#include "edges.h"   

void vider_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void menu(const char *nom_fichier){
    int entry = -1;
    struct Graph *g = charger_reseau(nom_fichier);

    if (g == NULL) {
        printf(" Impossible de charger le réseau.\n");
        return;
    }

    while (entry != 0){
        printf("\n===== MENU RESEAU DE TRANSPORT ===== \n");
        printf("1- Afficher les informations d'une station \n");
        printf("2- Lister les voisins d'une station \n");
        printf("3- Calculer un chemin minimal \n");
        printf("4- Afficher les stations triées par degré \n");
        printf("0- Quitter \n");
        printf("Votre choix : ");
    
        if (scanf("%d", &entry) != 1) {  // Vérifie si l'entrée est un entier
            printf("Saisie invalide.\n");
            vider_buffer();  // Vider le buffer pour éviter une boucle infinie
            continue;        // Recommence la boucle
        }

        switch (entry){

        case 1:
            afficher_info_station(g);
            break;
        
        case 2:
            afficher_voisins_station(g);
            break;

        case 3:
            break;
        
        case 4:
            break;

        case 0:
            printf("Au revoir !\n");
            break;

        default:
            printf("Choix invalide, réessayez.\n");
        }
    }
    liberer_tout();
}