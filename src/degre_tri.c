#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/degre_tri.h"
#include "../include/chargement_reseau.h"

// Échange deux éléments dans le tableau
static void echanger(StationDegre *a, StationDegre *b, StatsTri *stats) {
    StationDegre temp = *a;
    *a = *b;
    *b = temp;
    stats->nb_permutations++;
}


// tri par sélection (croissant)
static void tri_selection(StationDegre tableau[], int n, StatsTri *stats) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            stats->nb_comparaisons++;
            if (tableau[j].degre < tableau[min_idx].degre) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            echanger(&tableau[min_idx], &tableau[i], stats);
        }
    }
}

// Tri par insertion (croissant)
static void tri_insertion(StationDegre tableau[], int n, StatsTri *stats) {
    for (int i = 1; i < n; i++) {
        StationDegre cle = tableau[i];
        int j = i - 1;
        while (j >= 0) {
            stats->nb_comparaisons++;
            if (tableau[j].degre > cle.degre) {
                tableau[j + 1] = tableau[j];
                stats->nb_permutations++;
                j--;
            } else {
                break;
            }
        }
        tableau[j + 1] = cle;
    }
}

// Partition pour le Tri Rapide (QuickSort)
static int partition(StationDegre tableau[], int bas, int haut, StatsTri *stats) {
    int pivot = tableau[haut].degre;
    int i = (bas - 1);
    for (int j = bas; j <= haut - 1; j++) {
        stats->nb_comparaisons++;
        if (tableau[j].degre < pivot) {
            i++;
            echanger(&tableau[i], &tableau[j], stats);
        }
    }
    echanger(&tableau[i + 1], &tableau[haut], stats);
    return (i + 1);
}

// Tri Rapide récursif (QuickSort)
static void tri_rapide(StationDegre tableau[], int bas, int haut, StatsTri *stats) {
    if (bas < haut) {
        int pi = partition(tableau, bas, haut, stats);
        tri_rapide(tableau, bas, pi - 1, stats);
        tri_rapide(tableau, pi + 1, haut, stats);
    }
}

void executer_et_afficher_tri(struct Graph *graph, int type_tri) {
    if (!graph) return;

    int n = graph->numVertices;
    StationDegre *tableau = malloc(n * sizeof(StationDegre));
    if (!tableau) return;

    // Remplissage du tableau des couples (id, degre)
    int nb_valides = 0;
    for (int i = 0; i < n; i++) {
        char* nom = obtenir_nom_station(i);
        if (strcmp(nom, "Inconnu") != 0) {
            tableau[nb_valides].id_station = i;
            tableau[nb_valides].degre = degreSortant(graph, i);
            nb_valides++;
        }
    }

    StatsTri stats = {0, 0};

    if (type_tri == 1) {
        tri_insertion(tableau, nb_valides, &stats);
        printf("\n--- Résultats Tri par sélection ---\n");
    } else if (type_tri == 2) {
        tri_selection(tableau, nb_valides, &stats);
        printf("\n--- Résultats Tri par insertion ---\n");
    } else {
        tri_rapide(tableau, 0, nb_valides - 1, &stats);
        printf("\n--- Résultats Tri rapide ---\n");
    }



    // Affichage des 10 premières stations (plus bas degrés)
    printf("\n--- Les 10 stations avec les plus bas degrés ---\n");
    int limite_debut = (nb_valides < 10) ? nb_valides : 10;
    for (int i = 0; i < limite_debut; i++) {
        printf("ID %3d | %-35s | Degré : %d\n",
            tableau[i].id_station, obtenir_nom_station(tableau[i].id_station), tableau[i].degre);
    }

    if (nb_valides > 20) {
        printf("\n  ... ( %d stations intermédiaires masquées ) ...\n", nb_valides - 20);
    }

    // Affichage des 10 dernières stations (plus hauts degrés)
    if (nb_valides > 10) {
        printf("\n--- Les 10 stations avec les plus hauts degrés ---\n");
        int debut_fin = (nb_valides > 20) ? nb_valides - 10 : limite_debut;
        for (int i = debut_fin; i < nb_valides; i++) {
            printf("ID %3d | %-35s | Degré : %d\n",
                   tableau[i].id_station, obtenir_nom_station(tableau[i].id_station), tableau[i].degre);
        }
    }


    printf("\nStatistiques du tri :\n");
    printf("- Nombre de comparaisons : %ld\n", stats.nb_comparaisons);
    printf("- Nombre de permutations : %ld\n", stats.nb_permutations);

    free(tableau);
}