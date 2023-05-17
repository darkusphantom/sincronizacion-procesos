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
#include "caja.h"

int main()
{
     pthread_t mesonero_thread[MAX_WAITER];
     pthread_t cliente_thread[MAX_CLIENT];
     pthread_t caja_thread[MAX_CAJA];
     pthread_t taquilla_thread;

     // queue_init(&order);

     // Contiene el hilo para manejar la caja, la taquilla y los cobros
     semaphore_init();

     // supervisor_init();

     // Crear los hilos para clientes
     for (int i; i < MAX_TABLE; i++)
     {
          clients[i] = i;
          pthread_create(&cliente_thread[i], NULL, cliente_func, &clients[i]);
     }

     // Crea hilos para mesoneros
     for (int i = 0; i < MAX_WAITER; i++)
     {
          waiter_init(i);
          pthread_create(&mesonero_thread[i], NULL, mesonero_func, &waiters[i].id);
     }

     // Crea hilos para la caja
     for (int i = 0; i < MAX_CAJA; i++)
     {
          pthread_create(&caja_thread[i], NULL, caja_func, NULL);
     }

     pthread_create(&taquilla_thread, NULL, taquilla_func, NULL);

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

     for (int i = 0; i < MAX_PANTALLA; i++)
     {
          pthread_join(caja_thread[i], NULL);
     }

     // for (int i = 0; i < MAX_SUPERVISOR; i++)
     // {
     //   pthread_join(supervisor_thread[i], NULL);
     // }

     semaphore_destroy();

     return 0;
}