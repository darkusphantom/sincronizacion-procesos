#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>
#include "mesonero.h"

// Array con los mesoneros disponibles
pthread_t waiter_t[N_WAITER]; // Array con los hilos correspondientes a cada mesonero

int main()
{
    // Inicializar los mesoneros y los semáforos a cada mesa
    for (int i = 0; i < N_WAITER; i++)
    {
        waiter[i].id = i;
        waiter[i].in_cashbox = false;
        waiter[i].total_orders_claimed = 0;
        sem_init(&sem_table[i], 0, 1); // Inicializar cada semáforo a 1 (mesa disponible)
    }

    sem_init(&sem_cash_box, 0, 0);
    pthread_mutex_init(&mutex_cash_box, NULL);

    // Crear los hilos a cada mesonero
    for (int i = 0; i < N_WAITER; i++)
    {
        pthread_create(&waiter_t[i], NULL, waiter_thread, &waiter[i].id);
        sleep(1);
    }

    pthread_t thread_cashbox;
    pthread_create(&thread_cashbox, NULL, cash_box_thread, NULL);

    // Esperar a que los hilos terminen (esto no debería ocurrir nunca en este ejemplo)
    for (int i = 0; i < N_WAITER; i++)
    {
        pthread_join(waiter_t[i], NULL);
    }

    pthread_join(thread_cashbox, NULL);

    return 0;
}