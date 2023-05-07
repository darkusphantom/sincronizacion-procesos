#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5               // Número de procesos
#define MAX_DELAY 1000000 // Retardo máximo en microsegundos

int tiempo[N] = {0};     // Vector de tiempo de Lamport
int contador_global = 0; // Contador global compartido
sem_t semaforo[N];       // Vector de semáforos

void *proceso(void *id_ptr)
{
    int id = *(int *)id_ptr;
    int i, j, retardo;
    for (i = 0; i < 10; i++)
    {
        // Inicio sección crítica
        sem_wait(&semaforo[id]);
        tiempo[id] = contador_global + 1;
        contador_global = tiempo[id];
        sem_post(&semaforo[id]);
        for (j = 0; j < N; j++)
        {
            if (j != id)
            {
                sem_wait(&semaforo[j]);
                retardo = rand() % MAX_DELAY;
                usleep(retardo);
                if (tiempo[j] > tiempo[id])
                    tiempo[id] = tiempo[j];
                sem_post(&semaforo[j]);
            }
        }
        contador_global++;
        printf("Proceso %d entra en la sección crítica (tiempo de Lamport: %d).\n", id, tiempo[id]);
        // Sección crítica
        retardo = rand() % MAX_DELAY;
        usleep(retardo);
        printf("Proceso %d sale de la sección crítica (tiempo de Lamport: %d).\n", id, tiempo[id]);
        // Fin sección crítica
    }
    return NULL;
}

int main()
{
    pthread_t tid[N];
    int ids[N];
    int i;
    srand(time(NULL));
    for (i = 0; i < N; i++)
    {
        sem_init(&semaforo[i], 0, 1);
        ids[i] = i;
        pthread_create(&tid[i], NULL, proceso, &ids[i]);
    }
    for (i = 0; i < N; i++)
    {
        pthread_join(tid[i], NULL);
    }
    for (i = 0; i < N; i++)
    {
        sem_destroy(&semaforo[i]);
    }
    return 0;
}