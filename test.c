#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PANTALLAS 4
#define NUM_MESAS 20
#define NUM_SUPERVISORES 2
#define NUM_MESONEROS 10 // 10 dia. 10 noche

#define MAX_PEDIDOS 100

int pedidos_realizados = 0;
int mesonero_en_caja = -1;

sem_t pantallas;
sem_t mesas;
sem_t taquilla;
sem_t caja;
sem_t supervisores;
sem_t mutex;

void *mesonero(void *arg)
{
    int id_mesonero = *(int *)arg;
    while (pedidos_realizados < MAX_PEDIDOS)
    {
        sem_wait(&mesas);
        printf("Mesonero %d tomando pedido en la mesa.\n", id_mesonero);
        sem_wait(&pantallas);
        printf("Mesonero %d registrando pedido en una pantalla de registro de pedido.\n", id_mesonero);
        pedidos_realizados++;
        sem_post(&pantallas);
        sem_post(&taquilla);
        printf("Mesonero %d esperando a que el pedido esté listo en la taquilla.\n", id_mesonero);
        sem_wait(&mesas);
        printf("Mesonero %d recogiendo el pedido en la taquilla.\n", id_mesonero);
        sem_wait(&taquilla);
        int num_pedido = pedidos_realizados;
        sem_post(&taquilla);
        sem_post(&mesas);
        printf("Mesonero %d llevando el pedido a la mesa.\n", id_mesonero);
        sleep(2);
        if (mesonero_en_caja == -1)
        {
            sem_wait(&caja);
            printf("Mesonero %d atendiendo a clientes en la caja.\n", id_mesonero);
            mesonero_en_caja = id_mesonero;
            sem_post(&caja);
        }
        else
        {
            printf("Mesonero %d esperando a que la caja esté libre.\n", id_mesonero);
            while (mesonero_en_caja != -1)
            {
                sleep(1);
            }
            sem_wait(&caja);
            printf("Mesonero %d atendiendo a clientes en la caja.\n", id_mesonero);
            mesonero_en_caja = id_mesonero;
            sem_post(&caja);
        }
        sem_wait(&mutex);
        if (pedidos_realizados % 10 == 0)
        {
            sem_wait(&supervisores);
            printf("Mesonero %d necesita tomar un descanso. Avisando a un supervisor.\n", id_mesonero);
            sem_post(&supervisores);
        }
        sem_post(&mutex);
        sem_wait(&caja);
        printf("Mesonero %d terminando de atender a clientes en la caja.\n", id_mesonero);
        mesonero_en_caja = -1;
        sem_post(&caja);
    }
    pthread_exit(NULL);
    return NULL;
}

void *supervisor(void *arg)
{
    while (1)
    {
        sem_wait(&supervisores);
        int id_mesonero = -1;
        sem_wait(&mutex);
        for (int i = 0; i < NUM_MESONEROS; i++)
        {
            if (mesonero_en_caja != i)
            {
                id_mesonero = i;
                break;
            }
        }
        sem_post(&mutex);
        if (id_mesonero != -1)
        {
            printf("Supervisor llenando planilla para el mesonero %d.\n", id_mesonero);
            sleep(5);
        }
    }
    return NULL;
}

int main()
{
    int i;
    pthread_t th_mesoneros[NUM_MESONEROS];
    pthread_t th_supervisores[NUM_SUPERVISORES];
    sem_init(&pantallas, 0, NUM_PANTALLAS);
    sem_init(&mesas, 0, NUM_MESAS);
    sem_init(&taquilla, 0, 0);
    sem_init(&caja, 0, 1);
    sem_init(&supervisores, 0, 0);
    sem_init(&mutex, 0, 1);
    int mesoneros[NUM_MESONEROS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (i = 0; i < NUM_MESONEROS; i++)
    {
        pthread_create(&th_mesoneros[i], NULL, mesonero, &mesoneros[i]);
    }
    for (i = 0; i < NUM_SUPERVISORES; i++)
    {
        pthread_create(&th_supervisores[i], NULL, supervisor, NULL);
    }
    for (i = 0; i < NUM_MESONEROS; i++)
    {
        pthread_join(th_mesoneros[i], NULL);
    }
    for (i = 0; i < NUM_SUPERVISORES; i++)
    {
        pthread_cancel(th_supervisores[i]);
    }
    sem_destroy(&pantallas);
    sem_destroy(&mesas);
    sem_destroy(&taquilla);
    sem_destroy(&caja);
    sem_destroy(&supervisores);
    sem_destroy(&mutex);
    return 0;
}