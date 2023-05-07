#ifndef ORDER_SCREEN_H
#define ORDER_SCREEN_H

#include <pthread.h>

const int MAX_SCREEN = 4;

typedef struct
{
    int num_order;
    // cola de pedidos
} order_screen_t;

void *register_order(void *arg)
{
    order_screen_t *order_screen = (order_screen_t *)arg;

    // loop para recibir pedidos de los mesoneros
    while (1)
    {
        // recibir pedido de un mesonero y agregarlo a la cola de pedidos
    }

    return NULL;
}

void init_order_screen(pthread_t screen[MAX_SCREEN])
{
    order_screen_t pantallas[MAX_SCREEN];
    for (int i = 0; i < MAX_SCREEN; i++)
    {
        pantallas[i].num_order = i;
        // inicializar cola de pedidos
        pthread_create(&screen[i], NULL, register_order, (void *)&pantallas[i]);
    }
}

#endif