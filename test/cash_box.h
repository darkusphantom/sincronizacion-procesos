#ifndef CASH_BOX
#define CASH_BOX

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "waiter.h"

sem_t sem_cash_box;
pthread_mutex_t mutex_cash_box;

// Función que representa el hilo de atención en el área de caja
void *cash_box_thread(void *arg)
{
    while (1)
    {
        // Esperar a que haya al menos dos mesoneros disponibles para atender en el área de caja
        sem_wait(&sem_cash_box);

        // Bloquear el mutex correspondiente a la caja para evitar que dos mesoneros intenten cobrar al mismo tiempo
        pthread_mutex_lock(&mutex_cash_box);

        // Elegir al mesonero que va a atender en la caja
        int id_waiter = -1;
        for (int i = 0; i < 10; i++)
        {
            if (waiter[i].in_cashbox)
            {
                continue; // Saltar si el mesonero ya está en el área de caja
            }

            if (id_waiter == -1 || waiter[i].numPedidosCobrados < waiter[id_waiter].numPedidosCobrados)
            {
                id_waiter = i;
            }
        }

        // Indicar que el mesonero elegido está en el área de caja
        waiter[id_waiter].in_cashbox = true;

        // Cobrar a los clientes correspondientes al mesonero elegido
        printf("Atendiendo a los clientes del mesonero %d\n", id_waiter);
        pthread_mutex_lock(&mutexOrders);
        for (int i = 0; i < numPedidos; i++)
        {
            if (pedidos[i].numMesa == idMesonero)
            {
                printf("Cobrando el pedido de la mesa %d\n", idMesonero);
                // Simulamos una espera aleatoria para representar el tiempo que tarda en cobrarse el pedido
                usleep(rand() % 1000000);
            }
        }
        pthread_mutex_unlock(&mutexPedidos);

        // Indicar que el mesonero elegido ya no está en el área de caja
        mesoneros[idMesonero].in_cashbox = false;

        // Desbloquear el mutex correspondiente a la caja
        pthread_mutex_unlock(&mutexCaja);
    }
    return NULL;
}

#endif