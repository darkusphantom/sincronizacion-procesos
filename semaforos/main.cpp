#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define N_ROTATIONS 10000

static int counter = 0;

static void *increment(void *arg)
{
    for (int i = 0; i < N_ROTATIONS; i++)
    {
        counter += i;
        printf("Valor del contador: %i \n", counter);
        sleep(1);
    }
};

static void *decrement(void *arg)
{
    for (int i = 0; i < N_ROTATIONS; i++)
    {
        counter -= i;
        printf("Valor del contador: %i \n", counter);
        sleep(2);
    }
};

int main()
{
    
    return 0;
}
