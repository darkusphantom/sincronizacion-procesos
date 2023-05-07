#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N_ROTATIONS 10000

static int counter = 0;
sem_t semaphore;

static void *increment(void *arg)
{
    for (int i = 0; i < N_ROTATIONS; i++)
    {
        sem_wait(&semaphore);
        counter += 1;
        printf("Valor del contador: %i \n", counter);
        sleep(2);
        sem_post(&semaphore);
    }
}

static void *decrement(void *arg)
{
    for (int i = 0; i < N_ROTATIONS; i++)
    {
        sem_wait(&semaphore);
        counter -= 1;
        printf("Valor del contador: %i \n", counter);
        sleep(2);
        sem_post(&semaphore);
    }
}

int main()
{
    sem_init(&semaphore, 0, 1);

    pthread_t p_increment, p_decrement;

    pthread_create(&p_increment, NULL, *increment, NULL);
    pthread_create(&p_decrement, NULL, *decrement, NULL);

    pthread_join(p_increment, NULL);
    pthread_join(p_decrement, NULL);

    return 0;
}
