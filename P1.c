/*Mostar las caracteristicas de un archivo pasado por teclado*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

char *permisosF (mode_t); //prototipo de funcion

int main(int argc, char *argv[]){
    int fd,size;
    struct stat info;

    if((fd = open(argv[1], O_RDWR))== -1){
        perror("open");
        exit(-1);
    }
    stat(argv[1],&info);
    size = info.st_size;

    printf("Numero nodo-i: %d, size %d \n",(int)(info.st_ino),size);
    printf("Ultima modificacion: %s\n",ctime(&info.st_mtime));
    printf("Permisos en Bits: %04o \n",info.st_mode & 07777);
    printf("Permisos char USR-GRP-OTH: %s\n",permisosF(info.st_mode));
    printf("Numero inode: %lu\n",info.st_ino);
    printf("Propietario user-ID: %d\n",info.st_uid);
    printf("Propietario grupo-ID: %d\n",info.st_gid);
    printf("Enlaces: %ld\n",info.st_nlink);
    
    close(fd);

    return 0;
}
//Definicion de la funcion
char *permisosF(mode_t mode){
   
    static char permisos[10];

    strcpy(permisos, "---------");

    //USUARIO
    if(mode & S_IRUSR)
        permisos[0] = 'r';

    if(mode & S_IWUSR)
        permisos[1] = 'w';
    
    if(mode & S_IXUSR)
        permisos[2] = 'x';
    
    //GRUPO
    if(mode & S_IRGRP)
        permisos[3] = 'r';

    if(mode & S_IWGRP)
        permisos[4] = 'w';
    
    if(mode & S_IXGRP)
        permisos[5] = 'x';
    
    //OTHER
    if(mode & S_IROTH)
        permisos[6] = 'r';

    if(mode & S_IWOTH)
        permisos[7] = 'w';
    
    if(mode & S_IXOTH)
        permisos[8] = 'x';

    return permisos;
}