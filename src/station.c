#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/graphe.h"
#include "../include/station.h"

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
    if (tableau_stations != NULL && id >= 0 && id < nb_stations_global && tableau_stations[id].nom != NULL) return tableau_stations[id].nom;
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
            if (!copy) {
                perror("Erreur : strdup copy");
                liberer_tout();
                fclose(file);
                return NULL;
            }
            strtok(copy, ";");
            char *id_str = strtok(NULL, ";");
            if (id_str) {
                long id = strtol(id_str, NULL, 10);
                if (id > max_id) max_id = (int)id;
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
        perror("Erreur : calloc tableau_stations");
        fclose(file);
        return NULL;
    }
    struct Graph* graph = createGraph(nb_stations_global, 1);
    if (!graph) {
        perror("Erreur : malloc graph");
        liberer_tout();
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < nb_stations_global; i++) {
        tableau_stations[i].id = i;
        tableau_stations[i].nom = NULL; // Sera rempli par strdup ou restera NULL
    }

    // 3. Remplissage simultané (Graphe + Noms)
    int lineno = 0;
    while (fgets(line, sizeof(line), file)) {
        lineno++;
        if (line[0] == '#' || line[0] == '\n') continue;
        if (strncmp(line, "STATION", 7) == 0) {
            if (station_valide(line, lineno) < 0) {
                fprintf(stderr, "Erreur : lors du chargement des stations\n");
                freeGraph(graph);
                liberer_tout();
                fclose(file);
                return NULL;
            }
        } else if (strncmp(line, "EDGE", 4) == 0) {
            edge_valide (line, lineno, graph);
        } else {
            fprintf(stderr, "Avertissement ligne %d : ligne non reconnue\n", lineno);
        }
    }
    fclose(file);
    return graph;
}

int station_valide (char *line, int lineno) {
    strtok(line, ";");
    char *id_str = strtok(NULL, ";");
    char *nom = strtok(NULL, ";\n\r");

    if (!id_str || !nom || strlen(nom) == 0) {
        fprintf(stderr, "Avertissement ligne %d : Ligne STATION malformée ignorée\n", lineno);
        return 1;
    }

    char *end;
    long id_long = strtol(id_str, &end, 10);
    if (*end != '\0' || id_long < 0 || id_long >= nb_stations_global) {
        fprintf(stderr, "Avertissement ligne %d : ID STATION invalide (%s)\n", lineno, id_str);
        return 1;
    }

    int id = (int)id_long;
    if (tableau_stations[id].nom != NULL) {
        fprintf(stderr, "Avertissement ligne %d : ID de station en double détecté (%d). L'ancien nom '%s' sera remplacé par '%s'.\n", lineno, id, tableau_stations[id].nom, nom);
        free(tableau_stations[id].nom);
    }

    char *tmp_nom = strdup(nom);
    if (!tmp_nom) {
        perror("Erreur : strdup nom");
        return -1;
    }

    HashNode *hn = malloc(sizeof(HashNode));
    if (!hn) {
        perror("Erreur : malloc HashNode");
        free(tmp_nom);
        return -1;
    }

    tableau_stations[id].id = id;
    tableau_stations[id].nom = tmp_nom;
    // Insertion dans la table de hachage
    int h = hash(tmp_nom);
    hn->station = &tableau_stations[id];
    hn->next = table_hachage[h];
    table_hachage[h] = hn;

    // en cas de problème imprévu
    if (!tableau_stations[id].nom || tableau_stations[id].id != id) {
        fprintf(stderr, "Avertissement ligne %d : Problème inattendu avec STATION\n", lineno);
        return 1;
    }
    return 0;
}

int edge_valide (char *line, int lineno, struct Graph* graph) {
    strtok(line, ";");
    char *s_str = strtok(NULL, ";");
    char *d_str = strtok(NULL, ";");
    char *t_str = strtok(NULL, ";\n\r");

    if (!s_str || !d_str || !t_str || strlen(s_str) == 0 || strlen(d_str) == 0 || strlen(t_str) == 0) {
        fprintf(stderr, "Avertissement ligne %d : Ligne EDGE malformée ignorée\n", lineno);
        return 1;
    }

    char *end;
    long src_long = strtol(s_str, &end, 10);
    if (*end != '\0' || src_long < 0 || src_long >= nb_stations_global) {
        fprintf(stderr, "Avertissement ligne %d : EDGE ignorée, src invalide (%s)\n", lineno, s_str);
        return 1;
    }
    int src = (int)src_long;

    long dest_long = strtol(d_str, &end, 10);
    if (*end != '\0' || dest_long < 0 || dest_long >= nb_stations_global) {
        fprintf(stderr, "Avertissement ligne %d : EDGE ignorée, dest invalide (%s)\n", lineno, d_str);
        return 1;
    }
    int dest = (int)dest_long;

    if (src == dest) {
        fprintf(stderr, "Avertissement ligne %d : EDGE ignorée (%d -> %d) : source = destination\n", lineno, src, dest);
        return 1;
    }

    if (!tableau_stations[src].nom || !tableau_stations[dest].nom) {
        fprintf(stderr, "Avertissement ligne %d : EDGE ignorée (%d -> %d) : station inexistante\n", lineno, src, dest);
        return 1;
    }

    float weight = strtof(t_str, &end);
    if (end == t_str || *end != '\0') {
        fprintf(stderr, "Avertissement ligne %d : EDGE ignorée (%d -> %d) : poids invalide (%s)\n", lineno, src, dest, t_str);
        return 1;
    }

    if (weight <= 0.0f) {
        fprintf(stderr, "Avertissement ligne %d : EDGE ignorée (%d -> %d) : poids nul ou négatif (%f)\n", lineno, src, dest, weight);
        return 1;
    }

    if (edgeExists(graph, src, dest)) {
        fprintf(stderr, "Avertissement ligne %d : EDGE ignorée car déjà présente (%d -> %d)\n", lineno, src, dest);
        return 1; // edge ignorée
    }

    addEdge(graph, src, dest, weight);
    // en cas de problème imprévu
    if (src < 0 || dest < 0 || weight <= 0.0f) {
        fprintf(stderr, "Avertissement ligne %d : Problème inattendu avec EDGE\n", lineno);
        return 1;
    }

    return 0;
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
