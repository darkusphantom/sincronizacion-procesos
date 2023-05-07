#ifndef SUPERVISOR_H
#define SUPERVISOR_H

#include <pthread.h>

const int MAX_SUPERVISORS = 10;

/* Structure containing the data for the thread of an waiter */
typedef struct SupervisorData
{
    int id;
    // Otros datos necesarios para el hilo de un mesonero
} supervisor_data_t;

/* Function representing the thread body of an innkeeper's thread */
void *supervisor_thread(void *arg)
{
    struct SupervisorData *data = (struct SupervisorData *)arg;
    // Cuerpo del hilo de un mesonero
    return NULL;
}

void create_waiter(pthread_t supervisors[MAX_SUPERVISORS])
{
    for (int i = 0; i < 10; i++)
    {
        // struct WaiterData *data = malloc(sizeof(struct WaiterData));
        supervisor_data_t data[MAX_SUPERVISORS];
        data->id = i;
        // Inicializar otros datos necesarios para el hilo de un mesonero
        pthread_create(&supervisors[i], NULL, supervisor_thread, (void *)data);
    }
}
#endif