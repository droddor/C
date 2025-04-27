//PROYECTO BARCOS PUERTO SEVILLA

//REQUISITOS
    //1 . Se opera en una direccion a la vez. Salida o entrada
    //2. Barco puede entrar o salir. 
    //3. Los barcos que quieran ir en direcion opuesta a la que toque deberan esperar
    //4. sistema de colas por direccion hasta que la esclusa este disponible para usar
    //5. Hay un proceso de igualacion de agua en la esclusa de 2 sec
    //6. El transito de entrada o salida se comienza cuando haya al menos 3 barcos preparados.
    //7. Si tras 10 sec no se ha completado el grupo, se pasa al otro que esta dispobible


#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define TOTAL_BARCOS 10
#define TAM_ESCLUSA 3//tamaño maximo de la esclusa

//estructura de barco
typedef struct {
    int id;
    int direccion; //puede ser 0 Entrada, 1 Salida
} Barco;

//variables globales
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t sem_esclusa;
int barcosEsperando[2] = {0,0}; //de salida o de entrada.
int direccionActual = -1;
int barcosEnEsclusa = 0;
int barcosProcesando[TAM_ESCLUSA];

void esperarGrupo(int dir){
    while(1){
        pthread_mutex_lock(&mutex);
        if(barcosEsperando[dir]>=TAM_ESCLUSA && (direccionActual == -1 || direccionActual == dir)){
            direccionActual = dir;
            printf("[ESCLUSA] Activada en direccion %s con %d barcos\n",dir == 0 ? "ENTRADA" : "SALIDA",TAM_ESCLUSA);
            printf("[ESCLUSA] Igualando nivel del agua...\n");
            barcosEsperando[dir]-= TAM_ESCLUSA;
            barcosEnEsclusa = TAM_ESCLUSA;
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
        usleep(100000);
    }
}
void *funcionBarco(void *arg){
    Barco *barco = (Barco *)arg;
    int dir = barco->direccion;

    pthread_mutex_lock(&mutex);
    barcosEsperando[dir]++;
    printf("[BARCO %d] Solicita acceso para %s.\n",barco->id,dir == 0? "ENTRAR":"SALIR");
    pthread_mutex_unlock(&mutex);

    esperarGrupo(dir);
    sem_wait(&sem_esclusa);

    pthread_mutex_unlock(&mutex);
    barcosProcesando[barcosEnEsclusa] = barco->id; //guarda id del barco
    barcosEnEsclusa--;
    if(barcosEnEsclusa == 0){
        printf("[ESCLUSA] Barcos %d, %d, %d han %s al puerto",barcosProcesando[0],barcosProcesando[1],barcosProcesando[2],dir == 0 ? "ENTRANDO" : "SALIENDO");

    }
    pthread_mutex_unlock(&mutex);
    sleep(2);

    pthread_mutex_lock(&mutex);
    
    if(barcosEnEsclusa == 0){
        printf("[ESCLUSA] Lista para el siguiente grupo de barcos.\n");
        direccionActual = -1;
    }
    pthread_mutex_unlock(&mutex);

    sem_post(&sem_esclusa);
    free(barco);
    pthread_exit(NULL);
}
int main(void){
    int totalBarcos = TOTAL_BARCOS;

    pthread_t barcos[TOTAL_BARCOS];
    pthread_mutex_init(&mutex,NULL);
    sem_init(&sem_esclusa,0,TAM_ESCLUSA);

    srand(time(NULL)); //semilla para rand()

    for(int i=0; i<TOTAL_BARCOS;i++){
        Barco *barco = malloc(sizeof(Barco));
        barco->id = i + 1;
        barco->direccion = rand()%2; //dara 0 o 1
        pthread_create(&barcos[i],NULL,funcionBarco,barco);
        sleep(1);
    }
    for(int i=0; i< TOTAL_BARCOS;i++){
        pthread_join(barcos[i],NULL);
    }
    pthread_mutex_destroy(&mutex);
    sem_destroy(&sem_esclusa);
    return 0;
    
}

