/*" 2 Pipe para comunicar entre proceso padre e hijo. 
        1 - Padre lee cadenas y las envia al hijo.
        2 - Hijo lee cadenas del padre y las devuelve al padre traves de la 2da pipe
        3 - Se imprime por pantalla lo que se lee
"*/

#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define TAM 256
#define MAX 5

void main(void){
    char envio[TAM], buf[TAM];
    int cont=0, i=0, c=0, to_child[2],to_father[2];

    pipe(to_child); //de padre a hijo
    pipe(to_father); //de hijo a padre

    if (fork()==0){
        close(to_child[1]);
        close(to_father[0]);

        for(;;){
            if((cont = read(to_child[0],buf,TAM))==0){
                printf("[C] Pipe cerrada, se han recibido %d cadenas. \n",c);
                exit(3);
            }
            printf("[C] Recibido: %s\n",buf);
            c++;
            write(to_father[1],buf,cont);
        }
    }
    close(to_father[1]);
    close(to_child[0]);
    
    for(i=0;i<MAX;i++){
        printf("[F] Teclea la cadena a enviar: \n");
        scanf("%s",envio);
        cont = write(to_child[1],envio,strlen(envio)+1);
        c++;
        cont = read(to_father[0],envio,TAM);
        printf("[F] Redibido: %s\n",envio);
    }
    printf("[F] Recibidas %d cadenas. \n",c);
    close(to_child[1]);
    close(to_father[0]);
}