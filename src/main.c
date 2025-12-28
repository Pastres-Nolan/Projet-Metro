#include <stdio.h>

#include "menu.h"

int main(int argc, char **argv){

    if (argc != 2){
        printf("1 argument attendu\n");
        return -1;
    }

    menu(argv[1]);
    return 0;
}
