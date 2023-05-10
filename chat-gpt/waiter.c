// void *waiter_thread(void *arg)
// {
//     int id = *(int *)arg;
//     while (1)
//     {
//         int table_assigned = rand() % 20 + 1;
//         int id_table = table_assigned - 1;
//         sem_wait(&sem_table[id_table]);

//         Order order;
//         order.id_table = table_assigned;
//         printf("Mesonero %d tomando pedido de la mesa %d.\n", id, table_assigned);

//         // Register order
//         pthread_mutex_lock(&mutexOrders);
//         numOrders++;
//         pthread_mutex_unlock(&mutexOrders);

//         // Agregar el nuevo pedido al array de pedidos y anunciarlo
//         char items[50];
//         sprintf(items, "Pedido de la mesa %d", table_assigned);
//         registrarPedido(table_assigned, items);
//         anunciarPedido();

//         printf("Mesonero %d: Llevar pedido de la mesa %d a la cocina\n", id, table_assigned);
//         printf("Mesonero %d: Esperando a que el pedido esté listo en la taquilla\n", id);
//         // Simulamos una espera aleatoria para representar el tiempo que tarda en prepararse el pedido
//         // usleep(rand() % 1000);
//         sleep(3);

//         printf("Mesonero %d: Llevando pedido a la mesa %d\n", id, table_assigned);
//         sem_post(&sem_table[id_table]); // Liberar la mesa para el siguiente cliente

//         // Si no está en el área de caja, tomar un descanso después de cada 10 pedidos cobrados
//         if (!waiter[id].in_cashbox)
//         {
//             waiter[id].total_orders_claimed++;
//             printf("Mesonero %d: Total de pedidos reclamados: %d\n", id, waiter[id].total_orders_claimed);
//             if (waiter[id].total_orders_claimed % 10 == 0)
//             {
//                 printf("Mesonero %d: Tomando un descanso\n", id);
//                 // Simulamos una espera aleatoria para representar el tiempo de descanso
//                 // usleep(rand() % 1000);
//                 sleep(5);
//             }
//         }

//         sleep(3);
//     }

//     return NULL;
// }

// int main()
// {
//     // ...
    
//     // Iniciar el programa con un pedido ya registrado
//     registrarPedido(1, "Hamburguesa con queso");
//     anunciarPedido();
    
//     // Esperar a que el hilo termine (esto no debería ocurrir nunca en este ejemplo)
//     pthread_join(hiloAnuncio, NULL);

//     return 0;
// }