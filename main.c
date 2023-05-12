#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_MESAS 20
#define NUM_MESONEROS 10
#define NUM_SUPERVISORES 2
#define CAPACIDAD_CAJA 2

// Variables compartidas
int pedidos_registrados[NUM_MESAS];
int pedidos_cobrados[NUM_MESONEROS];
int descansos[NUM_MESONEROS];
int turno_actual = 0;
int pedidos_turno_actual = 0;
int cobros_turno_actual = 0;
int pedidos_totales = 0;
int cobros_totales = 0;

// Semaforos
sem_t sem_caja;
sem_t sem_mesoneros;
sem_t sem_supervisores;
sem_t sem_turno;
sem_t sem_descansos;

// Funciones auxiliares
int siguiente_mesa()
{
    // Devuelve el indice de la siguiente mesa disponible
    for (int i = 0; i < NUM_MESAS; i++)
    {
        if (pedidos_registrados[i] == 0)
        {
            return i;
        }
    }
    return -1;
}

void cobrar(int mesonero)
{
    // Simula el proceso de cobro en la caja
    printf("Mesonero %d: cobrando\n", mesonero);
    cobros_totales++;
    cobros_turno_actual++;
    pedidos_cobrados[mesonero]++;
    sem_post(&sem_caja);
}

void tomar_descanso(int mesonero)
{
    // Simula el proceso de tomar un descanso
    printf("Mesonero %d: tomando descanso\n", mesonero);
    descansos[mesonero]++;
    sem_post(&sem_descansos);
}

void registrar_pedido(int mesonero, int mesa)
{
    // Simula el proceso de registro de un pedido en la pantalla
    printf("Mesonero %d: tomando pedido en la mesa %d\n", mesonero, mesa);
    pedidos_registrados[mesa] = mesonero + 1;
    pedidos_totales++;
    pedidos_turno_actual++;
}

void entregar_pedido(int mesa)
{
    // Simula el proceso de entrega de un pedido en la taquilla
    printf("Pedido en la mesa %d listo para ser entregado\n", mesa);
    pedidos_registrados[mesa] = 0;
    sem_post(&sem_supervisores);
}

void *mesonero(void *arg)
{
    int mesonero_id = *((int *)arg);
    while (1)
    {
        sem_wait(&sem_mesoneros);
        sem_wait(&sem_caja);
        printf("Mesonero %d: atendiendo en la caja\n", mesonero_id);
        int mesa = siguiente_mesa();
        if (mesa != -1)
        {
            sem_post(&sem_mesoneros);
            registrar_pedido(mesonero_id, mesa);
            sem_post(&sem_caja);
            while (pedidos_registrados[mesa] != 0)
            {
                // Espera a que el pedido esté listo
            }
            entregar_pedido(mesa);
            if ((pedidos_turno_actual % 10) == 0)
            {
                tomar_descanso(mesonero_id);
            }
        }
        else
        {
            sem_post(&sem_caja);
            sem_post(&sem_mesoneros);
        }
    }
}

void *supervisor(void *arg)
{
    int supervisor_id = *((int *)arg);
    while (1)
    {
        sem_wait(&sem_supervisores);
        sem_wait(&sem_turno);
        int mesonero_id = turno_actual % NUM_MESONEROS;
        printf("Supervisor %d: llenando planilla para mesonero %d\n", supervisor_id, mesonero_id);
        turno_actual++;
        pedidos_turno_actual = 0;
        cobros_turno_actual = 0;
        sem_post(&sem_turno);
    }
}

void *cajero(void *arg)
{
    int mesonero_id = *(int *)arg;

    while (1)
    {
        sem_wait(&sem_descansos);
        sem_wait(&sem_caja);
        // Espera a que haya un mesonero disponible para atender
        sem_wait(&sem_mesoneros);
        sem_post(&sem_mesoneros);
        cobrar(mesonero_id);
    }
}

int main()
{
    // Inicialización de semáforos
    sem_init(&sem_caja, 0, CAPACIDAD_CAJA);
    sem_init(&sem_mesoneros, 0, NUM_MESONEROS - CAPACIDAD_CAJA);
    sem_init(&sem_supervisores, 0, 0);
    sem_init(&sem_turno, 0, 1);
    sem_init(&sem_descansos, 0, 0);
    // Creación de hilos
    pthread_t mesoneros[NUM_MESONEROS];
    pthread_t supervisores[NUM_SUPERVISORES];
    pthread_t cajero_thread;
    int mesonero_actual = 0;
    int mesoneros_id[NUM_MESONEROS];
    int supervisores_id[NUM_SUPERVISORES];
    for (int i = 0; i < NUM_MESONEROS; i++)
    {
        mesoneros_id[i] = i;
        mesonero_actual = mesoneros_id[i];
        pthread_create(&mesoneros[i], NULL, mesonero, &mesoneros_id[i]);
    }
    for (int i = 0; i < NUM_SUPERVISORES; i++)
    {
        supervisores_id[i] = i;
        pthread_create(&supervisores[i], NULL, supervisor, &supervisores_id[i]);
    }
    pthread_create(&cajero_thread, NULL, cajero, &mesonero_actual);
    // Espera a que los hilos terminen (en teoria nunca terminan)
    for (int i = 0; i < NUM_MESONEROS; i++)
    {
        pthread_join(mesoneros[i], NULL);
    }
    for (int i = 0; i < NUM_SUPERVISORES; i++)
    {
        pthread_join(supervisores[i], NULL);
    }
    pthread_join(cajero_thread, NULL);
    // Imprime las estadisticas
    printf("Pedidos contabilizados por supervisor:\n");
    for (int i = 0; i < NUM_SUPERVISORES; i++)
    {
        printf("Supervisor %d: %d\n", i, pedidos_totales / NUM_SUPERVISORES);
    }
    printf("Turno con mayor número de pedidos contabilizados: %d\n", pedidos_totales / NUM_MESONEROS);
    printf("Número total de pedidos por mesonero:\n");
    for (int i = 0; i < NUM_MESONEROS; i++)
    {
        printf("Mesonero %d: %d\n", i, pedidos_totales / NUM_MESONEROS);
    }
    printf("Número de cobros en caja por mesonero:\n");
    for (int i = 0; i < NUM_MESONEROS; i++)
    {
        printf("Mesonero %d: %d\n", i, cobros_totales / NUM_MESONEROS);
    }
    printf("Número de descansos por mesonero:\n");
    for (int i = 0; i < NUM_MESONEROS; i++)
    {
        printf("Mesonero %d: %d\n", i, descansos[i]);
    }
    int max_pedidos = 0;
    int max_cobros = 0;
    int mesonero_max_pedidos = 0;
    for (int i = 0; i < NUM_MESONEROS; i++)
    {
        if (pedidos_totales / NUM_MESONEROS > max_pedidos)
        {
            max_pedidos = pedidos_totales / NUM_MESONEROS;
            mesonero_max_pedidos = i;
        }
        if (cobros_totales / NUM_MESONEROS > max_cobros)
        {
            max_cobros = cobros_totales / NUM_MESONEROS;
        }
    }
    printf("Mesonero del turno del día: máximo número de pedidos: %d, máximo número de cobros: %d\n", mesonero_max_pedidos, max_cobros);
    return 0;
}