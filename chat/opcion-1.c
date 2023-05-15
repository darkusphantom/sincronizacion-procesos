// void *mesonero_func(void *arg) {
//     int mesonero_id = *(int *)arg;  // Identificador del mesonero
//     int pedidos_atendidos_mesonero = 0;  // Número de pedidos atendidos por el mesonero
//     int cobros_en_caja_mesonero = 0;  // Número de cobros realizados por el mesonero
//     int descansos_mesonero_actual = 0;  // Número de descansos que ha tomado el mesonero en este momento
//     int caja_disponible = 0;  // Bandera que indica si la caja está disponible
    
//     while (1) {
//         // Esperar a que un cliente llegue a la mesa
//         sem_wait(&cliente);
//         // Tomar el mutex para acceder a las variables compartidas
//         sem_wait(&mutex);
//         // Incrementar el número de mesas disponibles
//         mesas_disponibles++;
//         // Liberar el mutex
//         sem_post(&mutex);
//         // Despertar al mesonero para que atienda al cliente
//         sem_post(&mesonero);
//         // Atender al cliente en la mesa
//         printf("Mesonero %d tomando pedido en mesa\n", mesonero_id);
//         // Registrar pedido en la pantalla
//         // ...
//         // Esperar a que el pedido esté listo en la taquilla
//         sem_wait(&taquilla);
//         printf("Mesonero %d llevando pedido a la mesa\n", mesonero_id);
//         // Llevar el pedido a la mesa
//         // ...
//         // Cobrar en la caja
//         sem_wait(&caja);
//         caja_disponible = 1;
//         while (caja_disponible) {
//             if (cobros_en_caja_mesonero < 10) {
//                 printf("Mesonero %d cobrando en caja\n", mesonero_id);
//                 // Cobrar en la caja
//                 // ...
//                 // Incrementar el contador de pedidos atendidos y cobros en caja
//                 pedidos_atendidos_mesonero++;
//                 cobros_en_caja_mesonero++;
//                 total_pedidos_atendidos++;
//                 // Avisar a un supervisor cada 10 pedidos
//                 if (cobros_en_caja_mesonero % 10 == 0) {
//                     sem_wait(&planilla);
//                     printf("Mesonero %d avisa al supervisor\n", mesonero_id);
//                     // Avisar al supervisor
//                     // ...
//                     sem_post(&planilla);
//                     // Incrementar el contador de descansos
//                     descansos_mesonero_actual++;
//                 }
//                 // Liberar la caja
//                 caja_disponible = 0;
//                 sem_post(&caja);
//             } else {
//                 // Si ya se han cobrado 10 pedidos, el mesonero debe tomar un descanso
//                 printf("Mesonero %d se va a tomar un descanso\n", mesonero_id);
//                 // Ir a descansar
//                 // ...
//                 // Incrementar el contador de descansos
//                 descansos_mesonero_actual++;
//                 // Liberar la caja
//                 caja_disponible = 0;
//                 sem_post(&caja);
//             }
//         }
//         // Actualizar estadísticas de descansos
//         if (descansos_mesonero_actual > mesonero_stats[mesonero_id].total_descansos) {
//             mesonero_stats[mesonero_id].total_descansos = descansos_mesonero_actual;
//         }
//         // Descansar si es necesario
//         if (descansos_mesonero_actual == 2) {
//             printf("Mesonero %d se va a descansar\n", mesonero_id);
//             // Ir a descansar
//             // ...
//             descansos_mesonero_actual = 0;
//         }
//     }
//     return NULL;
// }