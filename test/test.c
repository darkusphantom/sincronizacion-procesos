#include <stdio.h>
#include <pthread.h>

void *computation(void *args)
{
    printf("Computation\n");
    return NULL;
}

void *add(void *add)
{
    long *add_num = (long *)add;

    printf("Add: %d", *add_num);

    return NULL;
}

int main()
{
    pthread_t thread;
    pthread_t thread_add, thread_add2;

    long value = 5;
    long value2 = 2;

    // pointer, pointer_initialitation, function, param_function
    pthread_create(&thread, NULL, *computation, NULL);
    pthread_create(&thread_add, NULL, *add, &value);
    pthread_create(&thread_add2, NULL, *add, &value2);

    pthread_join(thread, NULL);
    pthread_join(thread_add, NULL);
    pthread_join(thread_add2, NULL);

    return 0;
}