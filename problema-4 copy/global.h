#ifndef GLOBAL_H
#define GLOBAL_H

#include <pthread.h>
#include <semaphore.h>
#include "struct.h"
#include "const.h"

// Definición de variables globales
Order orders[100];                     // Array con los pedidos del restaurante
Waiter waiter[MAX_WAITER];             // Array con los mesoneros del turno
Supervisor supervisor[MAX_SUPERVISOR]; // Array con los supervisores

sem_t sem_table[MAX_TABLE]; // Array de semáforos correspondientes a las mesas
sem_t sem_cashier;          // Semáforo correspondiente al cajero
sem_t sem_rest;             // Semáforo correspondiente a los descansos

pthread_mutex_t mutex_orders = PTHREAD_MUTEX_INITIALIZER;  // Mutex para proteger la variable numOrders
pthread_mutex_t mutex_cashbox = PTHREAD_MUTEX_INITIALIZER; // Mutex para proteger el área de caja

pthread_t tid[MAX_WAITER];         // Array con los hilos correspondientes a los mesoneros
pthread_t sup_tid[MAX_SUPERVISOR]; // Array con los hilos correspondientes a los supervisores
pthread_t cashier_tid;             // Hilo correspondiente al cajero

int numOrders = 0; // Número de pedidos registrados

#endif // !GLOBAL_H