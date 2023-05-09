#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N_CLIENTS 30
#define N_TABLE 20
#define N_WAITER 10

sem_t sem_table[N_TABLE];
sem_t sem_waiter[N_WAITER];
sem_t sem_client[N_CLIENTS];
// sem_t cash_box;

int table_free = 0;
int busy_waiter = 0;

int is_empty_cashbox = 0;

// Función que representa la atención al cliente en una mesa
void attend_table(int id_table)
{
    printf("Atendiendo al cliente en la mesa %d\n", id_table);
}

void *waiter_thread(void *arg)
{
    int id = *(int *)arg;
    // while (1)
    // {
    //     if (!is_empty_cashbox && busy_waiter < N_WAITER)
    //     {
    //         sem_wait(&sem_client);
    //         sem_wait(&sem_table);
    //         table_free--;
    //         sem_post(&sem_waiter);
    //         sem_post(&sem_table);
    //         // printf("El mesero está atendiendo a un cliente.\n");
    //         sleep(3);
    //     }
    //     else
    //     {
    //         printf("No hay meseros disponibles. Soliciten a uno.\n");
    //         sem_post(&sem_waiter);
    //     }
    // }

    while (1)
    {
        sem_wait(&sem_table[id]); // Esperar hasta que haya una mesa disponible
        attend_table(id);
        sem_post(&sem_table[id]); // Liberar la mesa para el siguiente cliente
    }

    return NULL;
}

void *client_thread(void *arg)
{
    int id = *(int *)arg;
    printf("Cliente %d llega al restaurante.\n", id);
    sem_wait(&sem_table);

    if (table_free < N_TABLE)
    {
        table_free++;
        printf("Cliente %d se sienta en una mesa.\n", id);
        sem_post(&sem_client);
        sem_post(&sem_table);
        sem_wait(&sem_waiter);
        printf("Cliente %d le están tomando el pedido.\n", id);
        // printf("Mesero se dirige a la registro de pedidos.\n");
        // if (!is_empty_cashbox && busy_waiter < N_WAITER)
        // {
        //     busy_waiter++;
        // }
        // else
        // {
        //     printf("Necesitamos meseros.\n\n");
        //     busy_waiter--;
        //     sem_post(&sem_waiter);
        // }
    }
    else
    {
        printf("No hay mesas disponibles, cliente %d se va.\n\n", id);
        sem_post(&sem_table);
    }

    return NULL;
}

int main()
{
    pthread_t tid_waiter[N_WAITER], tid_clients[N_CLIENTS];
    int ids_clients[N_CLIENTS], ids_waiter[N_WAITER];

    sem_init(&sem_table, 0, N_TABLE);
    sem_init(&sem_waiter, 0, 0);
    sem_init(&sem_client, 0, 0);

    for (int i = 0; i < N_WAITER; i++)
    {
        ids_waiter[i] = i;
        pthread_create(&tid_waiter[i], NULL, waiter_thread, &ids_waiter[i]);
        sleep(1);
    }

    for (int i = 0; i < N_CLIENTS; i++)
    {
        ids_clients[i] = i;
        pthread_create(&tid_clients[i], NULL, client_thread, &ids_clients[i]);
        sleep(1);
    }

    for (int i = 0; i < N_WAITER; i++)
    {
        pthread_join(tid_waiter[i], NULL);
    }

    for (int i = 0; i < N_CLIENTS; i++)
    {
        pthread_join(tid_clients[i], NULL);
    }
    return 0;
}