#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "global.h"

void *cliente_func(void *arg)
{
    while (1)
    {
        // Tomar el mutex para acceder a las variables compartidas
        sem_wait(&mutex);
        if (mesas_disponibles > 0)
        {
            // Hay una mesa disponible
            mesas_disponibles--;
            // Despertar al mesonero
            sem_post(&cliente);
            // Esperar a que el mesonero lo atienda
            sem_wait(&mesonero);
            // Sentarse en la mesa
            printf("El cliente se está sentando en la mesa\n");
            printf("El cliente se sentó en la mesa\n");
        }
        else
        {
            // No hay mesas disponibles
            printf("El cliente se fue porque no había mesas disponibles\n");
        }
        // Liberar el mutex
        sem_post(&mutex);
        // Esperar un tiempo aleatorio antes de llegar al siguiente cliente
        sleep(rand() % 5 + 1);
    }
    return NULL;
}

void liberar_mesa()
{
    // Tomar el mutex para acceder a las variables compartidas
    sem_wait(&mutex);
    // Incrementar el número de mesas disponibles
    mesas_disponibles++;
    // Liberar el mutex
    sem_post(&mutex);
}

#endif // CLIENT_H