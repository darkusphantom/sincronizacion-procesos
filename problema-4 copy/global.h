#ifndef GLOBAL_H
#define GLOBAL_H

#include <pthread.h>
#include <semaphore.h>
#include "struct.h"
#include "const.h"

// Definición de variables globales
// Order orders[100];                     // Array con los pedidos del restaurante
// Waiter waiters[MAX_WAITER];             // Array con los mesoneros del turno
// Supervisor supervisor[MAX_SUPERVISOR]; // Array con los supervisores

// sem_t sem_table[MAX_TABLE]; // Array de semáforos correspondientes a las mesas
// sem_t sem_cashier;          // Semáforo correspondiente al cajero
// sem_t sem_rest;             // Semáforo correspondiente a los descansos

// pthread_mutex_t mutex_orders = PTHREAD_MUTEX_INITIALIZER;  // Mutex para proteger la variable numOrders
// pthread_mutex_t mutex_cashbox = PTHREAD_MUTEX_INITIALIZER; // Mutex para proteger el área de caja

// pthread_t tid[MAX_WAITER];         // Array con los hilos correspondientes a los mesoneros
// pthread_t sup_tid[MAX_SUPERVISOR]; // Array con los hilos correspondientes a los supervisores
// pthread_t cashier_tid;             // Hilo correspondiente al cajero

// int numOrders = 0; // Número de pedidos registrados

sem_t mutex;         // Semáforo binario para controlar el acceso a las variables compartidas
sem_t mutex_pedidos; //
sem_t mesonero;      // Semáforo binario para que el mesonero espere a que llegue un cliente
sem_t cliente;       // Semáforo binario para que el cliente espere a que el mesonero lo atienda
sem_t taquilla;      // Semáforo binario para la taquilla
sem_t planilla;      // Semaforo para Planilla del supervisor para registrar cobros
sem_t caja;          // Semaforo para Caja de cobro
sem_t waiter_rest;   // Semaforo para el descanso del mesonero
sem_t mutex_cobros;

int mesas_disponibles = MAX_TABLE; // Número de mesas disponibles
int caja_disponible = 0;           // Bandera que indica si la caja está disponible

Waiter waiters[MAX_WAITER];
Supervisor supervisor[MAX_SUPERVISOR];

// Inicializar los semáforos
void semaphore_init()
{
    sem_init(&mutex, 0, 1);
    sem_init(&mesonero, 0, 0);
    sem_init(&cliente, 0, 0);
    // sem_init(&taquilla, 0, 0);
    // sem_init(&planilla, 0, 0);
    sem_init(&caja, 0, 1);
    sem_init(&mutex_cobros, 0, 0);
}

// Destruir los semáforos
void semaphore_destroy()
{
    sem_destroy(&mutex);
    sem_destroy(&mesonero);
    sem_destroy(&cliente);
    // sem_destroy(&taquilla);
    sem_destroy(&planilla);
    sem_destroy(&caja);
    // sem_destroy(&waiter_rest);
    sem_destroy(&mutex_cobros);
}
#endif // !GLOBAL_H