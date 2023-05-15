#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
// #include "global.h"
#include "const.h"
#include "struct.h"

sem_t mutex;         // Semáforo binario para controlar el acceso a las variables compartidas
sem_t mutex_pedidos; //
sem_t mesonero;      // Semáforo binario para que el mesonero espere a que llegue un cliente
sem_t cliente;       // Semáforo binario para que el cliente espere a que el mesonero lo atienda
sem_t taquilla;      // Semáforo binario para la taquilla
sem_t planilla;      // Semaforo para Planilla del supervisor para registrar cobros
sem_t caja;          // Semaforo para Caja de cobro
sem_t waiter_rest;   // Semaforo para el descanso del mesonero

int mesas_disponibles = MAX_TABLE; // Número de mesas disponibles
int caja_disponible = 0;           // Bandera que indica si la caja está disponible

Waiter waiters[MAX_WAITER];
Supervisor supervisor[MAX_SUPERVISOR];

/**
 * @brief Funcion para devolver al mesonero al trabajo
 *
 * @param id Identificador del mesonero.
 */
void waiter_go_back_work(int id)
{
     waiters[id].en_descanso = 0;
     printf("El mesero %d volvió del descanso\n", id);
}

/**
 * @brief Funcion para enviar a descansar a un mesonero
 *
 * @param id Identificador del mesonero.
 */
void waiter_go_rest(int id)
{
     printf("Mesonero %d se va a tomar un descanso\n", id);
     waiters[id].en_descanso = 1;
     waiters[id].total_descansos++;
     sleep(3);
}

// Atender al cliente en la mesa
void attend_client()
{
     // Esperar a que llegue un cliente
     sem_wait(&cliente);
     // Tomar el mutex para acceder a las variables compartidas
     sem_wait(&mutex);
     // Incrementar el número de mesas disponibles
     mesas_disponibles++;
     // Liberar el mutex
     sem_post(&mutex);
     // Despertar al cliente
     sem_post(&mesonero);

     printf("El mesonero está atendiendo al cliente en la mesa\n");
     sleep(1);
     printf("El mesonero terminó de atender al cliente en la mesa\n");
}

// Registrar pedido en la pantalla
void register_order()
{
     printf("El mesonero esta registrando el pedido en pantalla\n");
}

// Llevar el pedido a la mesa
void send_order_to_client(int id_waiter)
{
     // Esperar a que el pedido esté listo en la taquilla
     // sem_wait(&taquilla);
     printf("Mesonero %d llevando pedido a la mesa\n", id_waiter);
     //...
     // sem_post(&taquilla);
}

// Muestra el resultado del turno actual
void waiter_show_results(int id)
{
     int total_pedidos_mesonero = waiters[id].pedidos_atendidos++;
     waiters[id].pedidos_atendidos = 0;

     int cobros_caja = waiters[id].cobros_en_caja++;
     waiters[id].cobros_en_caja = 0;

     waiters[id].total_pedidos_atendidos += total_pedidos_mesonero;
     int total_pedidos_turno = waiters[id].total_pedidos_atendidos;

     // Muestra el total de pedidos y cobros en la ronda antes del descanso
     printf("Pedidos atendidos del mesonero %d: %d\n", id, total_pedidos_mesonero);
     printf("Cobros en caja del mesonero %d: %d\n", id, cobros_caja);
     printf("Total de pedidos atendidos en este turno: %d\n", total_pedidos_turno);
}

void supervisor_saved_order(int id)
{
     // sem_wait(&planilla);
     printf("Mesonero %d avisa al supervisor\n", id);
     // Avisar al supervisor
     // ...
     // sem_post(&planilla);
}

// Cobrar en la caja
void cash_in(int id)
{
     int cobros_caja = waiters[id].cobros_en_caja;
     sem_wait(&caja);
     caja_disponible = 1;
     while (caja_disponible)
     {
          if (cobros_caja < MAX_ORDERS)
          {
               printf("Mesonero %d cobrando en caja\n", id);
               // Cobrar en la caja

               // Incrementar el contador de pedidos atendidos y cobros en caja
               waiters[id].pedidos_atendidos++;
               waiters[id].cobros_en_caja++;
               // Avisar a un supervisor cada 10 pedidos
               if (cobros_caja % MAX_ORDERS == 0)
               {
                    supervisor_saved_order(id);
                    waiters[id].total_descansos++;
               }
               sleep(2);
          }
          else
          {
               waiter_show_results(id);

               // Si ya se han cobrado 10 pedidos, el mesonero debe tomar un descanso
               waiter_go_rest(id);
          }
          // Liberar la caja
          caja_disponible = 0;
          sem_post(&caja);
     }
}

