#include <stdio.h>
#include <stdlib.h>
#include "menu.h"

int main(int argc, char **argv){

    if (argc != 2){
        return -1;
    }
     

    menu(argv[1]);
    return 0;
}
