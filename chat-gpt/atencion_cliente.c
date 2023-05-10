#include <pthread.h>
#include <semaphore.h>

// Definición de la estructura Mesonero
typedef struct {
    int id; // Identificador del mesonero
} Mesonero;

// Definición de variables globales
Mesonero mesoneros[10]; // Array con los mesoneros disponibles
pthread_t hilosMesoneros[10]; // Array con los hilos correspondientes a cada mesonero
sem_t semaforosMesas[10]; // Array con los semáforos correspondientes a cada mesa

// Función que representa el hilo de atención al cliente en una mesa
void* hiloMesonero(void* arg) {
    int id = *(int*)arg;
    while (1) {
        // Esperar hasta que haya una mesa disponible
        sem_wait(&semaforosMesas[id]);
        
        // Atender al cliente en la mesa correspondiente
        printf("Atendiendo al cliente en la mesa %d\n", id);
        
        // Liberar la mesa para el siguiente cliente
        sem_post(&semaforosMesas[id]);
    }
    return NULL;
}

// Función que atiende a un cliente en una mesa
void atenderClienteEnMesa(int numMesa) {
    // Esperar a que la mesa correspondiente esté disponible
    sem_wait(&semaforosMesas[numMesa]);
    
    // Atender al cliente en la mesa correspondiente
    printf("Atendiendo al cliente en la mesa %d\n", numMesa);
    
    // Liberar la mesa para el siguiente cliente
    sem_post(&semaforosMesas[numMesa]);
}

// Función principal que inicializa los mesoneros y los hilos correspondientes
int main() {
    // Inicializar los mesoneros y los semáforos correspondientes a cada mesa
    for (int i = 0; i < 10; i++) {
        mesoneros[i].id = i;
        sem_init(&semaforosMesas[i], 0, 1); // Inicializar cada semáforo a 1 (mesa disponible)
    }
    
    // Crear los hilos correspondientes a cada mesonero
    for (int i = 0; i < 10; i++) {
        pthread_create(&hilosMesoneros[i], NULL, hiloMesonero, &mesoneros[i].id);
    }
    
    // Esperar a que los hilos terminen (esto no debería ocurrir nunca en este ejemplo)
    for (int i = 0; i < 10; i++) {
        pthread_join(hilosMesoneros[i], NULL);
    }
    
    return 0;
}