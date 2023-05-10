#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_SCREEN 3

typedef struct
{
    int id_table;
} Order;

Order order[10];
pthread_t screen_t[MAX_SCREEN];
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

int main()
{
    // Inicializar los semáforos correspondientes a cada pantalla
    for (int i = 0; i < MAX_SCREEN; i++)
    {
        sem_init(&sem_screen[i], 0, 0);
    }

    // Crear los hilos correspondientes a cada pantalla
    for (int i = 0; i < MAX_SCREEN; i++)
    {
        pthread_create(&screen_t[i], NULL, screen_thread, &i);
    }

    // Esperar a que los hilos terminen (esto no debería ocurrir nunca en este ejemplo)
    for (int i = 0; i < MAX_SCREEN; i++)
    {
        pthread_join(screen_t[i], NULL);
    }

    return 0;
}