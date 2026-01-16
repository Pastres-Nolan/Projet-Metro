#ifndef CHARGEMENT_RESEAU_H
#define CHARGEMENT_RESEAU_H

#include "graphe.h"

struct Graph* charger_reseau(const char *nom_fichier);
int station_valide (char *line, int lineno);
int edge_valide (char *line, int lineno, struct Graph* graph);

int chercher_id_par_nom(const char *nom);
char* obtenir_nom_station(int id);
void liberer_tout();


#endif
