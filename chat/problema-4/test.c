#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_WAITER 10
#define MAX_SUPERVISOR 2
#define MAX_TABLE 20
#define MAX_ORDERS 10

// Definición de la estructura Order
typedef struct
{
    int id_table;  // Número de mesa del pedido
    int waiter_id; // ID del mesonero que atendió el pedido
} Order;

// Definición de la estructura Waiter
typedef struct
{
    int id;
    int total_orders;   // Número total de pedidos atendidos
    int orders_claimed; // Número de pedidos cobrado
    int in_cashbox;     // Indica si el mesonero está en el área de caja (1 = Sí, 0 = No)
    int rests;          // Número de descansos
} Waiter;

// Definición de la estructura Supervisor
typedef struct
{
    int id;
    int total_orders; // Número total de pedidos contabilizados
} Supervisor;

// Definición de variables globales
Order orders[100];                                         // Array con los pedidos del restaurante
int numOrders = 0;                                         // Número de pedidos registrados
Waiter waiter[MAX_WAITER];                                 // Array con los mesoneros del turno
Supervisor supervisor[MAX_SUPERVISOR];                     // Array con los supervisores
pthread_mutex_t mutex_orders = PTHREAD_MUTEX_INITIALIZER;  // Mutex para proteger la variable numOrders
pthread_mutex_t mutex_cashbox = PTHREAD_MUTEX_INITIALIZER; // Mutex para proteger el área de caja
pthread_t tid[MAX_WAITER];                                 // Array con los hilos correspondientes a los mesoneros
pthread_t sup_tid[MAX_SUPERVISOR];                         // Array con los hilos correspondientes a los supervisores
pthread_t cashier_tid;                                     // Hilo correspondiente al cajero
sem_t sem_table[MAX_TABLE];                                // Array de semáforos correspondientes a las mesas
sem_t sem_cashier;                                         // Semáforo correspondiente al cajero
sem_t sem_rest;

// Función que registra un nuevo pedido
void registrarPedido(int numMesa, int id_waiter)
{
    pthread_mutex_lock(&mutex_orders);
    orders[numOrders].id_table = numMesa;
    orders[numOrders].waiter_id = id_waiter;
    numOrders++;
    pthread_mutex_unlock(&mutex_orders);
}

// Función que representa el hilo de los supervisores
void *supervisorFuncion(void *arg)
{
    int id = *(int *)arg;
    while (1)
    {
        // Esperar a que un mesonero solicite un descanso
        sem_wait(&sem_rest);

        // Tomar los datos del mesonero que solicita el descanso
        pthread_mutex_lock(&mutex_orders);
        int waiter_id = orders[numOrders - 1].waiter_id;
        pthread_mutex_unlock(&mutex_orders);

        // Contabilizar los pedidos atendidos por el mesonero en el turno
        supervisor[id].total_orders += waiter[waiter_id].total_orders;
        printf("Supervisor %d: Mesonero %d ha atendido %d pedidos.\n", id, waiter_id, waiter[waiter_id].total_orders);
        waiter[waiter_id].total_orders = 0; // Reiniciar el conteo de pedidos

        // Dar por terminado el descanso del mesonero
        sem_post(&sem_rest);
        sleep(3);
    }
}

