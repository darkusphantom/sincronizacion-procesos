#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Definición de la estructura Order
typedef struct
{
    int id_table; // Número de mesa del pedido
} Order;

// Definición de la estructura Waiter
typedef struct
{
    int id;
    int total_orders_claimed;
    int in_cashbox;
} Waiter;

// Definición de variables globales
Order orders[100];                                       // Array con los pedidos del restaurante
int numOrders = 0;                                       // Número de pedidos registrados
Waiter waiter[5];                                        // Array con los mesoneros del restaurante
pthread_mutex_t mutexOrders = PTHREAD_MUTEX_INITIALIZER; // Mutex para proteger la variable numOrders
pthread_t tid[5];                                        // Array con los hilos correspondientes a los mesoneros
pthread_t cashier_tid;                                   // Hilo correspondiente al cajero
sem_t sem_table[20];                                     // Array de semáforos correspondientes a las mesas del restaurante
sem_t sem_cashier;                                       // Semáforo correspondiente al cajero

// Definición de la estructura Pedido
typedef struct
{
    int numMesa; // Número de mesa del pedido
} Pedido;

// Definición de variables globales para los pedidos
sem_t semaforoAnuncio; // Semáforo correspondiente al anuncio de pedidos
Pedido pedidos[10];    // Array con los pedidos que están en espera

// Definición de la estructura Cliente
typedef struct
{
    int numMesa; // Número de mesa del cliente
} Cliente;

// Definición de variables globales para la caja
Cliente clientes[10]; // Array con los clientes que están en la caja
pthread_t hiloCaja;   // Hilo correspondiente a la caja
sem_t semaforoCaja;   // Semáforo correspondiente a la caja

// Función que registra un nuevo pedido
void registrarPedido(int numMesa)
{
    // Esperar a que haya una posición libre en el array de pedidos
    sem_wait(&semaforoAnuncio);

    // Registrar el nuevo pedido
    Pedido pedido = {numMesa};
    pedidos[9] = pedido;

    // Liberar el semáforo correspondiente al anuncio de pedidos
    sem_post(&semaforoAnuncio);
}

// Función que anuncia un nuevo pedido
void anunciarPedido()
{
    sem_wait(&semaforoAnuncio);
    sem_post(&semaforoAnuncio);
}

// Función que representa el hilo de atención al cliente en la caja
void *hiloCajaFuncion(void *arg)
{
    while (1)
    {
        // Esperar hasta que haya un cliente en la caja
        sem_wait(&semaforoCaja);

        // Atender al cliente en la caja
        Cliente cliente = clientes[0];
        printf("Atendiendo al cliente de la mesa %d.\n", cliente.numMesa);

        // Eliminar al cliente de la caja
        for (int i = 0; i < 9; i++)
        {
            clientes[i] = clientes[i + 1];
        }
        sem_post(&semaforoCaja);
    }
    return NULL;
}

// Función que coloca un cliente en la caja
void colocarClienteEnCaja(int numMesa)
{
    // Esperar a que la caja esté disponible
    sem_wait(&semaforoCaja);

    // Colocar al cliente en la caja
    Cliente cliente = {numMesa};
    clientes[9] = cliente;

    // Liberar la caja para atender al cliente
    sem_post(&semaforoCaja);
}

// Función que representa el hilo de anuncio de pedidos
void *anuncioPedidosFuncion(void *arg)
{
    while (1)
    {
        // Esperar hasta que haya un nuevo pedido registrado
        sem_wait(&semaforoAnuncio);

        // Anunciar el nuevo pedido
        Pedido pedido = pedidos[0];
        printf("Nuevo pedido en la mesa %d\n", pedido.numMesa);

        // Eliminar el pedido del array de pedidos
        for (int i = 0; i < 9; i++)
        {
            pedidos[i] = pedidos[i + 1];
        }
    }
    return NULL;
}

