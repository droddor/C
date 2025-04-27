#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

#define SIZE_BUFFER 512

int main(int argc, char *argv[]){
    char buffer[SIZE_BUFFER];

    printf("El directorio actual es: %s\n",getcwd(buffer,SIZE_BUFFER));
    //chdir - indica en que directorio vamos a operar. 
    //(".") Directorio actual. ("..")Directorio superior
    chdir( "." ); 

    //se crea directorio con permisos segun chmod
    mkdir( "./directorio1",0755); 
    mkdir( "./directorio2",0755);
    mkdir("./directorio3",0777);
    rmdir( "./directorio1" );

return 0;
}
/*
----Permisos CHMOD----
4 digitos - Octal

1º Siempre 0
2º Permisos usuario 
3º Permisos grupo

7 - rwx  Lectura + escritura + ejecion
6 - rw-  Lectura + escritura
5 - r-x  Lectura + ejecucion
4 - r--  Lectura
0 - ---  Sin permisos

*/