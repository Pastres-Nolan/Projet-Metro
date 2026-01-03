#include <stdio.h>
#include "menu.h"
#include "chargement_reseau.h"
#include "edges.h"
#include "algorithmes.h"
#include <string.h>
#include <stdlib.h>

void vider_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void menu(const char *nom_fichier){
    int entry = -1;
    struct Graph *g = charger_reseau(nom_fichier);

    if (g == NULL) {
        printf(" Erreur : Impossible de charger le réseau.\n");
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
            printf("Erreur : Saisie invalide.\n");
            vider_buffer();  // Vider le buffer pour éviter une boucle infinie
            continue;        // Recommence la boucle
        }
        vider_buffer();
        switch (entry){

        case 1:
            afficher_info_station(g);
            break;
        
        case 2:
            afficher_voisins_station(g);
            break;

        case 3: {
            char nom_dep[100], nom_arr[100];
            char *endptr;
            int id_dep, id_arr;

            printf("Entrez l'ID ou le Nom de la station de départ : ");
            fgets(nom_dep, sizeof(nom_dep), stdin);
            nom_dep[strcspn(nom_dep, "\n")] = 0;

            // Détection ID ou Nom pour le départ
            long val_dep = strtol(nom_dep, &endptr, 10);
            if (*endptr == '\0' && strlen(nom_dep) > 0) id_dep = (int)val_dep;
            else id_dep = chercher_id_par_nom(nom_dep);

            printf("Entrez l'ID ou le Nom de la station d'arrivée : ");
            fgets(nom_arr, sizeof(nom_arr), stdin);
            nom_arr[strcspn(nom_arr, "\n")] = 0;

            // Détection ID ou Nom pour l'arrivée
            long val_arr = strtol(nom_arr, &endptr, 10);
            if (*endptr == '\0' && strlen(nom_arr) > 0) id_arr = (int)val_arr;
            else id_arr = chercher_id_par_nom(nom_arr);

            if (id_dep != -1 && id_arr != -1) {
                dijkstra(g, id_dep, id_arr);
            } else {
                printf("Erreur : Station(s) introuvable(s).\n");
            }
            break;
        }
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
    freeGraph(g);
}