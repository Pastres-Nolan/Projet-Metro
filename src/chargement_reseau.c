#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "chargement_reseau.h"
#include "edges.h"

typedef struct {
    int id;
    char *nom;
} Station;

typedef struct HashNode {
    Station *station;
    struct HashNode *next;
} HashNode;

// Table de hachage pour la recherche par nom
static HashNode *table_hachage[1024];
// Tableau dynamique pour la recherche par ID
static Station *tableau_stations = NULL;
static int nb_stations_global= 0;


static int hash(const char *nom) {
    int sum = 0;
    while (*nom) sum += *nom++;
    return sum % 1024;
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

struct Graph* charger_reseau(const char *nom_fichier) {
    FILE *file = fopen(nom_fichier, "r");
    if (!file) return NULL;
    // 1. Déterminer le nombre de stations (max_id)
    char line[2048];
    int max_id = -1;
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'S') {
            char *copy = strdup(line);
            char *t = strtok(copy, ";");
            char *i = strtok(NULL, ";");
            if (t && i && strcmp(t, "STATION") == 0) {
                int id = atoi(i);
                if (id > max_id) max_id = id;
            }
            free(copy);
        }
    }

    rewind(file);
    // 2. Allocation dynamique
    nb_stations_global = max_id + 1;
    tableau_stations = calloc(nb_stations_global, sizeof(Station));
    struct Graph *g = createGraph(nb_stations_global, 1);
    for (int i = 0; i < 1024; i++) table_hachage[i] = NULL;
    // 3. Remplissage simultané (Graphe + Noms)
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') continue;

        char *line_copy = strdup(line);
        char *type = strtok(line_copy, ";");

        if (type && strcmp(type, "STATION") == 0) {
            char *id_str = strtok(NULL, ";");
            char *nom = strtok(NULL, ";\n\r");

            if (id_str && nom) {
                int id = atoi(id_str);
                if (id >= 0 && id < nb_stations_global) {

                    tableau_stations[id].id = id;
                    tableau_stations[id].nom = strdup(nom);
                    // Insertion dans la table de hachage
                    int h = hash(nom);
                    HashNode *hn = malloc(sizeof(HashNode));
                    hn->station = &tableau_stations[id];
                    hn->next = table_hachage[h];
                    table_hachage[h] = hn;
                }
            }
        } else if (strcmp(type, "EDGE") == 0) {
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



void afficher_info_station(struct Graph *graph) {
    char input[100];
    printf("Entrez un id ou un nom : ");
    scanf(" %99[^\n]", input);

    char *endptr;
    long val = strtol(input, &endptr, 10);
    int id = -1;

    if (*endptr == '\0') id = (int)val;
    else id = chercher_id_par_nom(input);

    if (id >= 0 && id < nb_stations_global && tableau_stations[id].nom) {
        printf("\nStation : %s (ID: %d)\nDegré : %d\n",
               tableau_stations[id].nom, id, degreSortant(graph, id));
    } else printf("Station '%s' inconnue.\n", input);
}

void afficher_voisins_station(struct Graph *graph) {
    char input[100];
    printf("Entrez l'ID ou le Nom de la station pour lister ses voisins : ");
    scanf(" %99[^\n]", input);

    char *endptr;
    int id = (int)strtol(input, &endptr, 10);
    if (*endptr != '\0') {
        id = chercher_id_par_nom(input);
    }

    if (id >= 0 && id < nb_stations_global && tableau_stations[id].nom) {
        printf("\n--- Voisins de la station %s (ID: %d) ---\n", tableau_stations[id].nom, id);
        struct Node* temp = graph->adjLists[id];

        if (temp == NULL) {
            printf("Cette station n'a aucun voisin direct.\n");
            return;
        }

        while (temp) {
            int v_id = temp->vertex;
            char* v_nom = tableau_stations[v_id].nom ? tableau_stations[v_id].nom : "Inconnu";
            printf(" %d - %s (%d min)\n", v_id, v_nom, temp->weight);
            temp = temp->next;
        }
    } else {
        printf("Station '%s' inconnue.\n", input);
    }
}


void liberer_tout() {
    for (int i = 0; i < nb_stations_global; i++) free(tableau_stations[i].nom);
    free(tableau_stations);
    // + libération des HashNodes (boucle simple)
}
