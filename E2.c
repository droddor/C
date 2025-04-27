#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define STDOUT 1
#define SIZE 512 //constante para el tamaño del buffer

int main(int argc, char *argv[]){
    int fd, r;
    
    char buffer[SIZE]; //definimos un buffer donde se metera el contenido del archivo
    if ((fd = open (argv[1], O_RDWR))== -1){
        perror("open");
        exit(-1);
    }
    //QUE HAY QUE LEER, DONDE LO METEMOS, QUE TAMAÑO
    r = read (fd, buffer, SIZE);
    //DONDE LO ESCRIBIMOS - POR PANTALLA, EL QUE, QUIEN LO HA LEIDO
    write(STDOUT, buffer, r);
    close(fd);
return 0;
}
