#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "const.h"
// #include "struct.h"

int ORDER_BY_SUPERVISOR = 0;
int ORDER_BY_WAITER = 0;
int REST_BY_WAITER = 0;
int ORDER_BY_TURN = 0;
int TOTAL_ORDERS = 0;
int MAX_TOTAL_ORDERS = 0;
int MAX_TOTAL_CLAIMS = 0;

pthread_t screen[MAX_SCREEN];


int main()
{

    return 0;
}