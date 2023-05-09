#ifndef WAITER_H
#define WAITER_H

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "table.h"

#define N_WAITER 10

typedef struct
{
    int id;
    int in_cashbox;
    int total_orders_claimed;
} Waiter;

Waiter waiter[N_WAITER];

pthread_mutex_t mutexOrders;

int numOrders = 0;

void *waiter_thread(void *arg)
{
    int id = *(int *)arg;
    while (1)
    {
        sem_wait(&sem_table[id]); // Espera por mesa

        printf("Mesonero tomando pedido de la mesa %d.\n", id);

        // Register order
        pthread_mutex_lock(&mutexOrders);
        numOrders++;
        pthread_mutex_unlock(&mutexOrders);

        // Llevar el pedido a la cocina
        printf("Mesonero %d: Llevar pedido de la mesa %d a la cocina\n", id, id);

        // Esperar a que el pedido esté listo en la taquilla
        printf("Mesonero %d: Esperando a que el pedido esté listo en la taquilla\n", id);
        // Simulamos una espera aleatoria para representar el tiempo que tarda en prepararse el pedido
        // usleep(rand() % 1000000);
        sleep(3);

        // Llevar el pedido a la mesa correspondiente
        printf("Mesonero %d: Llevando pedido a la mesa %d\n", id, id);
        sem_post(&sem_table[id]); // Liberar la mesa para el siguiente cliente

        // Si no está en el área de caja, tomar un descanso después de cada 10 pedidos cobrados
        if (!waiter[id].in_cashbox)
        {
            waiter[id].total_orders_claimed++;
            if (waiter[id].total_orders_claimed % 10 == 0)
            {
                printf("Mesonero %d: Tomando un descanso\n", id);
                // Simulamos una espera aleatoria para representar el tiempo de descanso
                // usleep(rand() % 1000000);
                sleep(5);
            }
        }

        sleep(3);
    }

    return NULL;
}

#endif