#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STATIONS 1000

typedef struct {
    int start_id;
    int end_id;
    int time;
} Edge;


// Structure to represent a node in the adjacency list
struct Node {
    int vertex;
    struct Node* next;
};

// Structure to represent the graph
struct Graph {
    int numVertices;
    struct Node** adjLists;
    int isDirected;
};

// Function to create a new node
struct Node* createNode(int v) {
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

// Function to create a graph
struct Graph* createGraph(int vertices, int isDirected) {
    struct Graph* graph = malloc(sizeof(struct Graph));
    graph->numVertices = vertices;
    graph->isDirected = isDirected;

    // Create an array of adjacency lists
    graph->adjLists = malloc(vertices * sizeof(struct Node*));

    // Initialize each adjacency list as empty
    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
    }

    return graph;
}

// Function to add an edge to the graph
void addEdge(struct Graph* graph, int src, int dest) {
    // Add edge from src to dest
    struct Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

    // If the graph is undirected, add an edge from dest to src as well
    if (!graph->isDirected) {
        newNode = createNode(src);
        newNode->next = graph->adjLists[dest];
        graph->adjLists[dest] = newNode;
    }
}

// Function to print the adjacency list representation of the graph
void printGraph(struct Graph* graph) {
    printf("Vertex:  Adjacency List\n");
    for (int v = 0; v < graph->numVertices; v++) {
        if(graph->adjLists[v]==NULL){
            continue;
        }
        struct Node* temp = graph->adjLists[v];
        printf("%d --->", v);
        while (temp) {
            printf(" %d ->", temp->vertex);
            temp = temp->next;
    
        }
        printf(" NULL \n");
    }
}


int degreSortant(struct Graph* graph, int station){
    int compteur = 0;
    struct Node* temp = graph->adjLists[station]; 

    while( temp!= NULL){
        compteur++;
        temp = temp->next;
    }
    return compteur;

}


struct Graph* charger_graphe(const char *nom_fichier) {
    int fd = open(nom_fichier, O_RDONLY);
    if (fd < 0){
        printf("Fichier non reconnu \n");
        return NULL;
    }

    Edge edges[MAX_STATIONS];
    struct Graph* g = createGraph(MAX_STATIONS, 1);

    char buffer[1024];
    char line[2048];
    int line_index = 0;
    int n = 0;
    int nb_octet_read;

    while ((nb_octet_read = read(fd, buffer, sizeof(buffer))) > 0) {
        for (int i = 0; i < nb_octet_read; i++) {
            char c = buffer[i];
            if (c != '\n') {
                line[line_index++] = c;
            } else {
                line[line_index] = '\0';
                if (line_index > 0 && line[0] != '#') {
                    char* type = strtok(line, " ;");
                    char* start = strtok(NULL," ;");
                    char* end = strtok(NULL," ;");
                    char* time = strtok(NULL,"");
                    if (type && start && end && time && strcmp(type, "EDGE") == 0 && n < MAX_STATIONS){ 
                        edges[n].start_id = atoi(start);
                        edges[n].end_id = atoi(end);
                        edges[n].time = atoi(time);
                        addEdge(g, edges[n].start_id, edges[n].end_id);
                        n++;
                    }
                }
                line_index = 0;
            }
        }
    }

    close(fd);
    return g;
}

