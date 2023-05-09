#ifndef WAITER_H
#define WAITER_H

#include <pthread.h>
#include "sem.h"

const int MAX_WAITER = 10;

/// @brief Structure containing the data for the thread of an waiter
typedef struct WaiterData
{
    int id;
    int order_by_turn;
    int rest_counter;
    // Otros datos necesarios para el hilo de un mesonero
} waiter_data_t;

/**
 * @brief Function representing the thread body of an waiter's thread
 *
 * @param arg
 * @return void*
 */
void *waiter_thread(void *arg)
{
    struct WaiterData *data = (struct WaiterData *)arg;


    return NULL;
}

/**
 * @brief Function to create a waiter's thread
 *
 * @param waiter
 */
void create_waiter(pthread_t waiter[MAX_WAITER])
{
    for (int i = 0; i < 10; i++)
    {
        waiter_data_t data[MAX_WAITER];
        data->id = i;
        // Inicializar otros datos necesarios para el hilo de un mesonero
        pthread_create(&waiter[i], NULL, waiter_thread, (void *)data);
    }
}

#endif