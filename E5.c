#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

static void lookup(const char *arg){
    DIR *dirp;
    struct dirent *dp;

    if((dirp = opendir(".")) == NULL){
        perror("No se puede abir '.' ");
        return;
    }
    do{
        errno = 0;
        if ((dp = readdir(dirp)) != NULL){
            if(strcmp(dp->d_name, arg)==0){
                printf("Encontrado %s\n",arg);
                closedir(dirp);
                return;
            }
        }
    } while (dp != NULL);

    if (errno != 0)
        perror("Error leyendo el directorio");
    else
        printf("Fallo de busqueda %s\n",arg);
        closedir(dirp);
        return;
}
int main(int argc, char *argv[]){
    int i;
    for(i = 1; i< argc; i++){
        lookup(argv[i]);
    }
    return 0;
}


/*
Find dile in directory:

Example -  execute: >./E5 filaname.txt

*/