// Función que representa el hilo de atención al cliente por parte de los mesoneros
void *mesoneroFuncion(void *arg)
{
    int id = *(int *)arg;
    while (1)
    {
        // Esperar hasta que haya un pedido disponible en la mesa del mesonero
        sem_wait(&sem_table[id]);

        // Registrar el pedido atendido
        waiter[id].total_orders++;
        pthread_mutex_lock(&mutex_orders);
        orders[numOrders].id_table = id;
        orders[numOrders].waiter_id = id;
        numOrders++;
        pthread_mutex_unlock(&mutex_orders);

        // Si el mesonero no está en caja, verificar si puede ir a cobrar
        if (!waiter[id].in_cashbox && sem_trywait(&sem_cashier) == 0)
        {
            waiter[id].in_cashbox = 1; // El mesonero está en el área de caja

            // Esperar a que la caja esté disponible
            pthread_mutex_lock(&mutex_cashbox);
            while (waiter[0].in_cashbox && waiter[1].in_cashbox)
                pthread_mutex_unlock(&mutex_cashbox);

            // Cobrar pedidos hasta completar 10 o hasta que no haya más pedidos
            int orders_to_claim = 10 - waiter[id].orders_claimed;
            for (int i = 0; i < orders_to_claim && numOrders > 0; i++)
            {
                // Cobrar el pedido más antiguo
                pthread_mutex_lock(&mutex_orders);
                Order order = orders[0];
                printf("Mesonero %d: Cobrando el pedido de la mesa %d.\n", id, order.id_table);
                waiter[i].orders_claimed++;
                // Eliminar el pedido cobrado del array de pedidos
                for (int j = 0; j < numOrders - 1; j++)
                {
                    orders[j] = orders[j + 1];
                }
                numOrders--;
                pthread_mutex_unlock(&mutex_orders);
            }
            waiter[id].in_cashbox = 0; // El mesonero ya no está en el área de caja
            pthread_mutex_unlock(&mutex_cashbox);
        }

        // Si el mesonero ha cobrado 10 pedidos, solicitar un descanso
        if (waiter[id].orders_claimed % MAX_ORDERS == 0)
        {
            printf("Mesonero %d: Solicitando un descanso.\n", id);
            sem_post(&sem_rest);
            waiter[id].rests++;
            waiter[id].orders_claimed = 0; // Reiniciar el conteo de pedidos cobrado
        }

        // Liberar la mesa y la caja
        sem_post(&sem_table[id]);
        sem_post(&sem_cashier);
        sleep(3);
    }
}

void *generarPedidosFuncion(void *arg)
{
    int mesa;
    int i = 0;
    while (1)
    {
        // Generar un pedido aleatorio
        mesa = rand() % MAX_TABLE + 1;
        int mesonero = rand() % MAX_WAITER;
        registrarPedido(mesa, mesonero);

        // Esperar un tiempo aleatorio antes de generar el siguiente pedido
        int espera = rand() % 5 + 1;
        sleep(espera);

        i++;
        if (i % MAX_ORDERS == 0)
        {
            // Si se han generado 10 pedidos, imprimir un mensaje de aviso
            printf("Se han generado 10 pedidos.\n");
        }
    }
}

void showResults(int id_waiter)
{
    int total_orders = waiter[id_waiter].total_orders;
    int orders_claimed = waiter[id_waiter].orders_claimed;
    int total_waiter_rests = waiter[id_waiter].rests;
    printf("Mesonero %d: ", id_waiter);
    printf("%d pedidos atendidos, %d pedidos cobrados y % d descansos.\n", total_orders, orders_claimed, total_waiter_rests);
}

int main()
{
    // Inicializar los semáforos
    sem_init(&sem_cashier, 0, 1);
    sem_init(&sem_rest, 0, 1);
    for (int i = 0; i < MAX_TABLE; i++)
    {
        sem_init(&sem_table[i], 0, 1);
    }

    // Crear los hilos de los supervisores
    int sup_ids[MAX_SUPERVISOR] = {0, 1};
    for (int i = 0; i < MAX_SUPERVISOR; i++)
    {
        pthread_create(&sup_tid[i], NULL, supervisorFuncion, &sup_ids[i]);
    }

    // Crear los hilos de los mesoneros
    int waiter_ids[MAX_WAITER] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int i = 0; i < MAX_WAITER; i++)
    {
        pthread_create(&tid[i], NULL, mesoneroFuncion, &waiter_ids[i]);
    }

    // Simular la llegada de clientes a las mesas del restaurante
    // Crear el hilo de generación de pedidos
    pthread_t gen_tid;
    pthread_create(&gen_tid, NULL, generarPedidosFuncion, NULL);

    // Mostrar resultados
    printf("Resultados del turno:\n");
    for (int i = 0; i < MAX_SUPERVISOR; i++)
    {
        printf("Supervisor %d: %d pedidos contabilizados.\n", i, supervisor[i].total_orders);
    }
    // Obtener el turno y mesonero con mayor número de pedidos
    int max_orders = 0, max_waiter;
    for (int i = 0; i < MAX_WAITER; i++)
    {
        if (waiter[i].total_orders > max_orders)
        {
            max_orders = waiter[i].total_orders;
            max_waiter = i;
            showResults(max_waiter);
        }
    }
}