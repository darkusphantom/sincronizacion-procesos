#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_MESONEROS 10
#define MAX_CAJA 2
#define MAX_PLANILLA 10

pthread_t mesoneros[MAX_MESONEROS];
pthread_t supervisores[2];

sem_t caja;
sem_t planilla;
sem_t taquilla;

int pedidos_atendidos[MAX_MESONEROS];
int total_pedidos_atendidos = 0;

int current_turn = 0;

void *mesonero(void *arg)
{
    int mesonero_id = *(int *)arg;
    int pedidos_atendidos_mesonero = 0;
    int caja_disponible = 0;

    while (1)
    {
        // Esperar a que la caja esté disponible
        sem_wait(&caja);
        caja_disponible = 1;

        // Tomar pedidos si no se está en la caja
        if (caja_disponible)
        {
            printf("Mesonero %d tomando pedido en mesa\n", mesonero_id);
            // Registrar pedido en la pantalla
            // ...
            // Esperar a que el pedido esté listo en la taquilla
            sem_wait(&taquilla);
            printf("Mesonero %d llevando pedido a la mesa\n", mesonero_id);
            // Llevar el pedido a la mesa
            // ...
            // Cobrar en la caja
            printf("Mesonero %d cobrando en caja\n", mesonero_id);
            // Cobrar en la caja
            // ...
            // Incrementar el contador de pedidos atendidos
            pedidos_atendidos_mesonero++;
            total_pedidos_atendidos++;
            caja_disponible = 0;
            // Avisar a un supervisor cada 10 pedidos
            if (pedidos_atendidos_mesonero % 10 == 0)
            {
                sem_wait(&planilla);
                printf("Mesonero %d avisa al supervisor\n", mesonero_id);
                // Avisar al supervisor
                // ...
                sem_post(&planilla);
            }
        }

        // Liberar la caja
        sem_post(&caja);
    }
}

void *supervisor(void *arg)
{
    int supervisor_id = *(int *)arg;
    int mesonero_id = 0;

    while (1)
    {
        // Esperar a que un mesonero avise
        sem_wait(&planilla);
        printf("Supervisor %d llenando planilla\n", supervisor_id);
        // Llenar la planilla con el número de mesonero
        // ...
        sem_post(&planilla);
        // Esperar un tiempo para el cambio de turno
        printf("Supervisor %d esperando cambio de turno\n", supervisor_id);
        sleep(10);
        // Cambiar de turno
        current_turn = (current_turn + 1) % 2;
        printf("Supervisor %d cambia de turno a %d\n", supervisor_id, current_turn);
    }
}

int main()
{
    // Inicializar semáforos
    sem_init(&caja, 0, MAX_CAJA);
    sem_init(&planilla, 0, 1);
    sem_init(&taquilla, 0, 0);

    // Crear mesoneros
    int mesonero_ids[MAX_MESONEROS];
    for (int i = 0; i < MAX_MESONEROS; i++)
    {
        mesonero_ids[i] = i;
        pthread_create(&mesoneros[i], NULL, mesonero, &mesonero_ids[i]);
    }

    // Crear supervisores
    int supervisor_ids[2];
    for (int i = 0; i < 2; i++)
    {
        supervisor_ids[i] = i;
        pthread_create(&supervisores[i], NULL, supervisor, &supervisor_ids[i]);
    }

    // Esperar a que el programa sea interrumpido
    while (1)
    {
        // Esperar a una señal de interrupción
        // ...
    }

    // Liberar recursos
    sem_destroy(&caja);
    sem_destroy(&planilla);
    sem_destroy(&taquilla);

    return 0;
}