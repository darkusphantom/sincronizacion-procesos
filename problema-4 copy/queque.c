#include <stdio.h>
#include "queue.h"

// #define MAX_QUEUE_SIZE 100

// typedef struct
// {
//     int queue[MAX_QUEUE_SIZE];
//     int front;
//     int rear;
// } Queue;

// void enqueue(Queue *q, int value)
// {
//     if (q->rear == MAX_QUEUE_SIZE - 1)
//     {
//         printf("Error: La cola está llena.\n");
//     }
//     else
//     {
//         if (q->front == -1)
//         {
//             q->front = 0;
//         }
//         q->rear++;
//         q->queue[q->rear] = value;
//     }
// }

// int dequeue(Queue *q)
// {
//     int value;
//     if (q->front == -1 || q->front > q->rear)
//     {
//         printf("Error: La cola está vacía.\n");
//         return -1;
//     }
//     else
//     {
//         value = q->queue[q->front];
//         q->front++;
//         return value;
//     }
// }

int main()
{
    Queue q;
    queue_init(&q);
    enqueue(&q, 10);
    enqueue(&q, 20);
    enqueue(&q, 30);
    printf("%d\n", dequeue(&q));
    printf("%d\n", dequeue(&q));
    printf("%d\n", dequeue(&q));
    printf("%d\n", dequeue(&q));
    return 0;
}