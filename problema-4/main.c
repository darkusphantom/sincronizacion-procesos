#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "global.h"
#include "const.h"
#include "struct.h"
#include "client.h"
#include "supervisor.h"
#include "waiter.h"
#include "queue.h"

int main()
{
     pthread_t mesonero_thread[MAX_WAITER];
     pthread_t cliente_thread[MAX_CLIENT];
     // pthread_t monitor_thread;

     // queue_init(&order);

     // Contiene el hilo para manejar la caja, la taquilla y los cobros
     semaphore_init();

     // supervisor_init();

     // Crear los hilos del mesonero y los clientes
     for (int i; i < MAX_CLIENT; i++)
     {
          pthread_create(&cliente_thread[i], NULL, cliente_func, NULL);
     }

     for (int i = 0; i < MAX_WAITER; i++)
     {
          waiter_init(i);
          pthread_create(&mesonero_thread[i], NULL, mesonero_func, &waiters[i].id);
     }

     // pthread_create(&monitor_thread, NULL, monitor_func, NULL);

     // for (int i = 0; i < MAX_SUPERVISOR; i++)
     // {
     //      pthread_create(&supervisor_thread, NULL, supervisor_func, &supervisor[i]);
     // }

     // Esperar a que los hilos terminen
     for (int i = 0; i < MAX_WAITER; i++)
     {
          pthread_join(mesonero_thread[i], NULL);
     }

     for (int i = 0; i < MAX_CLIENT; i++)
     {
          pthread_join(cliente_thread[i], NULL);
     }

     // for (int i = 0; i < MAX_SUPERVISOR; i++)
     // {
     //   pthread_join(supervisor_thread[i], NULL);
     // }

     semaphore_destroy();

     return 0;
}