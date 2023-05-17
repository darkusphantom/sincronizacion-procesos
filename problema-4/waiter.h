#ifndef WAITER_H
#define WAITER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "global.h"
// #include "caja.h"

void waiter_init(int i)
{

    waiters[i].id = i;
    waiters[i].en_descanso = 0;
    waiters[i].pedidos_atendidos = 0;
    waiters[i].cobros_en_caja = 0;
    waiters[i].total_pedidos_atendidos = 0;
    waiters[i].total_descansos = 0;
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
 * @brief Funcion para el descanso del mesonero
 */
void descansar_mesonero(int id)
{
    // Revisar si el mesonero ha atendido a 10 clientes
    if (waiters[id].pedidos_atendidos % 10 == 0)
    {
        // Solicitar un descanso
        sem_post(&descanso_mesonero);
        sem_wait(&mesonero_en_descanso);
    }
}

// Registra el pedido en pantalla
void register_order(int id)
{
    sem_wait(&pantalla);
    printf("El mesonero %d esta anotando el pedido en pantalla.\n", id);
    waiters[id].pedidos_atendidos++;
    sem_post(&pantalla);
}

void *mesonero_func(void *arg)
{
    // id del mesero
    int id = *(int *)arg;
    int id_mesa;

    sem_getvalue(&cliente, &id_mesa);

    // Indica si la caja esta disponible u ocupada (Disponible = 1, Ocupada = 0)
    int available_cash_box = 1;

    descansar_mesonero(id);

    while (available_cash_box)
    {
        // Esperar a que llegue un cliente
        sem_wait(&cliente);
        // Despertar al cliente
        sem_post(&mesonero);
        printf("El mesonero %d está atendiendo al cliente en la mesa %d\n", id, id_mesa);
        printf("El mesonero %d terminó de atender al cliente en la mesa\n", id);

        register_order(id);

        // Esperar a que el pedido esté listo en la taquilla
        printf("Esperando a que el pedido esté listo en la taquilla\n");
        // Espera a la caja de cobro
        sem_post(&caja);
        // Llama al pedido
        sem_wait(&pedido);
        printf("Mesero %d esta llevando el pedido al cliente\n", id);
        // Llevar el pedido a la mesa cuando esté listo en la taquilla
        liberar_mesa();
    }

    return NULL;
}

#endif // WAITER_H