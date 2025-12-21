#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_STATIONS 1000

typedef struct{
    int start_id;
    int end_id;
    int time;
}Edge;


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
        struct Node* temp = graph->adjLists[v];
        printf("%d --->", v);
        while (temp) {
            printf(" %d ->", temp->vertex);
            temp = temp->next;
        }
        printf(" NULL\n");  
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Doit prendre exactement 1 argument\n");
        return -1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0){
        printf("Fichier non reconnu \n");
        return -1;
    } 
    Edge edges[MAX_STATIONS];
    

    // Create a directed graph with 3 vertices
    struct Graph* directedGraph = createGraph(125, 1);

    // Add edges to the directed graph
    
    printf("\nAdjacecncy List for Directed Graph:\n");
    printGraph(directedGraph);


    
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
                    

                    if (start && end && time && type && strcmp(type, "EDGE") == 0 && n< MAX_STATIONS){ 

                        edges[n].start_id = atoi(start);
                        edges[n].end_id = atoi(end);
                        edges[n].time = atoi(time);
                        addEdge(directedGraph, edges[n].start_id, edges[n].end_id);
                        

                        n++;
                    }
                }

                line_index = 0; //reset l'index
            }
        }
    }
    for (int i = 0; i < n; i++) {
    printf("id départ : %d : id arrivé : %d, temps : %d\n",

        edges[i].start_id,
        edges[i].end_id,
        edges[i].time);
    }
    printf("\nAdjacecncy List for Directed Graph:\n");
    printGraph(directedGraph);

    
    close(fd);
    return n;
}
