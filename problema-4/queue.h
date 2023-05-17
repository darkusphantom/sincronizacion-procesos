#ifndef QUEUE_H
#define QUEUE_H

#define MAX_QUEUE_SIZE 100

#include <stdio.h>

typedef struct
{
    int items[MAX_QUEUE_SIZE];
    int front;
    int rear;
} Queue;

/**
 * @brief Inicializa la cola
 *
 * @param queue La cola a inicializar
 */
void queue_init(Queue *q)
{
    q->front = -1;
    q->rear = -1;
}

/**
 * @brief Encola un elemento a la cola
 *
 * @param queue La cola donde vas a encolar el elemento
 * @param item elemento que vas a colocar en la cola
 */
void enqueue(Queue *q, int item)
{
    if (q->rear == MAX_QUEUE_SIZE - 1)
    {
        printf("Error: la cola está llena\n");
        return;
    }
    else
    {
        if (q->front == -1)
        {
            q->front = 0;
        }
        q->rear++;
        q->items[q->rear] = item;
    }
}

/**
 * @brief Desencola un elemento a la cola
 *
 * @param queue La cola donde vas a encolar el elemento
 */
int dequeue(Queue *q)
{
    if (q->front == -1 || q->front > q->rear)
    {
        printf("Error: la cola está vacía\n");
        return -1;
    }
    else
    {
        int item = q->items[q->front];
        q->front++;
        return item;
    }
}

#endif // !QUEUE_H