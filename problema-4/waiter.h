#ifndef WAITER_H
#define WAITER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "global.h"

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

// Atender al cliente en la mesa
void attend_client()
{
    // Esperar a que llegue un cliente
    sem_wait(&cliente);
    // Despertar al cliente
    sem_post(&mesonero);
    printf("El mesonero está atendiendo al cliente en la mesa\n");
    printf("El mesonero terminó de atender al cliente en la mesa\n");
    printf("El mesonero va a registrar el pedido en pantalla");
    sem_wait(&caja);
    sleep(1);
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

#endif // WAITER_H