#ifndef SCREEN_H
#define SCREEN_H

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>
#include "table.h"

#define MAX_SCREEN 3


typedef struct
{
    int id_table;
} Order;

Order order[10];
pthread_t t_screen[MAX_SCREEN];
sem_t sem_screen[MAX_SCREEN];

void *screen_thread(void *arg)
{
    int id = *(int *)arg;
    while (1)
    {
        sem_wait(&sem_screen[id]);
        printf("Pantalla %d: Pedido de mesa %d\n", id, order[id].id_table);
        sem_post(&sem_screen[id]);
    }
    return NULL;
}

#endif