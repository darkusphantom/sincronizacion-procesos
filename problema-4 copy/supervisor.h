#ifndef SUPERVISOR_JH
#define SUPERVISOR_JH

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "global.h"

void supervisor_init()
{
    for (int i = 0; i < MAX_SUPERVISOR; i++)
    {
        supervisor[i].id = i;
        supervisor[i].spreadsheet.pedidos_contabilizados = 0;
        supervisor[i].spreadsheet.mayor_numero_pedidos = 0;
        supervisor[i].spreadsheet.mesonero_del_dia = 0;
        supervisor[i].spreadsheet.total_cobros_por_mesoneros = 0;
        supervisor[i].spreadsheet.total_pedidos_por_mesoneros = 0;
    }
}

void *supervisor_func(void *arg)
{
    int supervisor_id = *(int *)arg;
    int mesonero_id = 0;
    while (1)
    {
        sem_wait(&mutex_pedidos);
        supervisor[supervisor_id].spreadsheet.pedidos_contabilizados++;
        sem_post(&mutex_pedidos);
        sem_wait(&mutex_cobros);
        supervisor[supervisor_id].spreadsheet.total_cobros_por_mesoneros++;
        sem_post(&mutex_cobros);
        printf("Supervisor %d con datos del Mesonero %d:", supervisor_id, mesonero_id);
        // printf(" - Pedidos: %d - Cobros: %d\n");
    }
    return NULL;
}

void supervisor_saved_order(int id)
{
    // sem_wait(&planilla);
    printf("Mesonero %d avisa al supervisor\n", id);
    // Avisar al supervisor
    // ...
    // sem_post(&planilla);
}

#endif // SUPERVISOR_JH