#include <pthread.h>
#include <semaphore.h>

// Definición de la estructura Cliente
typedef struct {
    int numMesa; // Número de mesa del cliente
    float total; // Total a pagar por el cliente
} Cliente;

// Definición de variables globales
Cliente clientes[10]; // Array con los clientes que están en la caja
pthread_t hiloCaja; // Hilo correspondiente a la caja
sem_t semaforoCaja; // Semáforo correspondiente a la caja

// Función que representa el hilo de atención al cliente en la caja
void* hiloCajaFuncion(void* arg) {
    while (1) {
        // Esperar hasta que haya un cliente en la caja
        sem_wait(&semaforoCaja);
        
        // Atender al cliente en la caja
        Cliente cliente = clientes[0];
        printf("Atendiendo al cliente de la mesa %d. Total a pagar: $%.2f\n", cliente.numMesa, cliente.total);
        
        // Eliminar al cliente de la caja
        for (int i = 0; i < 9; i++) {
            clientes[i] = clientes[i+1];
        }
        sem_post(&semaforoCaja);
    }
    return NULL;
}

// Función que coloca un cliente en la caja
void colocarClienteEnCaja(int numMesa, float total) {
    // Esperar a que la caja esté disponible
    sem_wait(&semaforoCaja);
    
    // Colocar al cliente en la caja
    Cliente cliente = {numMesa, total};
    clientes[9] = cliente;
    
    // Liberar la caja para atender al cliente
    sem_post(&semaforoCaja);
}

// Función principal que inicializa la caja y el hilo correspondiente
int main() {
    // Inicializar el semáforo correspondiente a la caja
    sem_init(&semaforoCaja, 0, 0); // Inicializar el semáforo a 0 (sin clientes)
    
    // Crear el hilo correspondiente a la caja
    pthread_create(&hiloCaja, NULL, hiloCajaFuncion, NULL);
    
    // Esperar a que el hilo termine (esto no debería ocurrir nunca en este ejemplo)
    pthread_join(hiloCaja, NULL);
    
    return 0;
}