/*Mover archivos - 2 Argumentos Origen, Destino . Usando link y unlink*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int fd;

    if (argc != 3){
        perror("Debe introducir 2 argimentos, Origen y Destino\n");
        exit(-1);
    }
    if(link(argv[1],argv[2])==-1){
        perror("Link error\n");
        exit(-1);
    }
    if(unlink(argv[1])==-1){
        perror("Unlink error\n");
        exit(-1);
    }
    return 0;
}