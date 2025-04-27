#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

int main(int argc, char *argv[]){
    DIR *dir;
    struct dirent *mi_dirent;
    if(argc != 2){
        printf("%s: %s directorio\n",argv[0], argv[0]);
        exit(-1);
    }
    if((dir = opendir(argv[1])) == NULL){
        perror("opendir");
        exit(-1);
    }
    while((mi_dirent = readdir(dir)) != NULL)
        printf("%s\n",mi_dirent->d_name);
    closedir(dir);
    return 0;
}

/*
Execute with arg , relative path or complete:

Relative : ./
Complete: /documents/dirname

Example:

>>./E4 ./

*/
