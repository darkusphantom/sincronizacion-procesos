#ifndef TABLE_H
#define TABLE_H

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N_TABLE 20

sem_t sem_table[N_TABLE];

int ids_table[N_TABLE];

#endif