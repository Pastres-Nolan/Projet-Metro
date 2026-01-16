#ifndef EDGES_H
#define EDGES_H

// Structure de graphe et nœud
struct Node {
    int vertex;
    int weight;
    struct Node* next;
};

struct Graph {
    int numVertices;
    struct Node** adjLists;
    int isDirected;
};

// Prototypes des fonctions
struct Graph* createGraph(int vertices, int isDirected);
int edgeExists(const struct Graph* graph, int src, int dest);
void addEdge(struct Graph* graph, int src, int dest, float weight);
// void printGraph(struct Graph* graph); // pour imprimer la liste d'adjacence
int degreSortant(const struct Graph* graph, int station);
void freeGraph(struct Graph* graph);


#endif
