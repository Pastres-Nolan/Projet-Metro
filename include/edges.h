#ifndef EDGES_H
#define EDGES_H

#define MAX_STATIONS 1000

// Structure de graphe et nœud
struct Node {
    int vertex;
    struct Node* next;
};

struct Graph {
    int numVertices;
    struct Node** adjLists;
    int isDirected;
};

// Prototypes des fonctions
struct Node* createNode(int v);
struct Graph* createGraph(int vertices, int isDirected);
void addEdge(struct Graph* graph, int src, int dest);
void printGraph(struct Graph* graph);
int degreSortant(struct Graph* graph, int station);
struct Graph* charger_graphe(const char *nom_fichier);

#endif
