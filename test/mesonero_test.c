#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "mesonero.h"

#define N_MESONEROS 10
#define N_MESAS 20

// Definición de la estructura Mesonero
typedef struct
{
    int id;
} Mesonero;

Mesonero waiter[N_MESONEROS];       // Array con los mesoneros disponibles
pthread_t waiter_t[N_MESONEROS]; // Array con los hilos correspondientes a cada mesonero
// sem_t semaforosMesas[N_MESAS];

// Función principal que inicializa los mesoneros y los hilos correspondientes
int main()
{
    // Inicializar los mesoneros y los semáforos correspondientes a cada mesa
    for (int i = 0; i < N_WAITER; i++)
    {
        waiter[i].id = i;
        sem_init(&sem_table[i], 0, 1); // Inicializar cada semáforo a 1 (mesa disponible)
    }

    // Crear los hilos correspondientes a cada mesonero
    for (int i = 0; i < N_WAITER; i++)
    {
        pthread_create(&waiter_t[i], NULL, waiter_thread, &waiter[i].id);
        sleep(1);
    }

    // Esperar a que los hilos terminen (esto no debería ocurrir nunca en este ejemplo)
    for (int i = 0; i < N_MESONEROS; i++)
    {
        pthread_join(waiter_t[i], NULL);
    }

    return 0;
}