#ifndef CHARGEMENT_RESEAU_H
#define CHARGEMENT_RESEAU_H

#include "edges.h"

struct Graph* charger_reseau(const char *nom_fichier);

void afficher_info_station(struct Graph *graph);
int chercher_id_par_nom(const char *nom);
void liberer_tout();


#endif
