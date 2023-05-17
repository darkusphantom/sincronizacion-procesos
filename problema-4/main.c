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
     queue_init(&order);
     pthread_t mesonero_thread, cliente_thread; // supervisor_thread;

     // Contiene el hilo para manejar la caja, la taquilla y los cobros
     // pthread_t monitor_thread;
     semaphore_init();

     waiter_init();
     // supervisor_init();

     // Crear los hilos del mesonero y los clientes
     pthread_create(&cliente_thread, NULL, cliente_func, NULL);

     for (int i = 0; i < MAX_WAITER; i++)
     {
          pthread_create(&mesonero_thread, NULL, mesonero_func, &i);
     }
     
     // pthread_create(&monitor_thread, NULL, monitor_func, NULL);

     // for (int i = 0; i < MAX_SUPERVISOR; i++)
     // {
     //      pthread_create(&supervisor_thread, NULL, supervisor_func, &supervisor[i]);
     // }

     // Esperar a que los hilos terminen
     pthread_join(mesonero_thread, NULL);
     pthread_join(cliente_thread, NULL);
     // pthread_join(supervisor_thread, NULL);

     semaphore_destroy();

     return 0;
}