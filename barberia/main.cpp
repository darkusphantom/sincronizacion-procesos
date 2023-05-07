#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N_CLIENTES 10 // Número de clientes
#define N_SILLAS 4    // Número de sillas de espera

sem_t sem_sillas;        // Semáforo para las sillas de espera
sem_t sem_barbero;       // Semáforo para el barbero
sem_t sem_cliente;       // Semáforo para los clientes
int sillas_ocupadas = 0; // Número de sillas ocupadas

void *barbero(void *arg)
{
    while (1)
    {
        sem_wait(&sem_cliente); // Espera a que un cliente llegue
        sem_wait(&sem_sillas);  // Bloquea el acceso a las sillas
        sillas_ocupadas--;      // Libera una silla
        sem_post(&sem_barbero); // Avisa al cliente de que está listo
        sem_post(&sem_sillas);  // Desbloquea el acceso a las sillas
        printf("El barbero está cortando el pelo de un cliente.\n");
        sleep(3); // El barbero corta el pelo del cliente
    }
    return NULL;
}

void *cliente(void *arg)
{
    int id = *(int *)arg;
    printf("Cliente %d llega a la barbería.\n", id);
    sem_wait(&sem_sillas); // Bloquea el acceso a las sillas
    if (sillas_ocupadas < N_SILLAS)
    {                      // Si hay sillas libres
        sillas_ocupadas++; // Ocupa una silla
        printf("Cliente %d se sienta en una silla de espera.\n", id);
        sem_post(&sem_cliente); // Avisa al barbero de que hay un cliente
        sem_post(&sem_sillas);  // Desbloquea el acceso a las sillas
        sem_wait(&sem_barbero); // Espera a que el barbero esté listo
        printf("Cliente %d se está cortando el pelo.\n", id);
    }
    else
    { // Si no hay sillas libres
        printf("La barbería está llena, cliente %d se va.\n", id);
        sem_post(&sem_sillas); // Desbloquea el acceso a las sillas
    }
    return NULL;
}

int main()
{
    pthread_t tid_barbero, tid_clientes[N_CLIENTES];
    int ids_clientes[N_CLIENTES];
    sem_init(&sem_sillas, 0, N_SILLAS);
    sem_init(&sem_barbero, 0, 0);
    sem_init(&sem_cliente, 0, 0);
    pthread_create(&tid_barbero, NULL, barbero, NULL);
    for (int i = 0; i < N_CLIENTES; i++)
    {
        ids_clientes[i] = i;
        pthread_create(&tid_clientes[i], NULL, cliente, &ids_clientes[i]);
        sleep(1); // Espera un segundo entre la llegada de cada cliente
    }
    pthread_join(tid_barbero, NULL);
    for (int i = 0; i < N_CLIENTES; i++)
    {
        pthread_join(tid_clientes[i], NULL);
    }
    return 0;
}