void *mesonero_func(void *arg)
{
     // waiter actual es id del mesonero
     int waiter_actual = *(int *)arg;
     // Indica si la caja esta disponible u ocupada (Disponible = 1, Ocupada = 0)
     int available_cash_box = 1;
     // Descanso actual del mesero
     int waiter_in_rest = waiters[waiter_actual].en_descanso;

     if (waiter_in_rest)
     {
          waiter_go_back_work(waiters[waiter_actual].id);
     }
     while (available_cash_box && !waiter_in_rest)
     {
          attend_client();
          register_order();
          send_order_to_client(waiter_actual);
          cash_in(waiter_actual);
     }

     return NULL;
}

void *cliente_func(void *arg)
{
     while (1)
     {
          // Tomar el mutex para acceder a las variables compartidas
          sem_wait(&mutex);
          if (mesas_disponibles > 0)
          {
               // Hay una mesa disponible
               mesas_disponibles--;
               // Liberar el mutex
               sem_post(&mutex);
               // Despertar al mesonero
               sem_post(&cliente);
               // Esperar a que el mesonero lo atienda
               sem_wait(&mesonero);
               // Sentarse en la mesa
               printf("El cliente se está sentando en la mesa\n");
               sleep(1);
               printf("El cliente se sentó en la mesa\n");
          }
          else
          {
               // No hay mesas disponibles
               // Liberar el mutex
               sem_post(&mutex);
               printf("El cliente se fue porque no había mesas disponibles\n");
          }
          // Esperar un tiempo aleatorio antes de llegar al siguiente cliente
          sleep(rand() % 5 + 1);
     }
     return NULL;
}

void *supervisor_func(void *arg)
{
     int supervisor_id = *(int *)arg;
     int turno_id = 0;
     while (1)
     {
          // sem_wait(&mutex_pedidos);
          // pedidos = supervisor[supervisor_id].spreadsheet.;
          // pedidos_supervisor[supervisor_id][turno_id][i] = 0;
          // sem_post(&mutex_pedidos);
          // sem_wait(&mutex_cobros);
          // cobros = cobros_mesonero[i];
          // cobros_mesonero[i] = 0;
          // sem_post(&mutex_cobros);
          // printf("Supervisor %d - Turno %d - Mesonero %d - Pedidos: %d - Cobros: %d\n", supervisor_id, turno_id, i, pedidos, cobros);
     }
     return NULL;
}

// Inicializar los semáforos
void semaphore_init()
{
     sem_init(&mutex, 0, 1);
     sem_init(&mesonero, 0, 0);
     sem_init(&cliente, 0, 0);
     // sem_init(&taquilla, 0, 0);
     // sem_init(&planilla, 0, 0);
     sem_init(&caja, 0, 1);
}

// Destruir los semáforos
void semaphore_destroy()
{
     sem_destroy(&mutex);
     sem_destroy(&mesonero);
     sem_destroy(&cliente);
     // sem_destroy(&taquilla);
     sem_destroy(&planilla);
     sem_destroy(&caja);
     // sem_destroy(&waiter_rest);
}

void waiter_init()
{
     for (int i = 0; i < MAX_WAITER; i++)
     {
          waiters[i].id = i;
          waiters[i].en_caja = 0;
          waiters[i].en_descanso = 0;
          waiters[i].pedidos_atendidos = 0;
          waiters[i].cobros_en_caja = 0;
          waiters[i].total_pedidos_atendidos = 0;
          waiters[i].total_descansos = 0;
     }
}

void supervisor_init()
{
     for (int i = 0; i < MAX_SUPERVISOR; i++)
     {
          supervisor[i].id = i;
          supervisor[i].spreadsheet.pedidos_contabilizados = 0;
          supervisor[i].spreadsheet.mayor_numero_pedidos = 0;
          supervisor[i].spreadsheet.mesonero_del_dia = 0;
          supervisor[i].spreadsheet.total_cobros_por_mesoneros = 0;
          supervisor[i].spreadsheet.total_pedidos_por_mesoneros = 0;
     }
}

int main()
{
     pthread_t mesonero_thread, cliente_thread, supervisor_thread;
     semaphore_init();

     waiter_init();
     supervisor_init();

     // Crear los hilos del mesonero y los clientes
     pthread_create(&mesonero_thread, NULL, mesonero_func, &waiters[0].id);
     pthread_create(&cliente_thread, NULL, cliente_func, NULL);
     pthread_create(&supervisor_thread, NULL, supervisor_func, &supervisor[0].id);

     // Esperar a que los hilos terminen
     pthread_join(mesonero_thread, NULL);
     pthread_join(cliente_thread, NULL);

     semaphore_destroy();

     return 0;
}