#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_PANTALLAS 4
#define NUM_MESONEROS 10
#define NUM_MESAS 20
#define NUM_SUPERVISORES 2
#define NUM_TURNOS 2

sem_t mutex_pedidos;
sem_t mutex_cobros;
sem_t mutex_descansos;
sem_t mutex_planilla;
sem_t sem_turno[NUM_TURNOS];
int pedidos_supervisor[NUM_SUPERVISORES][NUM_TURNOS][NUM_MESONEROS] = {0};
int cobros_mesonero[NUM_MESONEROS] = {0};
int descansos_mesonero[NUM_MESONEROS] = {0};
int max_pedidos[NUM_TURNOS] = {0};
int max_cobros[NUM_TURNOS] = {0};
int max_pedidos_mesonero[NUM_TURNOS] = {0};
int max_cobros_mesonero[NUM_TURNOS] = {0};
int mesonero_turno_dia = -1;
int max_pedidos_turno_dia = 0;
int max_cobros_turno_dia = 0;

void *supervisor(void *arg)
{
    int supervisor_id = *(int *)arg;
    int turno_id = 0;
    while (1)
    {
        // Esperar a que un mesonero avise
        sem_wait(&sem_turno[turno_id]);
        // Registrar los pedidos y cobros del mesonero
        for (int i = 0; i < NUM_MESONEROS; i++)
        {
            int pedidos = 0;
            int cobros = 0;
            sem_wait(&mutex_pedidos);
            pedidos = pedidos_supervisor[supervisor_id][turno_id][i];
            pedidos_supervisor[supervisor_id][turno_id][i] = 0;
            sem_post(&mutex_pedidos);
            sem_wait(&mutex_cobros);
            cobros = cobros_mesonero[i];
            cobros_mesonero[i] = 0;
            sem_post(&mutex_cobros);
            printf("Supervisor %d - Turno %d - Mesonero %d - Pedidos: %d - Cobros: %d\n", supervisor_id, turno_id, i, pedidos, cobros);
            // Actualizar los máximos
            // if (pedidos > max_pedidos[turno_id])
            // {
            //     max_pedidos[turno_id] = pedidos;
            // }
            // if (cobros > max_cobros[turno_id])
            // {
            //     max_cobros[turno_id] = cobros;
            // }
            // if (pedidos > max_pedidos_mesonero[turno_id][i])
            // {
            //     max_pedidos_mesonero[turno_id][i] = pedidos;
            // }
            // if (cobros > max_cobros_mesonero[turno_id][i])
            // {
            //     max_cobros_mesonero[turno_id][i] = cobros;
            // }
            // Actualizar el mesonero del turno del día
            if (turno_id == 0 && cobros > max_cobros_turno_dia)
            {
                max_cobros_turno_dia = cobros;
                mesonero_turno_dia = i;
                max_pedidos_turno_dia = pedidos;
            }
        }
        // Tomar un descanso
        for (int i = 0; i < NUM_MESONEROS; i++)
        {
            sem_wait(&mutex_descansos);
            descansos_mesonero[i]++;
            sem_post(&mutex_descansos);
        }
        // Actualizar el turno
        turno_id = (turno_id + 1) % NUM_TURNOS;
        // Llenar la planilla
        sem_wait(&mutex_planilla);
        printf("Supervisor %d - Turno %d - Planilla: Mesonero %d - Pedidos: %d - Cobros: %d\n", supervisor_id, turno_id, mesonero_turno_dia, max_pedidos_turno_dia, max_cobros_turno_dia);
        sem_post(&mutex_planilla);
        // Esperar al siguiente aviso
        sem_wait(&sem_turno[turno_id]);
    }
}

int main()
{
    sem_init(&mutex_pedidos, 0, 1);
    sem_init(&mutex_cobros, 0, 1);
    sem_init(&mutex_descansos, 0, 1);
    sem_init(&mutex_planilla, 0, 1);
    for (int i = 0; i < NUM_TURNOS; i++)
    {
        sem_init(&sem_turno[i], 0, 0);
    }
    pthread_t supervisor_thread[NUM_SUPERVISORES];
    int supervisor_id[NUM_SUPERVISORES];
    for (int i = 0; i < NUM_SUPERVISORES; i++)
    {
        supervisor_id[i] = i;
        pthread_create(&supervisor_thread[i], NULL, supervisor, &supervisor_id[i]);
    }
    // El programa principal simula el registro de pedidos y cobros
    // y avisa a los supervisores cada 10 pedidos cobrados por cada mesonero
    int mesonero_id = 0;
    int turno_id = 0;
    for (int i = 0; i < NUM_MESAS; i++)
    {
        sem_wait(&sem_turno[turno_id]);
        sem_wait(&mutex_pedidos);
        pedidos_supervisor[0][turno_id][mesonero_id]++;
        sem_post(&mutex_pedidos);
        sem_wait(&mutex_cobros);
        cobros_mesonero[mesonero_id]++;
        sem_post(&mutex_cobros);
        if (cobros_mesonero[mesonero_id] % 10 == 0)
        {
            sem_post(&sem_turno[turno_id]);
        }
        mesonero_id = (mesonero_id + 1) % NUM_MESONEROS;
        turno_id = (turno_id + 1) % NUM_TURNOS;
    }
    return 0;
}