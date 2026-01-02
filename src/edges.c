#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "edges.h"


// Function to create a new node
struct Node* createNode(int v, int weight) {
    struct Node* newNode = malloc(sizeof(struct Node));
    if (!newNode) {
        perror("Erreur malloc Node");
        exit(EXIT_FAILURE);
    }
    newNode -> vertex = v;
    newNode->weight = weight;
    newNode -> next = NULL;
    return newNode;
}

// Function to create a graph
struct Graph* createGraph(int vertices, int isDirected) {
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;

    graph->numVertices = vertices;
    graph->isDirected = isDirected;

    // Create an array of adjacency lists
    graph->adjLists = malloc(vertices * sizeof(struct Node*));

    if (!graph->adjLists) {
        free(graph);
        return NULL;
    }

    // Initialize each adjacency list as empty
    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
    }

    return graph;
}

static int edgeExists(const struct Graph* graph, int src, int dest) {
    struct Node* temp = graph->adjLists[src];
    while (temp) {
        if (temp->vertex == dest) return 1;
        temp = temp->next;
    }
    return 0;
}

// Function to add an edge to the graph
void addEdge(struct Graph* graph, int src, int dest, int weight) {
    if (src < 0 || src >= graph->numVertices || dest < 0 || dest >= graph->numVertices) {
        return;
    }

    if (edgeExists(graph, src, dest)) return;

    // Add edge from src to dest
    struct Node* newNode = createNode(dest, weight);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

    // If the graph is undirected, add an edge from dest to src as well
    if (!graph->isDirected) {
        if (!edgeExists(graph, dest, src)) {
            struct Node* newNodeRev = createNode(src, weight);
            newNodeRev->next = graph->adjLists[dest];
            graph->adjLists[dest] = newNodeRev;
        }
    }
}

// Function to print the adjacency list representation of the graph
void printGraph(struct Graph* graph) {
    if (!graph) return;
    printf("Vertex:  Adjacency List\n");
    for (int v = 0; v < graph->numVertices; v++) {
        struct Node* temp = graph->adjLists[v];
        if (temp) {
            printf("Station %d :", v);
            while (temp) {
                printf(" %d(%dmn) ->", temp->vertex, temp->weight);
                temp = temp->next;
            }
            printf(" NULL \n");
        }
    }
}


int degreSortant(struct Graph* graph, int station){
    if (!graph || station < 0 || station >= graph->numVertices) return 0;
    int compteur = 0;
    struct Node* temp = graph->adjLists[station];

    while(temp) {
        compteur++;
        temp = temp->next;
    }
    return compteur;
}

void freeGraph(struct Graph* graph) {
    if (graph == NULL) return;

    // Pour chaque station, on libère la liste chaînée de voisins
    for (int i = 0; i < graph->numVertices; i++) {
        struct Node* temp = graph->adjLists[i];
        while (temp) {
            struct Node* toDelete = temp;
            temp = temp->next;
            free(toDelete); // Libère chaque maillon
        }
    }

    // On libère le tableau de pointeurs
    free(graph->adjLists);
    // On libère la structure principale
    free(graph);
}

