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
int total_pedidos_turno_diurno = 0;
int total_pedidos_turno_nocturno = 0;
int mesonero_turno_max_pedidos = 0;
int mesonero_turno_max_cobros = 0;
int total_cobros_en_caja = 0;

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

        // Descansar si es necesario
        if (descansos_mesonero_actual == 2)
        {
            printf("Mesonero %d se va a descansar\n", mesonero_id);
            // Ir a descansar
            // ...
            descansos_mesonero_actual = 0;
        }
    }
}

void *supervisor(void *arg)
{
    int supervisor_id = *(int *)arg;
    int turno_actual = 0;
    int total_pedidos_supervisor = 0;

    while (1)
    {
        // Llenar planilla
        sem_wait(&planilla);
        printf("Supervisor %d llenando planilla\n", supervisor_id);
        // Llenar planilla
        // ...
        sem_post(&planilla);
        // Esperar cambio de turno
        sem_wait(&planilla);
        printf("Supervisor %d esperando cambio de turno\n", supervisor_id);
        // Esperar cambio de turno
        // ...
        sem_post(&planilla);
        // Cambiar de turno si es necesario
        if (current_turn == 0 && supervisor_id == 0)
        {
            current_turn = 1;
            supervisor_stats_turno_diurno.turno = 1;
            supervisor_stats_turno_diurno.total_pedidos = total_pedidos_turno_diurno;
            total_pedidos_turno_diurno = 0;
        }
        else if (current_turn == 1 && supervisor_id == 1)
        {
            current_turn = 0;
            supervisor_stats_turno_nocturno.turno = 0;
            supervisor_stats_turno_nocturno.total_pedidos = total_pedidos_turno_nocturno;
            total_pedidos_turno_nocturno = 0;
        }
        // Actualizar estadísticas de pedidos por supervisor
        if (supervisor_id == 0)
        {
            total_pedidos_supervisor = supervisor_stats_turno_diurno.total_pedidos;
        }
        else
        {
            total_pedidos_supervisor = supervisor_stats_turno_nocturno.total_pedidos;
        }
        printf("Turno %s:\n", current_turn == 0 ? "nocturno" : "diurno");
        printf("Total de pedidos por supervisor %d: %d\n", supervisor_id, total_pedidos_supervisor);
        // Dormir un segundo antes de volver a comenzar el ciclo
        sleep(1);
    }
}

int main()
{
    int i;
    int mesonero_ids[MAX_MESONEROS];
    int supervisor_ids[2];

    // Inicializar semáforos
    sem_init(&caja, 0, MAX_CAJA);
    sem_init(&planilla, 0, MAX_PLANILLA);
    sem_init(&taquilla, 0, 0);

    // Crear hilos de mesoneros
    for (i = 0; i < MAX_MESONEROS; i++)
    {
        mesonero_ids[i] = i;
        pthread_create(&mesoneros[i], NULL, mesonero, &mesonero_ids[i]);
    }

    // Crear hilos de supervisores
    for (i = 0; i < 2; i++)
    {
        supervisor_ids[i] = i;
        pthread_create(&supervisores[i], NULL, supervisor, &supervisor_ids[i]);
    }

    // Esperar a que los hilos terminen (esto nunca sucederá)
    for (i = 0; i < MAX_MESONEROS; i++)
    {
        pthread_join(mesoneros[i], NULL);
    }
    for (i = 0; i < 2; i++)
    {
        pthread_join(supervisores[i], NULL);
    }

    return 0;
}