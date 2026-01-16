#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../include/menu.h"
#include "../include/itineraire.h"
#include "../include/graphe.h"
#include "../include/station.h"
#include "../include/tri_degre.h"

double time1, timedif;

void vider_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void menu(const char *nom_fichier){
    int entry = -1;
    struct Graph *graph = charger_reseau(nom_fichier);

    if (!graph) {
        fprintf(stderr, " Erreur : Impossible de charger le réseau, fichier introuvable.\n");
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
            printf("Erreur : saisie invalide.\n");
            vider_buffer();  // Vider le buffer pour éviter une boucle infinie
            continue;        // Recommence la boucle
        }
        vider_buffer();
        switch (entry){

        case 1:
            afficher_info_station(graph);
            break;

        case 2:
            afficher_voisins_station(graph);
            break;

        case 3:
            calcul_chemin_minimal(graph);
            break;
        case 4:
            gerer_menu_tri(graph);
            break;

        case 0:
            printf("Au revoir !\n");
            break;

        default:
            printf("Erreur : saisie invalide.\n");
        }
        if (entry != 0) {
            printf("\nAppuyez sur Entrée pour revenir au menu");
            while (getchar() != '\n'); // Attend que l'utilisateur appuie sur Entrée
        }
    }
    liberer_tout();
    freeGraph(graph);
}


void afficher_info_station(const struct Graph *graph) {
    char input[100];
    char *endptr;

    printf("Entrez l'ID ou le Nom de la station pour lister ses informations : ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (input[0] == '\0') {
        printf("Erreur : entrée vide.\n");
        return;
    }
    long val = strtol(input, &endptr, 10);
    int id = (*endptr == '\0') ? (int)val : chercher_id_par_nom(input);
    char* nom = obtenir_nom_station(id);

    if (strcmp(nom, "Inconnu") != 0) {
        printf("\n--- Informations de la station %s (ID: %d) ---\n", nom, id);
        printf("Nom    : %s\n", nom);
        printf("ID     : %d\n", id);
        printf("Degré  : %d\n", degreSortant(graph, id));
    } else printf("Erreur : Station '%s' inconnue.\n", input);
}

void afficher_voisins_station(const struct Graph *graph) {
    char input[100];
    char *endptr;

    printf("Entrez l'ID ou le Nom de la station pour lister ses voisins : ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (input[0] == '\0') {
        printf("Erreur : entrée vide.\n");
        return;
    }
    long val = strtol(input, &endptr, 10);
    int id = (*endptr == '\0') ? (int)val : chercher_id_par_nom(input);
    char* nom = obtenir_nom_station(id);

    if (strcmp(nom, "Inconnu") != 0) {
        printf("\n--- Voisins de la station %s (ID: %d) ---\n", nom, id);
        struct Node* temp = graph->adjLists[id];

        if (!temp) {
            printf("Cette station n'a aucun voisin direct.\n");
            return;
        }

        while (temp) {
            printf(" %d - %-30s (%d min)\n", temp->vertex, obtenir_nom_station(temp->vertex), temp->weight);
            temp = temp->next;
        }
    } else printf("Erreur : Station '%s' inconnue.\n", input);
}

void calcul_chemin_minimal(struct Graph *graph) {
    char nom_dep[100], nom_arr[100];
    char *endptr;

    printf("Entrez l'ID ou le Nom de la station de départ : ");
    fgets(nom_dep, sizeof(nom_dep), stdin);
    nom_dep[strcspn(nom_dep, "\n")] = 0;

    // Détection ID ou Nom pour le départ
    long val_dep = strtol(nom_dep, &endptr, 10);
    int id_dep = (*endptr == '\0' && strlen(nom_dep) > 0) ? (int)val_dep : chercher_id_par_nom(nom_dep);

    printf("Entrez l'ID ou le Nom de la station d'arrivée : ");
    fgets(nom_arr, sizeof(nom_arr), stdin);
    nom_arr[strcspn(nom_arr, "\n")] = 0;

    // Détection ID ou Nom pour l'arrivée
    long val_arr = strtol(nom_arr, &endptr, 10);
    int id_arr = (*endptr == '\0' && strlen(nom_arr) > 0) ? (int)val_arr : chercher_id_par_nom(nom_arr);


    if (id_dep != -1 && id_arr != -1) {
        dijkstra(graph, id_dep, id_arr);
    } else {
        fprintf(stderr, "Erreur : Station(s) introuvable(s).\n");
    }
}

void gerer_menu_tri(struct Graph *graph) {
    int choix;
    printf("\n--- Classement des stations par degré ---\n");
    printf("Choisissez l'algorithme de tri :\n");
    printf("1- Tri par sélection\n");
    printf("2- Tri par insertion\n");
    printf("3- Tri rapide (récursif)\n");
    printf("Votre choix : ");

    if (scanf("%d", &choix) != 1) {
        vider_buffer();
        fprintf(stderr, "Erreur : Saisie invalide.\n");
        return;
    }
    vider_buffer();

    if (choix >= 1 && choix <= 3) {
        time1 = (double) clock();
        time1 = time1 / CLOCKS_PER_SEC;
        // On appelle le tri en fonction du choix de l'utilisateur
        executer_et_afficher_tri(graph, choix);
        timedif = ( ((double) clock()) / CLOCKS_PER_SEC) - time1;
     printf("- Temps d'exécution du tri %lf secondes\n", timedif);
    } else {
        fprintf(stderr, "Erreur : Saisie invalide.\n");
    }
}
