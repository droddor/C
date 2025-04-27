#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int fd; //variable para guardar el valor del descriptor

    if((fd = open(argv[1], O_RDWR)) == -1){
        perror("open");
        exit(-1);
    }
    printf("El archivo abierto tiene el descriptor: %d\n",fd);
    close(fd); //Cerramos siempre el archivo

return 0;
}
