#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/edges.h"
#include "../include/chargement_reseau.h"

#define HASH_SIZE 1024

typedef struct {
    int id;
    char *nom;
} Station;

typedef struct HashNode {
    Station *station;
    struct HashNode *next;
} HashNode;

// Table de hachage pour la recherche par nom
static HashNode *table_hachage[HASH_SIZE];
// Tableau dynamique pour la recherche par ID
static Station *tableau_stations = NULL;
static int nb_stations_global= 0;


static int hash(const char *nom) {
    int sum = 0;
    while (*nom) sum += *nom++;
    return sum % HASH_SIZE;
}

int chercher_id_par_nom(const char *nom) {
    HashNode *curr = table_hachage[hash(nom)];
    while (curr) {
        if (curr->station && curr->station->nom && strcmp(curr->station->nom, nom) == 0) {
            return curr->station->id;
        }
        curr = curr->next;
    }
    return -1;
}

char* obtenir_nom_station(int id) {
    if (id >= 0 && id < nb_stations_global) return tableau_stations[id].nom;
    return "Inconnu";
}

struct Graph* charger_reseau(const char *nom_fichier) {
    FILE *file = fopen(nom_fichier, "r");
    if (!file) return NULL;
    // 1. Déterminer le nombre de stations (max_id)
    char line[2048];
    int max_id = -1;
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "STATION", 7) == 0) {
            char *copy = strdup(line);
            if (!copy) continue; // Malloc check
            strtok(copy, ";");
            char *id_str = strtok(NULL, ";");
            if (id_str) {
                int id = atoi(id_str);
                if (id > max_id) max_id = id;
            }
            free(copy);
        }
    }
    if (max_id < 0) {
        fclose(file);
        return NULL;
    }
    rewind(file);
    // 2. Allocation dynamique
    liberer_tout();
    nb_stations_global = max_id + 1;
    tableau_stations = calloc(nb_stations_global, sizeof(Station));
    if (!tableau_stations) {
        fclose(file);
        return NULL;
    }
    struct Graph *g = createGraph(nb_stations_global, 1);
    if (!g) {
        free(tableau_stations);
        tableau_stations = NULL;
        fclose(file);
        return NULL;
    }
    for (int i = 0; i < HASH_SIZE; i++) table_hachage[i] = NULL;
    // 3. Remplissage simultané (Graphe + Noms)
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') continue;

        char *line_copy = strdup(line);
        if (!line_copy) continue;
        char *type = strtok(line_copy, ";");

        if (type && strcmp(type, "STATION") == 0) {
            char *id_str = strtok(NULL, ";");
            char *nom = strtok(NULL, ";\n\r");

            if (id_str && nom) {
                int id = atoi(id_str);
                if (id >= 0 && id < nb_stations_global) {
                    char *tmp_nom = strdup(nom);
                    if (!tmp_nom) {
                        free(line_copy);
                        continue;
                    }

                    HashNode *hn = malloc(sizeof(HashNode));
                    if (!hn) {
                        free(tmp_nom);
                        free(line_copy);
                        continue;
                    }

                    tableau_stations[id].id = id;
                    tableau_stations[id].nom = tmp_nom;
                    // Insertion dans la table de hachage
                    int h = hash(tmp_nom);
                    hn->station = &tableau_stations[id];
                    hn->next = table_hachage[h];
                    table_hachage[h] = hn;
                }
            } else {
                fprintf(stderr, "Avertissement : Ligne station malformée ignorée.\n");
            }

        } else if (type && strcmp(type, "EDGE") == 0) {
            char *s_str = strtok(NULL, ";");
            char *d_str = strtok(NULL, ";");
            char *t_str = strtok(NULL, ";\n\r");
            if (s_str && d_str && t_str) {
                addEdge(g, atoi(s_str), atoi(d_str), atoi(t_str));
            }
        }
        free(line_copy);
    }
    fclose(file);
    return g;
}

void liberer_tout() {
    // Libérer les noms des stations
    for (int i = 0; i < nb_stations_global; i++) {
        free(tableau_stations[i].nom);
    }
    free(tableau_stations);
    tableau_stations = NULL;

    // Libérer les HashNodes
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode *curr = table_hachage[i];
        while (curr) {
            HashNode *tmp = curr;
            curr = curr->next;
            free(tmp);
        }
        table_hachage[i] = NULL;
    }

    nb_stations_global = 0;
}
