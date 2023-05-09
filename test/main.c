#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N_CLIENTS 30
#define N_TABLE 20

sem_t sem_table;
sem_t sem_waiter;
sem_t sem_client;
int busy_table = 0;

void *waiter(void *arg)
{
    while (1)
    {
        sem_wait(&sem_client);
        sem_wait(&sem_table);
        busy_table--;
        sem_post(&sem_waiter);
        sem_post(&sem_table);
        printf("El barbero está cortando el pelo de un cliente.\n");
        sleep(3);
    }
    return NULL;
}

void *client(void *arg)
{
    int id = *(int *)arg;
    printf("Cliente %d llega a la barbería.\n", id);
    sem_wait(&sem_table);
    if (busy_table < N_TABLE)
    {
        busy_table++;
        printf("Cliente %d se sienta en una silla de espera.\n", id);
        sem_post(&sem_client);
        sem_post(&sem_table);
        sem_wait(&sem_waiter);
        printf("Cliente %d se está cortando el pelo.\n", id);
    }
    else
    {
        printf("La barbería está llena, cliente %d se va.\n", id);
        sem_post(&sem_table);
    }
    return NULL;
}

int main()
{
    pthread_t tid_waiter, tid_clients[N_CLIENTS];
    int ids_clients[N_CLIENTS];
    sem_init(&sem_table, 0, N_TABLE);
    sem_init(&sem_waiter, 0, 0);
    sem_init(&sem_client, 0, 0);
    pthread_create(&tid_waiter, NULL, waiter, NULL);
    for (int i = 0; i < N_CLIENTS; i++)
    {
        ids_clients[i] = i;
        pthread_create(&tid_clients[i], NULL, client, &ids_clients[i]);
        sleep(1);
    }
    pthread_join(tid_waiter, NULL);
    for (int i = 0; i < N_CLIENTS; i++)
    {
        pthread_join(tid_clients[i], NULL);
    }
    return 0;
}