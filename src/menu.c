#include <stdio.h>
#include "menu.h"
#include "chargement_reseau.h"
#include "edges.h"   


void menu(const char *nom_fichier){
    int entry = -1;
    // char fichier[100]; utilité ?
    struct Graph *g = charger_graphe(nom_fichier);

    while (entry != 0){
      
        printf("\n");
        printf("===== MENU RESEAU DE TRANSPORT ===== \n");
        printf("1- Afficher les informations d'une station \n");
        printf("2- Lister les voisins d'une station \n");
        printf("3- Calculer un chemin minimal \n");
        printf("4- Afficher les stations triées par degré \n");
        printf("0- Quitter \n");
        printf("Votre choix : ");
    
        scanf("%d", &entry);

        switch (entry){

        case 1:
            afficher_reseau(nom_fichier, g);
            break;
        
        case 2:
            printGraph(g);
            break;

        case 3:
            break;
        
        case 4:
            break;
        }
    }
}