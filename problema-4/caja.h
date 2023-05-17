#ifndef CAJA_H
#define CAJA_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "global.h"
#include "const.h"
#include "supervisor.h"

// Cobrar en la caja
void cash_in()
{
    while (caja_disponible)
    {
        if (mesonero_en_caja > 2)
        {
            printf("La caja esta ocupada.\n");
        }
        else
        {
            printf("Mesonero cobrando en caja\n");
        }

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
        //   waiter_go_rest(id);
        //   }
        // Liberar la caja
        caja_disponible = 0;
        sem_post(&caja);
    }
}

// Llevar el pedido a la mesa
void send_order_to_client(int id_waiter)
{
    // Esperar a que el pedido esté listo en la taquilla
    // sem_wait(&taquilla);
    printf("Mesonero %d llevando pedido a la mesa\n", id_waiter);
    //...
    // sem_post(&taquilla);
}

#endif // !CAJA_H