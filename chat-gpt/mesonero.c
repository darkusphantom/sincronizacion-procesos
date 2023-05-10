#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

typedef struct {
    int id;
    bool enCaja;
    int numPedidosCobrados;
} Mesonero;

Mesonero mesoneros[10];
pthread_t hilosMesoneros[10];
sem_t semaforosMesas[10];
sem_t semaforoCaja;
pthread_mutex_t mutexCaja;

typedef struct {
    int numMesa;
} Pedido;

Pedido pedidos[10];
int numPedidos = 0;

void* hiloMesonero(void* arg) {
    int id = *(int*)arg;
    while (1) {
    
        sem_wait(&semaforosMesas[id]);
    
        Pedido pedido;
        pedido.numMesa = id;
        printf("Mesonero %d: Tomando pedido de la mesa %d\n", id, id);
    
        pthread_mutex_lock(&mutexCaja);
        pedidos[numPedidos] = pedido;
        numPedidos++;
        pthread_mutex_unlock(&mutexCaja);
        
        printf("Mesonero %d: Llevar pedido de la mesa %d a la cocina\n", id, id);
        printf("Mesonero %d: Esperando a que el pedido esté listo en la taquilla\n", id);
    
        usleep(rand() % 1000000);
    
        printf("Mesonero %d: Llevando pedido a la mesa %d\n", id, id);

        sem_post(&semaforosMesas[id]);

    
        if (!mesoneros[id].enCaja) {
            mesoneros[id].numPedidosCobrados++;
            if (mesoneros[id].numPedidosCobrados % 10 == 0) {
                printf("Mesonero %d: Tomando un descanso\n", id);
            
                usleep(rand() % 1000000);
            }
        }
    }
    return NULL;
}

void* hiloCaja(void* arg) {
    while (1) {
        sem_wait(&semaforoCaja);
    
        pthread_mutex_lock(&mutexCaja);
    
        int idMesonero = -1;
        for (int i = 0; i < 10; i++) {
            if (mesoneros[i].enCaja) {
                continue;
            }
            if (idMesonero == -1 || mesoneros[i].numPedidosCobrados < mesoneros[idMesonero].numPedidosCobrados) {
                idMesonero = i;
            }
        }
    
        mesoneros[idMesonero].enCaja = true;
    
        printf("Atendiendo a los clientes del mesonero %d\n", idMesonero);
        pthread_mutex_lock(&mutexCaja);
        for (int i = 0; i < numPedidos; i++) {
            if (pedidos[i].numMesa == idMesonero) {
                printf("Cobrando el pedido de la mesa %d\n", idMesonero);
            
                usleep(rand() % 1000000);
            }
        }
        pthread_mutex_unlock(&mutexCaja);
    
        mesoneros[idMesonero].enCaja = false;
    
        pthread_mutex_unlock(&mutexCaja);
    }
    return NULL;
}

int main() {
    for (int i = 0; i < 10; i++) {
        mesoneros[i].id = i;
        mesoneros[i].enCaja = false;
        mesoneros[i].numPedidosCobrados = 0;
        sem_init(&semaforosMesas[i], 0, 1);
    }

    sem_init(&semaforoCaja, 0, 0);
    pthread_mutex_init(&mutexCaja, NULL);

    for (int i = 0; i < 10; i++) {
        pthread_create(&hilosMesoneros[i], NULL, hiloMesonero, &mesoneros[i].id);
    }

    pthread_t hiloCaja;
    pthread_create(&hiloCaja, NULL, hiloCaja, NULL);

    for (int i = 0; i < 10; i++) {
        pthread_join(hilosMesoneros[i], NULL);
    }
    pthread_join(hiloCaja, NULL);

    return 0;
}