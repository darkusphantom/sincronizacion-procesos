#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

// Definir los semáforos necesarios
sem_t clientes_en_cola;
sem_t mesoneros_disponibles;
sem_t max_mesoneros_en_caja;
sem_t mesonero_en_caja_cola;
sem_t mesonero_en_descanso;
sem_t mesoneros_fuera_de_caja;
sem_t caja_disponible;
sem_t mutex_caja;
sem_t fin_turno_actual;
sem_t inicio_nuevo_turno;
sem_t descanso_mesonero;

// Definir variables necesarias
int clientes_atendidos = 0;
int pedidos_cobrados = 0;
int mesonero_en_caja = 0;

// Función para atender a un cliente
void atender_cliente()
{
    // Atender al cliente
    printf("Atendiendo a un cliente\n");
}

// Función para tomar un pedido en la mesa
void tomar_pedido()
{
    // Tomar el pedido en la mesa
    printf("Tomando un pedido en la mesa\n");
}

// Función para registrar un pedido en la pantalla correspondiente
void registrar_pedido()
{
    // Registrar el pedido en la pantalla correspondiente
    printf("Registrando un pedido en la pantalla\n");
}

// Función para esperar a que el pedido esté listo en la taquilla
void esperar_pedido_en_taquilla()
{
    // Esperar a que el pedido esté listo en la taquilla
    printf("Esperando a que el pedido esté listo en la taquilla\n");
}

// Función para cobrar a los clientes
void cobrar()
{
    // Cobrar a los clientes
    printf("Cobrando a los clientes\n");
    pedidos_cobrados++;
}

// Función para cambiar de turno
void cambiar_turno()
{
    // Cambiar de turno
    printf("Cambiando de turno\n");
}

// Función para el ciclo de vida de un mesonero
void *ciclo_de_vida_mesonero(void *arg)
{
    while (1)
    {
        // Esperar a que un mesonero esté disponible para tomar un pedido
        sem_wait(&mesoneros_disponibles);

        // Revisar si el mesonero ha atendido a 10 clientes
        if (clientes_atendidos % 10 == 0)
        {
            // Solicitar un descanso
            sem_post(&descanso_mesonero);
            sem_wait(&mesonero_en_descanso);
        }

        // Tomar el pedido en la mesa
        tomar_pedido();

        // Registrar el pedido en la pantalla correspondiente
        registrar_pedido();

        // Llevar el pedido a la mesa cuando esté listo en la taquilla
        esperar_pedido_en_taquilla();

        // Cobrar en caja a los clientes
        sem_wait(&caja_disponible);
        sem_wait(&max_mesoneros_en_caja);
        sem_wait(&mutex_caja);
        mesonero_en_caja = 1;
        sem_post(&mesonero_en_caja_cola);
        cobrar();
        mesonero_en_caja = 0;
        sem_post(&mutex_caja);
        sem_post(&max_mesoneros_en_caja);
        sem_post(&caja_disponible);

        // Incrementar el contador de clientes atendidos
        clientes_atendidos++;

        // Liberar al mesonero para que pueda tomar otro pedido
        sem_post(&mesoneros_disponibles);
    }
    pthread_exit(NULL);
}

// Función para el ciclo de vida de un cliente
void *ciclo_de_vida_cliente(void *arg)
{
    // Esperar un tiempo aleatorio antes de llegar a la cola
    sleep(rand() % 5 + 1);

    // Llegar a la cola
    printf("Cliente llega a la cola\n");
    sem_post(&clientes_en_cola);
    pthread_exit(NULL);
}

// Función para el ciclo de vida de un supervisor
void *ciclo_de_vida_supervisor(void *arg)
{
    while (1)
    {
        // Esperar a que se hayan cobrado 10 pedidos
        sleep(10);
        printf("Supervisor: se han cobrado 10 pedidos\n");
    }
    pthread_exit(NULL);
}

// Función para el ciclo de vida de un turno
void *ciclo_de_vida_turno(void *arg)
{
    while (1)
    {
        // Esperar a que termine el turno actual
        sem_wait(&fin_turno_actual);

        // Esperar a que todos los mesoneros hayan salido de la caja
        sem_wait(&mesoneros_fuera_de_caja);

        // Cambiar de turno
        cambiar_turno();

        // Notificar a los mesoneros del inicio del nuevo turno
        sem_post(&inicio_nuevo_turno);

        // Liberar el semáforo que garantiza que todos los mesoneros han salido de la caja
        sem_post(&mesoneros_fuera_de_caja);
    }
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    // Inicializar semáforos
    sem_init(&clientes_en_cola, 0, 0);
    sem_init(&mesoneros_disponibles, 0, 2);
    sem_init(&max_mesoneros_en_caja, 0, 2);
    sem_init(&mesonero_en_caja_cola, 0, 0);
    sem_init(&mesonero_en_descanso, 0, 0);
    sem_init(&mesoneros_fuera_de_caja, 0, 2);
    sem_init(&caja_disponible, 0, 1);
    sem_init(&mutex_caja, 0, 1);
    sem_init(&fin_turno_actual, 0, 0);
    sem_init(&inicio_nuevo_turno, 0, 0);
    sem_init(&descanso_mesonero, 0, 0);

    // Crear los hilos para los mesoneros
    pthread_t mesoneros[2];
    for (int i = 0; i < 2; i++)
    {
        pthread_create(&mesoneros[i], NULL, ciclo_de_vida_mesonero, NULL);
    }

    // Crear el hilo para los supervisores
    pthread_t supervisor;
    pthread_create(&supervisor, NULL, ciclo_de_vida_supervisor, NULL);

    // Crear los hilos para los clientes
    pthread_t clientes[10];
    for (int i = 0; i < 10; i++)
    {
        pthread_create(&clientes[i], NULL, ciclo_de_vida_cliente, NULL);
    }

    // Crear el hilo para el ciclo de vida del turno
    pthread_t turno;
    pthread_create(&turno, NULL, ciclo_de_vida_turno, NULL);

    // Esperar a que todos los hilos hayan terminado
    for (int i = 0; i < 2; i++)
    {
        pthread_join(mesoneros[i], NULL);
    }
    for (int i = 0; i < 10; i++)
    {
        pthread_join(clientes[i], NULL);
    }
    pthread_join(supervisor, NULL);
    pthread_join(turno, NULL);

    // Destruir semáforos
    sem_destroy(&clientes_en_cola);
    sem_destroy(&mesoneros_disponibles);
    sem_destroy(&max_mesoneros_en_caja);
    sem_destroy(&mesonero_en_caja_cola);
    sem_destroy(&mesonero_en_descanso);
    sem_destroy(&mesoneros_fuera_de_caja);
    sem_destroy(&caja_disponible);
    sem_destroy(&mutex_caja);
    sem_destroy(&fin_turno_actual);
    sem_destroy(&inicio_nuevo_turno);
    sem_destroy(&descanso_mesonero);

    return 0;
}