// Función que representa el hilo de atención al cliente por parte de los mesoneros
void *mesoneroFuncion(void *arg)
{
    int id = *(int *)arg;
    while (1)
    {
        // Esperar hasta que haya un pedido disponible en la mesa del mesonero
        sem_wait(&sem_table[id]);

        // Esperar a que la caja esté disponible
        sem_wait(&sem_cashier);

        // Verificar si el mesonero está en el área de caja
        if (waiter[id].in_cashbox)
        {
            printf("Mesonero %d: Recogiendo el dinero\n", id);
            waiter[id].in_cashbox = 0; // El mesonero ya no está en el área de caja
        }

        // Procesar el pedido
        pthread_mutex_lock(&mutexOrders);
        orders[numOrders].id_table = id;
        numOrders++;
        pthread_mutex_unlock(&mutexOrders);

        // Liberar la mesa para el siguiente cliente
        sem_post(&sem_table[id]);

        // Liberar la caja para el siguiente cliente
        sem_post(&sem_cashier);

        // Si no está en el área de caja, tomar un descanso después de cada 10 pedidos cobrados
        if (!waiter[id].in_cashbox)
        {
            waiter[id].total_orders_claimed++;
            printf("Mesonero %d: Total de pedidos reclamados: %d\n", id, waiter[id].total_orders_claimed);
            if (waiter[id].total_orders_claimed % 10 == 0)
            {
                printf("Mesonero %d: Tomando un descanso\n", id);
                // Simulamos una espera aleatoria para representar el tiempo de descanso
                sleep(5);
            }
        }

        // Llamar a la función para anunciar un nuevo pedido
        anunciarPedido();
    }
    return NULL;
}

// Función que representa el hilo de atención al cliente por parte del cajero
void *cajeroFuncion(void *arg)
{
    while (1)
    {
        // Esperar hasta que haya un pedido registrado
        pthread_mutex_lock(&mutexOrders);
        while (numOrders == 0)
        {
            pthread_mutex_unlock(&mutexOrders);
            sleep(1);
            pthread_mutex_lock(&mutexOrders);
        }

        // Cobrar el pedido más antiguo
        Order order = orders[0];
        printf("Cajero: Cobrando el pedido de la mesa %d.\n", order.id_table);

        // Colocar al cliente en la caja
        colocarClienteEnCaja(order.id_table);

        // Eliminar el pedido cobrado del array de pedidos
        for (int i = 0; i < numOrders - 1; i++)
        {
            orders[i] = orders[i + 1];
        }
        numOrders--;

        // Liberar el mutex para acceder a la variable numOrders
        pthread_mutex_unlock(&mutexOrders);

        // Liberar al cajero para atender al siguiente cliente
        sem_post(&sem_cashier);
    }
    return NULL;
}

int main()
{
    // Inicializar los semáforos
    sem_init(&semaforoAnuncio, 0, 0);
    sem_init(&semaforoCaja, 0, 1);
    sem_init(&sem_cashier, 0, 1);
    for (int i = 0; i < 20; i++)
    {
        sem_init(&sem_table[i], 0, 1);
    }

    // Crear el hilo de anuncio de pedidos
    pthread_t anuncio_tid;
    pthread_create(&anuncio_tid, NULL, anuncioPedidosFuncion, NULL);

    // Crear los hilos de los mesoneros
    int ids[5];
    for (int i = 0; i < 5; i++)
    {
        ids[i] = i;
        pthread_create(&tid[i], NULL, mesoneroFuncion, &ids[i]);
    }

    // Crear el hilo del cajero
    pthread_create(&cashier_tid, NULL, cajeroFuncion, NULL);

    // Crear el hilo de atención al cliente en la caja
    pthread_create(&hiloCaja, NULL, hiloCajaFuncion, NULL);

    // Simular la llegada de clientes a las mesas del restaurante
    for (int i = 0; i < 20; i++)
    {
        int id_mesonero = rand() % 5;
        sem_wait(&sem_table[id_mesonero]);
        printf("Cliente en la mesa %d\n", id_mesonero);
        sem_post(&sem_table[id_mesonero]);
    }

    // Esperar a que todos los hilos terminen su ejecución
    pthread_join(anuncio_tid, NULL);
    for (int i = 0; i < 5; i++)
    {
        pthread_join(tid[i], NULL);
    }
    pthread_join(cashier_tid, NULL);
    pthread_join(hiloCaja, NULL);

    // Destruir los semáforos
    sem_destroy(&semaforoAnuncio);
    sem_destroy(&semaforoCaja);
    sem_destroy(&sem_cashier);
    for (int i = 0; i < 20; i++)
    {
        sem_destroy(&sem_table[i]);
    }

    return 0;
}