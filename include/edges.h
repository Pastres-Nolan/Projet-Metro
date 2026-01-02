#ifndef EDGES_H
#define EDGES_H

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
struct Graph* createGraph(int vertices, int isDirected);
void addEdge(struct Graph* graph, int src, int dest);
void printGraph(struct Graph* graph);
int degreSortant(struct Graph* graph, int station);
void freeGraph(struct Graph* graph);


#endif
