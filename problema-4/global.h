#ifndef GLOBAL_H
#define GLOBAL_H

#include <pthread.h>
#include <semaphore.h>
#include "struct.h"
#include "const.h"
#include "queue.h"

sem_t mutex_pedidos; //
sem_t mutex_cobros;  //
sem_t mesonero;      // Semáforo binario para que el mesonero espere a que llegue un cliente
sem_t mesa;          // Semáforo binario para controlar las mesas disponibles
sem_t cliente;       // Semáforo binario para que el cliente espere a que el mesonero lo atienda
sem_t taquilla;      // Semáforo binario para la taquilla
sem_t planilla;      // Semaforo para Planilla del supervisor para registrar cobros
sem_t caja;          // Semaforo para Caja de cobro
sem_t pantalla;
// sem_t sem_cola_pedidos; // Semaforo para la lista de pedidos

int mesas_disponibles = MAX_TABLE; // Número de mesas disponibles
int caja_disponible = 1;           // Bandera que indica si la caja está disponible
int mesoneros_en_caja = 0;         // Indica el numero de mesoneros en caja

Waiter waiters[MAX_WAITER];            // Lista de meseros
Supervisor supervisor[MAX_SUPERVISOR]; // Lista de supervisores
Queue order;                           // Cola de pedidos
int clients[MAX_CLIENT];               // Lista de clientes

// Inicializar los semáforos
void semaphore_init()
{
    sem_init(&mesa, 0, MAX_TABLE);
    sem_init(&mesonero, 0, MAX_WAITER);
    sem_init(&cliente, 0, 0);
    // sem_init(&supervisor, 0 , MAX_SUPERVISOR);
    // sem_init(&taquilla, 0, 0);
    // sem_init(&planilla, 0, 0);
    sem_init(&caja, 0, 2);
    // sem_init(&mutex_pedidos, 0, 1);
    // sem_init(&mutex_cobros, 0, 0);
    // sem_init(&sem_cola_pedidos, 0, 0);
}

// Destruir los semáforos
void semaphore_destroy()
{
    sem_destroy(&mesa);
    sem_destroy(&mesonero);
    sem_destroy(&cliente);
    // sem_destroy(&taquilla);
    sem_destroy(&planilla);
    sem_destroy(&caja);
    // sem_destroy(&mutex_pedidos);
    // sem_destroy(&waiter_rest);
    sem_destroy(&mutex_cobros);
    // sem_destroy(&sem_cola_pedidos);
}

#endif // !GLOBAL_H