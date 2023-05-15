#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
// #include "global.h"
#include "const.h"
#include "struct.h"

sem_t mutex;       // Semáforo binario para controlar el acceso a las variables compartidas
sem_t mesonero;    // Semáforo binario para que el mesonero espere a que llegue un cliente
sem_t cliente;     // Semáforo binario para que el cliente espere a que el mesonero lo atienda
sem_t taquilla;    // Semáforo binario para la taquilla
sem_t planilla;    // Semaforo para Planilla del supervisor para registrar cobros
sem_t caja;        // Semaforo para Caja de cobro
sem_t waiter_rest; // Semaforo para el descanso del mesonero

int mesas_disponibles = MAX_TABLE; // Número de mesas disponibles
int caja_disponible = 0;           // Bandera que indica si la caja está disponible

Waiter waiters[MAX_WAITER];

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
          // Atender al cliente en la mesa
          printf("El mesonero está atendiendo al cliente en la mesa\n");
          printf("El mesonero terminó de atender al cliente en la mesa\n");
          // Registrar pedido en la pantalla
          // ...
          printf("El mesonero esta registrando el pedido en pantalla\n");
          // Esperar a que el pedido esté listo en la taquilla
          sem_wait(&taquilla);
          printf("Mesonero %d llevando pedido a la mesa\n", waiter_actual);
          // Llevar el pedido a la mesa
          // ...
          // Cobrar en la caja
          sem_wait(&caja);
          caja_disponible = 1;
          while (caja_disponible)
          {
               if (waiters[waiter_actual].cobros_en_caja < 10)
               {
                    printf("Mesonero %d cobrando en caja\n", waiter_actual);
                    sleep(2);
                    // Cobrar en la caja
                    // ...
                    // Incrementar el contador de pedidos atendidos y cobros en caja
                    waiters[waiter_actual].pedidos_atendidos++;
                    waiters[waiter_actual].cobros_en_caja++;
                    //      // Avisar a un supervisor cada 10 pedidos
                    //      if (cobros_en_caja_mesonero % 10 == 0)
                    //      {
                    //           sem_wait(&planilla);
                    //           printf("Mesonero %d avisa al supervisor\n", waiter_id);
                    //           // Avisar al supervisor
                    //           // ...
                    //           sem_post(&planilla);
                    //           // Incrementar el contador de descansos
                    //           descansos_mesonero_actual++;
                    //      }
               }
               else
               {
                    int total_pedidos_mesonero = waiters[waiter_actual].pedidos_atendidos++;
                    waiters[waiter_actual].pedidos_atendidos = 0;

                    int cobros_caja = waiters[waiter_actual].cobros_en_caja++;
                    waiters[waiter_actual].cobros_en_caja = 0;

                    waiters[waiter_actual].total_pedidos_atendidos += total_pedidos_mesonero;
                    int total_pedidos_turno = waiters[waiter_actual].total_pedidos_atendidos;

                    // Muestra el total de pedidos y cobros en la ronda antes del descanso
                    printf("Pedidos atendidos del mesonero %d: %d\n", waiter_actual, total_pedidos_mesonero);
                    printf("Cobros en caja del mesonero %d: %d\n", waiter_actual, cobros_caja);
                    printf("Total de pedidos atendidos en este turno: %d\n", total_pedidos_turno);

                    // Si ya se han cobrado 10 pedidos, el mesonero debe tomar un descanso
                    waiter_go_rest(waiter_actual);
               }
               // Liberar la caja
               caja_disponible = 0;
               sem_post(&caja);
          }
          // Actualizar estadísticas de descansos
          // if (descansos_mesonero_actual > mesonero_stats[mesonero_id].total_descansos)
          // {
          //      mesonero_stats[mesonero_id].total_descansos = descansos_mesonero_actual;
          // }
          // Descansar si es necesario
          // if (descansos_mesonero_actual == 2)
          // {
          //      printf("Mesonero %d se va a descansar\n", waiter_id);
          //      // Ir a descansar
          //      // ...
          //      descansos_mesonero_actual = 0;
          // }
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

// Inicializar los semáforos
void semaphore_init()
{
     sem_init(&mutex, 0, 1);
     sem_init(&mesonero, 0, 0);
     sem_init(&cliente, 0, 0);
}

// Destruir los semáforos
void semaphore_destroy()
{
     sem_destroy(&mutex);
     sem_destroy(&mesonero);
     sem_destroy(&cliente);
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

int main()
{
     pthread_t mesonero_thread, cliente_thread;
     semaphore_init();

     waiter_init();

     // Crear los hilos del mesonero y los clientes
     pthread_create(&mesonero_thread, NULL, mesonero_func, &waiters[0].id);
     pthread_create(&cliente_thread, NULL, cliente_func, NULL);

     // Esperar a que los hilos terminen
     pthread_join(mesonero_thread, NULL);
     pthread_join(cliente_thread, NULL);

     semaphore_destroy();

     return 0;
}