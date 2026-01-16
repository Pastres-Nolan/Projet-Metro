#ifndef MENU_H
#define MENU_H

#include "graphe.h"

void vider_buffer();
void menu(const char *nom_fichier);

void afficher_info_station(const struct Graph *graph);
void afficher_voisins_station(const struct Graph *graph);
void calcul_chemin_minimal(struct Graph *g);
void gerer_menu_tri(struct Graph *g);

#endif
