#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STATIONS 1000

typedef struct{
    int id;
    char ville[100];
} Station;

#define TABLE_SIZE 100 

typedef struct HashNode {
    Station station;
    struct HashNode *next;
} HashNode;


HashNode* table[TABLE_SIZE]; 

int hash(int id) {
    return id % TABLE_SIZE;
}

void ajouter_station_hash(Station s) {
    int index = hash(s.id);
    HashNode *node = malloc(sizeof(HashNode));
    node->station = s;
    node->next = table[index];
    table[index] = node;
}

Station* chercher_station(int id) {
    int index = hash(id);
    HashNode *node = table[index];
    while (node) {
        if (node->station.id == id) {
            return &node->station;
        }
        node = node->next;
    }
    return NULL; 
}



int afficher_reseau(const char *nom_fichier, struct Graph *graph)
{

    int fd = open(nom_fichier, O_RDONLY);
    if (fd < 0){
        printf("Fichier non reconnu \n");
        return -1;
    }

    Station stations[MAX_STATIONS];
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
                    char *type = strtok(line, " ;");
                    char *id_str = strtok(NULL, " ;");
                    char *ville = strtok(NULL, "");

                    if (type && id_str && ville &&
                        strcmp(type, "STATION") == 0 && n < MAX_STATIONS) {

                        stations[n].id = atoi(id_str);
                        strncpy(stations[n].ville, ville,
                                sizeof(stations[n].ville) - 1);
                        stations[n].ville[sizeof(stations[n].ville) - 1] = '\0';
                        n++;
                    }
                }
                line_index = 0;
            }
        }
    }

    close(fd);


    for(int i = 0; i < TABLE_SIZE; i++)
        table[i] = NULL;

    for(int i = 0; i < n; i++)
        ajouter_station_hash(stations[i]);

    char input[100];
    printf("Entrez un id ou un nom : ");
    scanf("%s", input);

    char *endptr;
    long id = strtol(input, &endptr, 10); // char to int

    if (*endptr == '\0') {

        Station *s = chercher_station((int)id);
        if (s)
            if (s) {
    printf("ID : %d\n", s->id);
    printf("Nom : %s\n", s->ville);
    printf("Degré sortant : %d\n",
           degreSortant(graph, s->id));
}
        else
            printf("Station id=%d non trouvée\n", (int)id);
    } else {
        
        for(int i=0;i<n;i++) {
            if(strcmp(stations[i].ville, input) == 0) {
                printf("ID : %d\n", stations[i].id);
                printf("Nom : %s\n", stations[i].ville);
                printf("Degré sortant : %d\n", degreSortant(graph, stations[i].id));
                }
        }
    }
   
    return 0;
}
