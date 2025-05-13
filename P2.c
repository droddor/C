/*Comvierte cada caracter leido por telcado a Mayuscula
1 - Crea un proceso hijo
2 - Proceso padre pide datos, lee y envia los datos a traves de pipe al proceso hijo
3 - El proceso hijo traduce y convierte en minusculas y devuelve el resultado al padre por otra pipe
4 - El padre imprime por pantalla.asm

usar tolower(char c)

*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

//funciones

void gestion_usuario(int[],int[]); //recibir y devolver al usuario
void traductor(int[],int[]); //para recibir y enviar la traduccion

int main(void){
    //2 array que contentan los descriptoraes para 2 pipes
    int usuario_traduccion[2];
    int traduccion_usuario[2];
    int pid; //pid para proceso hijo - 0 fork
    int rc; //guardar valores de las rutinas

    rc = pipe(usuario_traduccion);

    if(rc==-1){
        perror("main: pipe usuario traduccion");
        exit(-1);
    }
    rc = pipe(traduccion_usuario);

    if (rc==-1){
        perror("main: pipe traduccion a usuario");
        exit(-1);
    }
    pid = fork();

    switch (pid){

        case -1: //falla el fork
            perror("main: fork");
            exit(-1);
        case 0 : //dentro del proceso hijo
            traductor(usuario_traduccion,traduccion_usuario);

        default: //dentro del proceso padre

            gestion_usuario(traduccion_usuario,usuario_traduccion); //linea B

    }
return 0;
}

void gestion_usuario(int input_pipe[],int output_pipe[]){
    int c;
    char ch;
    int rc;

    close(input_pipe[1]);
    close(output_pipe[0]);

    while((c = getchar()) > 0){
        ch = (char)c;

        rc = write(output_pipe[1],&ch,1);

        if(rc == -1){
            perror("usser gestion: write");
            close(input_pipe[0]);
            close(output_pipe[1]);
            exit(1);
        }

        rc = read (input_pipe[0],&ch,1);
        c = (int)ch;

        if(rc <= 0){
            perror("user gestion: read");
            close(input_pipe[0]);
            close(output_pipe[1]);
            exit(1);
        }
        putchar(c);

    }
    //cerrar pipes y salir
    close(input_pipe[0]);
    close(output_pipe[1]);
    exit(0);

}
void traductor(int input_pipe[],int output_pipe[]){
    int c;
    char ch;
    int rc;

    close(input_pipe[1]);
    close(output_pipe[0]);
    
    while(read(input_pipe[0],&ch,1) > 0){
        c = (int)ch;
        
        if(isascii(c) && isupper(c))
            c = tolower(c);

        ch = (char)c;

        rc = write(output_pipe[1], &ch, 1);

        if(rc == -1){
            perror("Traductor: write");
            close(input_pipe[0]);
            close(output_pipe[1]);
            exit(1);
        }
    }

    //cerrar pipes y salir
    close(input_pipe[0]);
    close(output_pipe[1]);
    exit(0);
}