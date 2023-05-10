#ifndef WAITER_H
#define WAITER_H

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>
#include "table.h"

#define N_WAITER 10

typedef struct
{
    int id;
    int in_cashbox;
    int total_orders_claimed;
} Waiter;
Waiter waiter[N_WAITER];

typedef struct
{
    int id_table;
} Order;
Order orders[10];
int numOrders = 0;

sem_t sem_cash_box;
pthread_mutex_t mutex_cash_box;
pthread_mutex_t mutexOrders;

// Función que representa el hilo de atención en el área de caja
void *cash_box_thread(void *arg)
{
    while (1)
    {
        sem_wait(&sem_cash_box);
        pthread_mutex_lock(&mutex_cash_box);

        int id_waiter = -1;
        for (int i = 0; i < 10; i++)
        {
            if (waiter[i].in_cashbox)
            {
                continue;
            }

            if (id_waiter == -1 || waiter[i].total_orders_claimed < waiter[id_waiter].total_orders_claimed)
            {
                id_waiter = i;
            }
        }

        waiter[id_waiter].in_cashbox = true;
        printf("Atendiendo a los clientes del mesonero %d\n", id_waiter);
        pthread_mutex_lock(&mutex_cash_box);
        for (int i = 0; i < numOrders; i++)
        {
            if (orders[i].id_table == id_waiter)
            {
                printf("Cobrando el pedido de la mesa %d\n", id_waiter);
                // Simulamos una espera aleatoria para representar el tiempo que tarda en cobrarse el pedido
                // usleep(rand() % 1000000);
                sleep(3);
            }
        }
        pthread_mutex_unlock(&mutex_cash_box);
        waiter[id_waiter].in_cashbox = false;
        pthread_mutex_unlock(&mutex_cash_box);
    }
    return NULL;
}

void *waiter_thread(void *arg)
{
    int id = *(int *)arg;
    while (1)
    {
        int table_assigned = rand() % 20 + 1;
        int id_table = table_assigned - 1;
        sem_wait(&sem_table[id_table]);

        Order order;
        order.id_table = table_assigned;
        printf("Mesonero %d tomando pedido de la mesa %d.\n", id, table_assigned);

        // Register order
        pthread_mutex_lock(&mutexOrders);
        numOrders++;
        pthread_mutex_unlock(&mutexOrders);

        printf("Mesonero %d: Llevar pedido de la mesa %d a la cocina\n", id, table_assigned);
        printf("Mesonero %d: Esperando a que el pedido esté listo en la taquilla\n", id);
        // Simulamos una espera aleatoria para representar el tiempo que tarda en prepararse el pedido
        // usleep(rand() % 1000);
        sleep(3);

        printf("Mesonero %d: Llevando pedido a la mesa %d\n", id, table_assigned);
        sem_post(&sem_table[id_table]); // Liberar la mesa para el siguiente cliente

        // Si no está en el área de caja, tomar un descanso después de cada 10 pedidos cobrados
        if (!waiter[id].in_cashbox)
        {
            waiter[id].total_orders_claimed++;
            printf("Mesonero %d: Total de pedidos reclamados: %d\n", id, waiter[id].total_orders_claimed);
            if (waiter[id].total_orders_claimed % 10 == 0)
            {
                printf("Mesonero %d: Tomando un descanso\n", id);
                // Simulamos una espera aleatoria para representar el tiempo de descanso
                // usleep(rand() % 1000);
                sleep(5);
            }
        }

        sleep(3);
    }

    return NULL;
}

#endif