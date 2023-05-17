#ifndef WAITER_H
#define WAITER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "global.h"
// #include "caja.h"

void waiter_init()
{
    for (int i = 0; i < MAX_WAITER; i++)
    {
        waiters[i].id = i;
        waiters[i].en_descanso = 0;
        waiters[i].pedidos_atendidos = 0;
        waiters[i].cobros_en_caja = 0;
        waiters[i].total_pedidos_atendidos = 0;
        waiters[i].total_descansos = 0;
    }
}

// Muestra el resultado del turno actual
void waiter_show_results(int id)
{
    int total_pedidos_mesonero = waiters[id].pedidos_atendidos++;
    waiters[id].pedidos_atendidos = 0;

    int cobros_caja = waiters[id].cobros_en_caja++;
    waiters[id].cobros_en_caja = 0;

    waiters[id].total_pedidos_atendidos += total_pedidos_mesonero;
    int total_pedidos_turno = waiters[id].total_pedidos_atendidos;

    // Muestra el total de pedidos y cobros en la ronda antes del descanso
    printf("Pedidos atendidos del mesonero %d: %d\n", id, total_pedidos_mesonero);
    printf("Cobros en caja del mesonero %d: %d\n", id, cobros_caja);
    printf("Total de pedidos atendidos en este turno: %d\n", total_pedidos_turno);
}

/**
 * @brief Funcion para devolver al mesonero al trabajo
 *
 * @param id Identificador del mesonero.
 */
void waiter_go_back_work(int id)
{
    waiters[id].en_descanso = 0;
    printf("El mesero %d volvió del descanso\n", id);
}

/**
 * @brief Funcion para enviar a descansar a un mesonero
 *
 * @param id Identificador del mesonero.
 */
void waiter_go_rest(int id)
{
    printf("Mesonero %d se va a tomar un descanso\n", id);
    waiters[id].en_descanso = 1;
    waiters[id].total_descansos++;
    sleep(3);
}

void *mesonero_func(void *arg)
{
    // waiter actual es id del mesonero
    int waiter_actual = *(int *)arg;
    // Indica si la caja esta disponible u ocupada (Disponible = 1, Ocupada = 0)
    int available_cash_box = 1;
    // Descanso actual del mesero
    int waiter_in_rest = waiters[waiter_actual].en_descanso;

    // if (waiter_in_rest)
    // {
    //     waiter_go_back_work(waiters[waiter_actual].id);
    // }
    while (available_cash_box)
    {
        // Esperar a que llegue un cliente
        sem_wait(&cliente);
        // Despertar al cliente
        sem_post(&mesonero);
        printf("El mesonero está atendiendo al cliente en la mesa\n");
        printf("El mesonero terminó de atender al cliente en la mesa\n");
        sleep(1);

        sem_wait(&caja);
        caja_disponible = 1;
        mesoneros_en_caja++;
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
            //   waiter_go_rest(id);
            //   }
            // Liberar la caja
            caja_disponible = 0;
            mesoneros_en_caja--;
            sem_post(&caja);
        }

        // send_order_to_client(waiter_actual);
        liberar_mesa();
    }

    return NULL;
}

#endif // WAITER_H