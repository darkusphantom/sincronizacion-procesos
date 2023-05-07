#ifndef WAITER_H
#define WAITER_H

#include <pthread.h>

const int MAX_WAITER = 10;

/* Structure containing the data for the thread of an waiter */
typedef struct WaiterData
{
    int id;
    // Otros datos necesarios para el hilo de un mesonero
} waiter_data_t;

/* Function representing the thread body of an innkeeper's thread */
void *waiter_thread(void *arg)
{
    struct WaiterData *data = (struct WaiterData *)arg;
    // Cuerpo del hilo de un mesonero
    return NULL;
}

void create_waiter(pthread_t waiters[MAX_WAITER])
{
    for (int i = 0; i < 10; i++)
    {
        // struct WaiterData *data = malloc(sizeof(struct WaiterData));
        waiter_data_t data[MAX_WAITER];
        data->id = i;
        // Inicializar otros datos necesarios para el hilo de un mesonero
        pthread_create(&waiters[i], NULL, waiter_thread, (void *)data);
    }
}

#endif