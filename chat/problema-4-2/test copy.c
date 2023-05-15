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
int cobros_en_caja[MAX_MESONEROS];
int descansos_mesonero[MAX_MESONEROS];
int total_pedidos_atendidos = 0;

struct supervisor_stats
{
    int turno;
    int total_pedidos;
};

struct mesonero_stats
{
    int total_pedidos;
    int total_cobros;
    int total_descansos;
};

struct supervisor_stats supervisor_stats_turno_diurno;
struct supervisor_stats supervisor_stats_turno_nocturno;
struct mesonero_stats mesonero_stats[MAX_MESONEROS];

int current_turn = 0;

void *mesonero(void *arg)
{
    int mesonero_id = *(int *)arg;
    int pedidos_atendidos_mesonero = 0;
    int cobros_en_caja_mesonero = 0;
    int descansos_mesonero_actual = 0;
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
            // Incrementar el contador de pedidos atendidos y cobros en caja
            pedidos_atendidos_mesonero++;
            cobros_en_caja_mesonero++;
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
                // Incrementar el contador de descansos
                descansos_mesonero_actual++;
            }
        }

        // Liberar la caja
        sem_post(&caja);

        // Actualizar estadísticas de descansos
        if (descansos_mesonero_actual > mesonero_stats[mesonero_id].total_descansos)
        {
            mesonero_stats[mesonero_id].total_descansos = descansos_mesonero_actual;
        }
    }
}

void *supervisor(void *arg)
{
    int supervisor_id = *(int *)arg;
    int mesonero_id = 0;
    int total_pedidos_turno_diurno = 0;
    int total_pedidos_turno_nocturno = 0;

    while (1)
    {
        // Esperar a que un mesonero avise
        sem_wait(&planilla);
        printf("Supervisor %d llenando planilla\n", supervisor_id);
        // Llenar la planilla con el número de mesonero
        // ...
        sem_post(&planilla);
        // Incrementar el total de pedidos contabilizados en el turno actual
        if (current_turn == 0)
        {
            supervisor_stats_turno_diurno.total_pedidos++;
            total_pedidos_turno_diurno++;
        }
        else
        {
            supervisor_stats_turno_nocturno.total_pedidos++;
            total_pedidos_turno_nocturno++;
        }
        // Esperar un tiempo para el cambio de turno
        printf("Supervisor %d esperando cambio de turno\n", supervisor_id);
        sleep(10);
        // Cambiar de turno
        current_turn = (current_turn + 1) % 2;
        printf("Supervisor %d cambia de turno a %d\n", supervisor_id, current_turn);
        // Imprimir estadísticas del turno anterior
        if (current_turn == 0)
        {
            printf("Turno diurno:\n");
            printf("Total de pedidos por supervisor: %d\n", supervisor_stats_turno_diurno.total_pedidos);
            printf("Total de pedidos por mesonero:\n");
            for (int i = 0; i < MAX_MESONEROS; i++)
            {
                printf("Mesonero %d: %d\n", i, mesonero_stats[i].total_pedidos);
            }
        }
        else
        {
            printf("Turno nocturno:\n");
            printf("Total de pedidos por supervisor: %d\n", supervisor_stats_turno_nocturno.total_pedidos);
            printf("Total de pedidos por mesonero:\n");
            for (int i = 0; i < MAX_MESONEROS; i++)
            {
                printf("Mesonero %d: %d\n", i, mesonero_stats[i].total_pedidos);
            }
        }
        // Actualizar estadísticas de mesoneros
        for (int i = 0; i < MAX_MESONEROS; i++)
        {
            mesonero_stats[i].total_cobros += cobros_en_caja[i];
            mesonero_stats[i].total_pedidos += pedidos_atendidos[i];
            cobros_en_caja[i] = 0;
            pedidos_atendidos[i] = 0;
        }
        // Imprimir estadísticas generales
        printf("Total de pedidos: %d\n", total_pedidos_atendidos);
        printf("Número de descansos por mesonero:\n");
        for (int i = 0; i < MAX_MESONEROS; i++)
        {
            printf("Mesonero %d: %d\n", i, mesonero_stats[i].total_descansos);
            mesonero_stats[i].total_descansos = 0;
        }
    }
}

int main()
{
    sem_init(&caja, 0, MAX_CAJA);
    sem_init(&planilla, 0, MAX_PLANILLA);
    sem_init(&taquilla, 0, 0);

    int mesonero_ids[MAX_MESONEROS];
    for (int i = 0; i < MAX_MESONEROS; i++)
    {
        mesonero_ids[i] = i;
        pthread_create(&mesoneros[i], NULL, mesonero, &mesonero_ids[i]);
    }

    int supervisor_ids[2] = {0, 1};
    pthread_create(&supervisores[0], NULL, supervisor, &supervisor_ids[0]);
    pthread_create(&supervisores[1], NULL, supervisor, &supervisor_ids[1]);

    pthread_join(supervisores[0], NULL);

    return 0;
}