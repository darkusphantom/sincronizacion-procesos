#ifndef CASH_REGISTER_H
#define CASH_REGISTER_H

#include <pthread.h>
#include "sem.h"

// Función que representa el cuerpo del hilo de la caja
void *cash_register_thread(void *arg)
{
    // Cuerpo del hilo de la caja
    return NULL;
}

void create_cash_register(pthread_t cash_register)
{
    // Crear e implementar hilo para la caja
    pthread_create(&cash_register, NULL, cash_register_thread, NULL);
}
#endif