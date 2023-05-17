#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "global.h"

void *cliente_func(void *arg)
{
    int id = *((int *)arg);
    while (1)
    {
        // Tomar el mutex para acceder a las variables compartidas
        sem_wait(&mesa);
        if (mesas_disponibles > 0)
        {
            // Hay una mesa disponible
            mesas_disponibles--;
            // Despertar al mesonero
            sem_post(&cliente);
            // Esperar a que el mesonero lo atienda
            sem_wait(&mesonero);
            // Sentarse en la mesa
            printf("El cliente %d acaba de llegar y se está sentando en la mesa\n", id);
        }
        else
        {
            // No hay mesas disponibles
            printf("El cliente %d se fue porque no había mesas disponibles\n", id);
        }
        // Liberar el mesa
        sem_post(&mesa);
        // Esperar un tiempo aleatorio antes de llegar al siguiente cliente
        sleep(rand() % 5 + 1);
    }
    return NULL;
}

void liberar_mesa()
{
    // Tomar el mutex para acceder a las variables compartidas
    sem_wait(&mesa);
    // Incrementar el número de mesas disponibles
    mesas_disponibles++;
    // Liberar el mesa
    sem_post(&mesa);
}

#endif // CLIENT_H