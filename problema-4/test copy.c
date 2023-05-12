#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

// Definir la estructura del mesonero
typedef struct
{
    int id;
    int in_cashbox;
    int total_served;
    int total_paid;
    int total_breaks;
} Waiter;

// Definir la estructura del supervisor
typedef struct
{
    int total_served;
    int total_paid;
    int total_breaks;
} Supervisor;

// Definir el semáforo y el mutex para controlar el acceso al área de caja
sem_t cashbox_sem;
pthread_mutex_t cashbox_mutex;

// Definir el semáforo para avisar a los supervisores cada 10 pedidos cobrados
sem_t paid_sem;

// Definir una lista de mesoneros y un supervisor
Waiter waiters[5];
Supervisor supervisor;

// Función para generar los pedidos
void *generate_orders(void *arg)
{
    int orders = 0;
    while (1)
    {
        // Elegir un mesonero al azar
        Waiter *waiter = &waiters[rand() % 5];
        // Verificar si el mesonero está en caja o no
        if (!waiter->in_cashbox)
        {
            // Tomar un descanso al azar (entre 1 y 3 segundos)
            usleep((rand() % 3 + 1) * 1000000);
            // Atender una mesa al azar (entre 1 y 5 pedidos)
            int num_orders = rand() % 5 + 1;
            // Actualizar los datos del mesonero y del supervisor
            waiter->total_served += num_orders;
            supervisor.total_served += num_orders;
            orders += num_orders;
            // Verificar si se han cobrado 10 pedidos
            if (orders % 10 == 0)
            {
                // Avisar a los supervisores que deben tomar un descanso
                sem_post(&paid_sem);
            }
        }
        else
        {
            // Esperar un tiempo aleatorio antes de intentar atender otra mesa
            usleep((rand() % 5 + 1) * 1000000);
        }
    }
    return NULL;
}

// Función para que el mesonero cobre los pedidos
void *cash_orders(void *arg)
{
    Waiter *waiter = (Waiter *)arg;
    while (1)
    {
        // Tomar un descanso al azar (entre 1 y 5 segundos)
        usleep((rand() % 5 + 1) * 1000000);
        // Verificar si el mesonero puede entrar al área de caja
        if (sem_trywait(&cashbox_sem) == 0)
        {
            // Bloquear el mutex para evitar que otro mesonero entre a la caja
            pthread_mutex_lock(&cashbox_mutex);
            waiter->in_cashbox = 1;
            // Cobrar los pedidos al azar (entre 1 y 5 segundos por pedido)
            int num_orders = rand() % 5 + 1;
            usleep((rand() % 5 + 1) * num_orders * 1000000);
            waiter->total_paid += num_orders;
            supervisor.total_paid += num_orders;
            waiter->in_cashbox = 0;
            // Desbloquear el mutex y liberar el semáforo para avisar a los supervisores
            pthread_mutex_unlock(&cashbox_mutex);
            sem_post(&paid_sem);
        }
    }
    return NULL;
}

// Función para que el supervisor tome un descanso
void take_break(Supervisor *supervisor)
{
    supervisor->total_breaks++;
    // Tomar un descanso al azar (entre 1 y 5 segundos)
    usleep((rand() % 5 + 1) * 1000000);
}

// Función para mostrar los resultados al final del turno
void show_results()
{
    printf("Resultados del turno:\n");
    for (int i = 0; i < 5; i++)
    {
        Waiter *waiter = &waiters[i];
        printf(" - Mesonero %d: atendió %d mesas, cobró %d pedidos y tomó %d descansos.\n", waiter->id, waiter->total_served, waiter->total_paid, waiter->total_breaks);
    }
    printf(" - Supervisor: atendió %d mesas, cobró %d pedidos y tomó %d descansos.\n", supervisor.total_served, supervisor.total_paid, supervisor.total_breaks);
}

int main()
{
    // Inicializar el semáforo y el mutex para controlar el acceso al área de caja
    sem_init(&cashbox_sem, 0, 2);
    pthread_mutex_init(&cashbox_mutex, NULL);

    // Inicializar el semáforo para avisar a los supervisores cada 10 pedidos cobrados
    sem_init(&paid_sem, 0, 0);

    // Inicializar la lista de mesoneros y el supervisor
    for (int i = 0; i < 5; i++)
    {
        waiters[i].id = i;
        waiters[i].in_cashbox = 0;
        waiters[i].total_served = 0;
        waiters[i].total_paid = 0;
        waiters[i].total_breaks = 0;
    }
    supervisor.total_served = 0;
    supervisor.total_paid = 0;
    supervisor.total_breaks = 0;

    // Crear hilos para generar los pedidos y cobrar los pedidos
    pthread_t order_thread;
    pthread_t cash_threads[5];
    pthread_create(&order_thread, NULL, generate_orders, NULL);
    for (int i = 0; i < 5; i++)
    {
        pthread_create(&cash_threads[i], NULL, cash_orders, (void *)&waiters[i]);
    }

    // Esperar a que se cobren todos los pedidos y los supervisores tomen sus descansos
    for (int i = 0; i < 5; i++)
    {
        pthread_join(cash_threads[i], NULL);
    }
    for (int i = 0; i < 5 / 2; i++)
    {
        sem_wait(&paid_sem);
        take_break(&supervisor);
    }

    // Mostrar los resultados al final del turno
    show_results();

    // Liberar los recursos
    sem_destroy(&cashbox_sem);
    pthread_mutex_destroy(&cashbox_mutex);
    sem_destroy(&paid_sem);

    return 0;
}