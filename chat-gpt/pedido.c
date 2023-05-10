#include <pthread.h>
#include <semaphore.h>

// Definición de la estructura Pedido
typedef struct {
    int numMesa; // Número de mesa del pedido
    char items[50]; // Lista de items del pedido
} Pedido;

// Definición de variables globales
Pedido pedidos[10]; // Array con los pedidos registrados
pthread_t hiloAnuncio; // Hilo correspondiente al anuncio de pedidos
sem_t semaforoAnuncio; // Semáforo correspondiente al anuncio de pedidos

// Función que representa el hilo de anuncio de pedidos
void* hiloAnuncioFuncion(void* arg) {
    while (1) {
        // Esperar hasta que haya un pedido registrado
        sem_wait(&semaforoAnuncio);
        
        // Anunciar el pedido registrado
        Pedido pedido = pedidos[0];
        printf("Pedido de la mesa %d: %s\n", pedido.numMesa, pedido.items);
        
        // Eliminar el pedido registrado
        for (int i = 0; i < 9; i++) {
            pedidos[i] = pedidos[i+1];
        }
        
        // Liberar el semáforo correspondiente al anuncio de pedidos
        sem_post(&semaforoAnuncio);
    }
    return NULL;
}

// Función que registra un nuevo pedido
void registrarPedido(int numMesa, char* items) {
    // Esperar a que haya una posición libre en el array de pedidos
    sem_wait(&semaforoAnuncio);
    
    // Registrar el nuevo pedido
    Pedido pedido = {numMesa, *items};
    pedidos[9] = pedido;
    
    // Liberar el semáforo correspondiente al anuncio de pedidos
    sem_post(&semaforoAnuncio);
}

// Función que anuncia el primer pedido registrado en el array de pedidos
void anunciarPedido() {
    sem_wait(&semaforoAnuncio);
    sem_post(&semaforoAnuncio);
}

// Función principal que inicializa el hilo correspondiente al anuncio de pedidos
int main() {
    // Inicializar el semáforo correspondiente al anuncio de pedidos
    sem_init(&semaforoAnuncio, 0, 0); // Inicializar el semáforo a 0 (sin pedidos)
    
    // Crear el hilo correspondiente al anuncio de pedidos
    pthread_create(&hiloAnuncio, NULL, hiloAnuncioFuncion, NULL);
    
    // Esperar a que el hilo termine (esto no debería ocurrir nunca en este ejemplo)
    pthread_join(hiloAnuncio, NULL);
    
    return 0;
}