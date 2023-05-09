#ifndef CASH_REGISTER_H
#define CASH_REGISTER_H

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N_TABLE 20
#define N_WAITER 10

/// Definición de la estructura Mesonero
typedef struct
{
    int id;
} Waiter;

sem_t sem_table[N_TABLE];

int waiter_in_cashbox = 0;

/// Función que representa la atención al cliente en una mesa
void attend_table(int id_table)
{
    printf("Atendiendo al cliente en la mesa %d\n", id_table);
}

void *waiter_thread(void *arg)
{
    int id = *(int *)arg;
    while (1)
    {
        sem_wait(&sem_table[id]); // Esperar hasta que haya una mesa disponible
        
        if(waiter_in_cashbox < 2) {
            
        }

        attend_table(id);
        sem_post(&sem_table[id]); // Liberar la mesa para el siguiente cliente
        sleep(3);
    }

    return NULL;
}

/// Inicializar los mesoneros y los semáforos correspondientes a cada mesa
void init_waiter(pthread_t t_waiter[], Waiter waiter[])
{
    for (int i = 0; i < N_WAITER; i++)
    {
        waiter[i].id = i;
        sem_init(&sem_table[i], 0, 1); // Inicializar cada semáforo a 1 (mesa disponible)
    }

    // Crear los hilos correspondientes a cada mesonero
    for (int i = 0; i < N_WAITER; i++)
    {
        pthread_create(&t_waiter[i], NULL, waiter_thread, &waiter[i].id);
        sleep(1);
    }
}

#endif