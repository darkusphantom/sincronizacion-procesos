#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "const.h"
#include "waiter.h"
#include "sem.h"

// int ORDER_BY_SUPERVISOR = 0;
// int ORDER_BY_WAITER = 0;
// int REST_BY_WAITER = 0;
// int ORDER_BY_TURN = 0;
int TOTAL_ORDERS = 0;
int MAX_TOTAL_ORDERS = 0;
int MAX_TOTAL_CLAIMS = 0;

pthread_t order_screen[MAX_SCREEN];
pthread_t waiter[MAX_WAITER];

/**
 * @brief Function to create the thread of waiters, cash_box and supervisors.
 *
 */
// void create_threads()
// {
//     // create_waiter(waiter);
// }

int main()
{
    sem_init(&sem_cash_register, 0, MAX_CASH_REGISTER);
    sem_init(&sem_table, 0, MAX_TABLE);
    sem_init(&sem_order_screen, 0, 0);
    sem_init(&sem_ticket_office, 0, 0);
    sem_init(&sem_waiter_rest, 0, 0);

    return 0;
}