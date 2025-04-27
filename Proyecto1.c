//PROYECTO BARCOS PUERTO

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
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define MAX_BARCOS 3 //Numero maximo de barcos en en esclusa
#define TOTAL_BARCOS 12 //Total de barcos por poner un max, se puede poner infinito si hacemos un while true
#define TIMEOUT 10 //Time out segun requisitos

//Variables globales
pthread_mutex_t mutex; //mutex

//Semáforos generales
sem_t sem_barcosEsperando, sem_barcosEnEsclusa; 

//Arrays para almacenar los barcos en espera
int cola_entrada[MAX_BARCOS];
int cola_salida[MAX_BARCOS]; 

//Contadores de barcos en cada cola
int count_entrada = 0, count_salida = 0; 

//Estructura de barco con id y direccion
typedef struct {
    int id; 
    int direccion; //0 ENTRADA - 1 SALIDA
} Barco;

//Funcion barco
void *barco_func(void *arg) {
    Barco *barco = (Barco *)arg;
    int dir = barco->direccion;
    time_t start_time = time(NULL);

    //Region critica
    pthread_mutex_lock(&mutex);
    if (dir == 0 && count_entrada < MAX_BARCOS) {
        cola_entrada[count_entrada++] = barco->id;
    } else if (dir == 1 && count_salida < MAX_BARCOS) {
        cola_salida[count_salida++] = barco->id;
    }
    printf("[BARCO %d] Solicita acceso para %s.\n", barco->id, dir == 0 ? "ENTRAR" : "SALIR");
    pthread_mutex_unlock(&mutex);

    sem_post(&sem_barcosEsperando); //Indica que hay barcos esperando

    //Esperamos a que la esclusa se llene o cumpla el timeout de los requisitos 10sec
    while (time(NULL) - start_time < TIMEOUT) {
        sem_wait(&sem_barcosEnEsclusa); //Esperando que se procesen
        pthread_exit(NULL);
    }
    //printf("[BARCO %d] Timeout esperando la esclusa. Reintentará más tarde.\n", barco->id);
    pthread_exit(NULL);
}

//Funcion esclusa
void *esclusa_func(void *arg) {
    while (1) {
        sem_wait(&sem_barcosEsperando); //Esperando barcos
        pthread_mutex_lock(&mutex); //bloqueo de semaforo para procesar
        
        //Procesamos barcos de entrada si hay suficientes
        if (count_entrada == MAX_BARCOS) {
            printf("[ESCLUSA] Activada en dirección ENTRADA con %d barcos\n", MAX_BARCOS);
            printf("[ESCLUSA] Igualando nivel del agua...\n");
            sleep(2); //requisito 2 segundos para igualar el nivel de agua
            printf("[ESCLUSA] Barcos %d, %d, %d han ENTRADO al puerto.\n", cola_entrada[0], cola_entrada[1], cola_entrada[2]);
            count_entrada = 0; //Reinicio de cola de entrada
            sem_post(&sem_barcosEnEsclusa);
            sem_post(&sem_barcosEnEsclusa);
            sem_post(&sem_barcosEnEsclusa);
        } 
        //Procesamos barcos de salida si hay sificientes
        else if (count_salida == MAX_BARCOS) {
            printf("[ESCLUSA] Activada en dirección SALIDA con %d barcos\n", MAX_BARCOS);
            printf("[ESCLUSA] Igualando nivel del agua...\n");
            printf("[ESCLUSA] Barcos %d, %d, %d han SALIDO del puerto.\n", cola_salida[0], cola_salida[1], cola_salida[2]);
            count_salida = 0; //Reinicio cola de salisa
            sem_post(&sem_barcosEnEsclusa);
            sem_post(&sem_barcosEnEsclusa);
            sem_post(&sem_barcosEnEsclusa);
        }
        pthread_mutex_unlock(&mutex);
    }
}

int main() {
    pthread_t barcos[TOTAL_BARCOS], esclusa;
    pthread_mutex_init(&mutex, NULL);
    sem_init(&sem_barcosEsperando, 0, 0);
    sem_init(&sem_barcosEnEsclusa, 0, 0);

    srand(time(NULL)); //Semilla para rand()
    
    //Hilo para esclusa
    pthread_create(&esclusa, NULL, esclusa_func, NULL); 

    //Se crean barcos con id contador y direccion aleatoria
    for (int i = 0; i < TOTAL_BARCOS; i++) {
        Barco *barco = malloc(sizeof(Barco));
        barco->id = i + 1;
        barco->direccion = rand() % 2; //Randon para entrada salida 0,1
        pthread_create(&barcos[i], NULL, barco_func, barco);
        usleep(100000); //Generamos pequeño retraso para evitar conflicto
    }

    //Esperamos que finalicen todos los hilos/barcos
    for (int i = 0; i < TOTAL_BARCOS; i++) {
        pthread_join(barcos[i], NULL);
    }

    //Cancelar hilos
    //pthread_cancel(esclusa);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&sem_barcosEsperando);
    sem_destroy(&sem_barcosEnEsclusa);
    return 0;
}
