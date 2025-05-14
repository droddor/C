#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TAM 256
#define MAX 5


//prototipo
char *numToCard(int n); 

int main(void){

    char envio[TAM],buf[TAM];

    int cont=0,i=0,c=0;

    int aHijo[2],aPadre[2];

    pipe(aHijo);
    pipe(aPadre);

    //Proceso hijo

    if(fork()==0){

        close(aHijo[1]);
        close(aPadre[0]);

        for(;;){
            cont = read(aHijo[0],buf,TAM);
            if(cont ==0){
                printf("[HIJO] Pipe cerrada. Reibibidos %d\n",c);
                exit(0);
            }
            int num = atoi(buf);
            char *name = numToCard(num);

            printf("[HIJO] Redibido: %d -> %s\n",num,name);
            c++;
            write(aPadre[1],name,strlen(name)+1);

        }
    }
    //proceso padre

    close(aPadre[1]);
    close(aHijo[0]);

    for(i=0;i<MAX;i++){
        printf("[PADRE]: Introduce numero: \n");
        scanf("%s",envio);

        write(aHijo[1],envio,strlen(envio)+1);
        cont = read(aPadre[0],buf,TAM);

        printf("[PADRE] CARD: %s\n",buf);
    }

    printf("[PADRE]Finalizado. Leidas %d cards\n",i);
    close(aHijo[1]);
    close(aPadre[0]);
}
char *numToCard(int n){
    char *card[] = { "Corazones" , "treboles", "picas", "rombos"};
    if(n >=0 && n <= 4){
        return card[n];
    }
}