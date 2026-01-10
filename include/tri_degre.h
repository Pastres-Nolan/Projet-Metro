#ifndef TRI_DEGRE_H
#define TRI_DEGRE_H

#include "graphe.h"

// Structure stockant le couple (id_station, degre)
typedef struct {
    int id_station;
    int degre;
} StationDegre;

// Structure pour stocker les statistiques de tri
typedef struct {
    long nb_comparaisons;
    long nb_permutations;
} StatsTri;

// Fonction principale pour l'option 4 du menu
void executer_et_afficher_tri(struct Graph *graph, int type_tri);

#endif