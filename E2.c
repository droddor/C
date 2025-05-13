#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){
    int pfds[2];
    char buf[30];

    pipe(pfds);

    if(!fork()){
        printf("HIJO: Escribiendo en pipe\n");
        write(pfds[1],"test",5);
        printf("HIJO: Saliendo\n");
        exit(0);
    }else{
        printf("PADRE: Leyendo desde pipe\n");
        read(pfds[0],buf,5);
        printf("PADRE: leido \"%s\"\n",buf);
    }
    return 0;
}
