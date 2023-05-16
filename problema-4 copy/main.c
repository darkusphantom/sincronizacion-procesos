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

void *mesonero_func(void *arg)
{
     // waiter actual es id del mesonero
     int waiter_actual = *(int *)arg;
     // Indica si la caja esta disponible u ocupada (Disponible = 1, Ocupada = 0)
     int available_cash_box = 1;
     // Descanso actual del mesero
     int waiter_in_rest = waiters[waiter_actual].en_descanso;

     if (waiter_in_rest)
     {
          waiter_go_back_work(waiters[waiter_actual].id);
     }
     while (available_cash_box && !waiter_in_rest)
     {
          attend_client();
          // send_order_to_client(waiter_actual);
          // cash_in(waiter_actual);
          liberar_mesa();
     }

     return NULL;
}

void *monitor_func(void *arg)
{
     // int id = 0;
     // int cobros_caja = waiters[id].cobros_en_caja;
     while (caja_disponible)
     {
          
          printf("Mesonero cobrando en caja\n");

          // if (cobros_caja < MAX_ORDERS)
          // {
          //      printf("Mesonero %d cobrando en caja\n", id);
          //      caja_disponible--;
          // Cobrar en la caja

          // // Incrementar el contador de pedidos atendidos y cobros en caja
          // waiters[id].pedidos_atendidos++;
          // waiters[id].cobros_en_caja++;
          // // Avisar a un supervisor cada 10 pedidos
          // if (cobros_caja % MAX_ORDERS == 0)
          // {
          //     supervisor_saved_order(id);
          //     waiters[id].total_descansos++;
          // }
          //      sleep(2);
          // }
          // else
          // {
          // waiter_show_results(id);

          // Si ya se han cobrado 10 pedidos, el mesonero debe tomar un descanso
          // waiter_go_rest(id);
          // }
          // Liberar la caja
          caja_disponible = 0;
          sem_post(&caja);
     }

     return NULL;
}
int main()
{
     queue_init(&order);
     // F
     pthread_t mesonero_thread, cliente_thread, supervisor_thread;

     // Contiene el hilo para manejar la caja, la taquilla y los cobros
     pthread_t monitor_thread;
     semaphore_init();

     waiter_init();
     // supervisor_init();

     // Crear los hilos del mesonero y los clientes
     pthread_create(&mesonero_thread, NULL, mesonero_func, &waiters[0].id);
     pthread_create(&cliente_thread, NULL, cliente_func, NULL);
     pthread_create(&monitor_thread, NULL, monitor_func, NULL);
     // pthread_create(&supervisor_thread, NULL, supervisor_func, &supervisor[0].id);

     // Esperar a que los hilos terminen
     pthread_join(mesonero_thread, NULL);
     pthread_join(cliente_thread, NULL);
     // pthread_join(supervisor_thread, NULL);

     semaphore_destroy();

     return 0;
}