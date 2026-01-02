#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "edges.h"


// Function to create a new node
struct Node* createNode(int v) {
    struct Node* newNode = malloc(sizeof(struct Node));
    if (!newNode) {
        perror("Erreur malloc Node");
        exit(EXIT_FAILURE);
    }
    newNode -> vertex = v;
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

static int edgeExists(struct Graph* graph, int src, int dest) {
    struct Node* temp = graph->adjLists[src];
    while (temp) {
        if (temp->vertex == dest) return 1;
        temp = temp->next;
    }
    return 0;
}

// Function to add an edge to the graph
void addEdge(struct Graph* graph, int src, int dest) {
    if (src < 0 || src >= graph->numVertices || dest < 0 || dest >= graph->numVertices) {
        return;
    }

    if (edgeExists(graph, src, dest)) return;

    // Add edge from src to dest
    struct Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

    // If the graph is undirected, add an edge from dest to src as well
    if (!graph->isDirected) {
        if (!edgeExists(graph, dest, src)) {
            struct Node* newNodeRev = createNode(src);
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
                printf(" %d ->", temp->vertex);
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



struct Graph* charger_graphe(const char *nom_fichier) {
    FILE *file = fopen(nom_fichier, "r");
    if (!file) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier '%s'\n", nom_fichier);
        return NULL;
    }
    char line[2048];
    int max_id = -1;

    // 1. Déterminer le nombre de stations (max_id)
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;
        char *line_copy = strdup(line);
        char *type = strtok(line_copy, ";");
        if (type && strcmp(type, "STATION") == 0) {
            char *id_str = strtok(NULL, ";");
            if (id_str && strlen(id_str) > 0) {
                int id = atoi(id_str);
                if (id > max_id) max_id = id;
            }
        }
        free(line_copy);
    }

    if (max_id == -1) {
        fclose(file);
        return NULL;
    }

    // 2. Allocation dynamique
    struct Graph* g = createGraph(max_id + 1, 1);
    int *stations_definies = calloc(max_id + 1, sizeof(int));

    rewind(file);

    // 3. Remplissage et vérification d'erreurs
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;

        char *type = strtok(line, ";");
        if (!type) continue;

        if (strcmp(type, "STATION") == 0) {
            char *id_str = strtok(NULL, ";");
            if (id_str && strlen(id_str) > 0) {
                stations_definies[atoi(id_str)] = 1;
            }
        }
        else if (strcmp(type, "EDGE") == 0) {
            char *src_str = strtok(NULL, ";");
            char *dest_str = strtok(NULL, ";");
            if (src_str && dest_str) {
                int src = atoi(src_str);
                int dest = atoi(dest_str);

                // Erreur : station référencée, mais non définie
                if (src > max_id || dest > max_id || !stations_definies[src] || !stations_definies[dest]) {
                    fprintf(stderr, "Erreur : Arête entre stations non définies (%d-%d)\n", src, dest);
                } else {
                    addEdge(g, src, dest);
                }
            }
        }
    }

    free(stations_definies);
    fclose(file);
    return g;
}


