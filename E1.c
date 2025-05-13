/*Probado Pipes*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//declaracion de funciones
void read_char(void);
void check_hit(void);

int fideles[2]; //array lectura/escritura

int main(void){
    int i,pid;

    pipe(fideles);

    pid = fork();

    if (pid == -1)
        exit(1);
    if(pid == 0){
        read_char();
    }else{
        check_hit();
    }
return 0;
}
void read_char(void){
    char c;
    char s[300];
    int num;

    printf("Entrando en en rutina para leer caracter ........\nEscribe un caracter: \n");
    c = getchar();

    while(c !='0'){
        if(c !='\n'){
            if((num = write(fideles[1], &c, 1))== -1){
                perror("write");
                exit(-1);
            }else
                printf("Hijo: Escribio el caracter: %c\n",c);
            
            printf("Hijo: Escribe un caracter: \n");
        }
        c = getchar();
    }
    write(fideles[1],&c,1);
}
void check_hit(void){
    char c;
    char s[300];
    int num;

    printf("Entrando en rutina para chequear .....\n");
    
    do{
        printf("Padre: Esperando\n");
        if((num = read(fideles[0],&c,1))==-1){
            perror("read");
        }else{
            if(c!='0')
                printf("Padre: %d bytes leidos \"%c\"\n",num,c);
        }
    }while(c!='0');
}
