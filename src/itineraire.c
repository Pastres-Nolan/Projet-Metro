#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../include/itineraire.h"
#include "../include/graphe.h"
#include "../include/station.h"

#define INF INT_MAX

// Trouve le sommet avec la distance minimale non encore visité
static int trouver_min_distance(int dist[], int visite[], int nb_sommets) {
    int min = INF, min_index = -1;
    for (int v = 0; v < nb_sommets; v++) {
        if (visite[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}


// Affiche récursivement le chemin en remontant les prédécesseurs
static void afficher_chemin_recursif(int parent[], int j) {
    if (parent[j] == -1) {
        printf("%s (%d)", obtenir_nom_station(j), j);
        return;
    }
    afficher_chemin_recursif(parent, parent[j]);
    printf(" -> %s (%d)", obtenir_nom_station(j), j);
}

void dijkstra(struct Graph* graph, int start_node, int end_node) {
    if (!graph) {
        fprintf(stderr, "Erreur : graphe inexistant.\n");
        return;
    }
    int n = graph->numVertices;
    if (start_node < 0 || start_node >= n || end_node   < 0 || end_node   >= n) {
        fprintf(stderr, "Erreur : ID de station invalide.\n");
        return;
    }
    int *dist = malloc(n * sizeof(int));
    int *visite = malloc(n * sizeof(int));
    int *parent = malloc(n * sizeof(int));

    if (!dist || !visite || !parent) {
        free(dist); free(visite); free(parent);
        perror("Erreur : malloc Dijkstra");
        return;
    }
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        visite[i] = 0;
        parent[i] = -1;
    }

    dist[start_node] = 0;

    for (int count = 0; count < n - 1; count++) {
        int u = trouver_min_distance(dist, visite, n);
        if (u == -1) break;
        visite[u] = 1;
        if (u == end_node) break;

        for (struct Node *temp = graph->adjLists[u]; temp; temp = temp->next) {
            int v = temp->vertex;
            if (!visite[v] && dist[u] != INF && dist[u] + temp->weight < dist[v]) {
                dist[v] = dist[u] + temp->weight;
                parent[v] = u;
            }
        }
    }

    if (dist[end_node] == INF) {
        printf("\n--- Aucun chemin trouvé entre %d et %d. ---\n", start_node, end_node);
    } else {
        printf("\n--- Chemin minimal (Temps total : %d min) ---\n", dist[end_node]);
        afficher_chemin_recursif(parent, end_node);
        printf("\n");
    }

    free(dist);
    free(visite);
    free(parent);
}