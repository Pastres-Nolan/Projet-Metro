#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "algorithmes.h"
#include "edges.h"
#include "chargement_reseau.h"

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
        printf("%d", j);
        return;
    }
    afficher_chemin_recursif(parent, parent[j]);
    printf(" -> %d", j);
}

void dijkstra(struct Graph* g, int start_node, int end_node) {
    int n = g->numVertices;
    int *dist = malloc(n * sizeof(int));
    int *visite = malloc(n * sizeof(int));
    int *parent = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        visite[i] = 0;
        parent[i] = -1;
    }

    dist[start_node] = 0;

    for (int count = 0; count < n - 1; count++) {
        int u = trouver_min_distance(dist, visite, n);
        if (u == -1 || u == end_node) break;

        visite[u] = 1;

        struct Node* temp = g->adjLists[u];
        while (temp) {
            int v = temp->vertex;
            if (!visite[v] && dist[u] != INF && dist[u] + temp->weight < dist[v]) {
                dist[v] = dist[u] + temp->weight;
                parent[v] = u;
            }
            temp = temp->next;
        }
    }

    if (dist[end_node] == INF) {
        printf("Aucun chemin trouvé entre %d et %d.\n", start_node, end_node);
    } else {
        printf("Chemin minimal (Temps total : %d min) :\n", dist[end_node]);
        afficher_chemin_recursif(parent, end_node);
        printf("\n");
    }

    free(dist);
    free(visite);
    free(parent);
}