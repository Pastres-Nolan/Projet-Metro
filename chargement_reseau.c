#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_STATIONS 1000


typedef struct{
    int id;
    char ville[100];
}Station;

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
    Station stations[MAX_STATIONS];

    char buffer[1024];
    char line[2048];
    int line_index = 0;
    int n = 0;
    int nb_octet_read;

    while ((nb_octet_read = read(fd, buffer, sizeof(buffer))) > 0) {

        for (int i = 0; i < nb_octet_read; i++) {
            char c = buffer[i];

            // Construction de la ligne
            if (c != '\n') {
                line[line_index++] = c;
            } else {
                line[line_index] = '\0'; // on le remet en string 

             
                if (line_index > 0 && line[0] != '#') {
                    char *type = strtok(line, " ;");
                    char* id_str = strtok(NULL," ;");
                    char* ville = strtok(NULL,"");

                    if (id_str && ville && type && strcmp(type, "STATION") == 0 && n< MAX_STATIONS){ 

                        stations[n].id = atoi(id_str);
                        strncpy(stations[n].ville, ville, sizeof(stations[n].ville)-1);
                        stations[n].ville[sizeof(stations[n].ville) - 1] = '\0';
                        n++;
                    }
                }

                line_index = 0; //reset l'index
            }
        }
    }
    for (int i = 0; i < n; i++) {
    printf("Station %d : id = %d, ville = %s\n",
           i,
           stations[i].id,
           stations[i].ville);
    }

    
    close(fd);
    return n;
}
