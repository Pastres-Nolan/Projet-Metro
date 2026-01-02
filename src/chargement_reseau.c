#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "chargement_reseau.h"

#define TABLE_SIZE 100

typedef struct {
    int id;
    char nom[100];
} Station;

typedef struct HashNode {
    Station station;
    struct HashNode *next;
} HashNode;

// Table de hachage pour la recherche par nom
static HashNode* table_noms[TABLE_SIZE];
// Tableau dynamique pour la recherche par ID
static Station* tableau_stations = NULL;
static int nb_stations_max = 0;


int hash(const char *nom) {
    int sum = 0;
    for (int i = 0; nom[i] != '\0'; i++) {
        sum += nom[i];
    }
    return sum % TABLE_SIZE;
}

void ajouter_station_hash(Station s) {
    int index = hash(s.nom);
    HashNode *node = malloc(sizeof(HashNode));
    node->station = s;
    node->next = table_noms[index];
    table_noms[index] = node;
}

void liberer_stations() {
    if (tableau_stations) free(tableau_stations);
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode *curr = table_noms[i];
        while (curr) {
            HashNode *temp = curr;
            curr = curr->next;
            free(temp);
        }
        table_noms[i] = NULL;
    }
}

int chercher_id_par_nom(const char *nom) {
    int index = hash(nom);
    HashNode *node = table_noms[index];
    while (node) {
        if (strcmp(node->station.nom, nom) == 0) {
            return node->station.id;
        }
        node = node->next;
    }
    return -1;
}

static void charger_donnees(const char *nom_fichier, int max_id) {
    FILE *file = fopen(nom_fichier, "r");
    if (!file) return;
    nb_stations_max = max_id + 1;
    tableau_stations = calloc(nb_stations_max, sizeof(Station));
    for (int i = 0; i < TABLE_SIZE; i++) table_noms[i] = NULL;

    char line[2048];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') continue;

        char *type = strtok(line, ";");
        if (type && strcmp(type, "STATION") == 0) {
            char *id_str = strtok(NULL, ";");
            char *nom = strtok(NULL, "\n\r");

            if (id_str && nom) {
                int id = atoi(id_str);
                if (id >= 0 && id < nb_stations_max) {
                    tableau_stations[id].id = id;
                    strncpy(tableau_stations[id].nom, nom, 99);
                    ajouter_station_hash(tableau_stations[id]);
                }
            }
        }
    }
    fclose(file);
}



int afficher_reseau(const char *nom_fichier, struct Graph *graph) {
    if (tableau_stations == NULL) {
        charger_donnees(nom_fichier, graph->numVertices - 1);
    }

    char input[100];
    printf("Entrez un id ou un nom : ");
    scanf(" %99[^\n]", input);

    char *endptr;
    long id = strtol(input, &endptr, 10); // char to int

    if (*endptr == '\0') {
        // Recherche par ID (Directe)
        if (id >= 0 && id < nb_stations_max && tableau_stations[id].nom[0] != '\0') {
            printf("ID : %d, Nom : %s, Voisins : %d\n",
                tableau_stations[id].id, tableau_stations[id].nom, degreSortant(graph, (int)id));
        } else printf("Station ID %ld non trouvée.\n", id);

    } else {
        // Recherche par Nom (via Hash Table)
        int id_trouve = chercher_id_par_nom(input);
        if (id_trouve != -1) {
            printf("\n--- Station trouvée (par Nom) ---\n");
            printf("ID : %d\nNom : %s\nDegré sortant : %d\n",
                   id_trouve, tableau_stations[id_trouve].nom, degreSortant(graph, id_trouve));
        } else printf("Erreur : Station '%s' non trouvée.\n", input);
    }
    return 0;
}
