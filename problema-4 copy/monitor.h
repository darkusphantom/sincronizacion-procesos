#ifndef MONITOR_H
#define MONITOR_H

#include <stdio.h>

// void *monitor(void *args)
// {
//     int pedidos_turno_dia = 0, pedidos_turno_noche = 0;
//     int cobros_turno_dia = 0, cobros_turno_noche = 0;
//     mesonero_t *mesonero_mas_pedidos = NULL;
//     mesonero_t *mesonero_mas_cobros = NULL;

//     while (1)
//     {
//         // Leer datos de las estructuras compartidas por otros hilos
//         for (int i = 0; i < NUM_MESONEROS; i++)
//         {
//             mesonero_t *m = &mesoneros[i];
//             if (m->turno == DIA)
//             {
//                 pedidos_turno_dia += m->pedidos_tomados;
//                 cobros_turno_dia += m->cobros_realizados;
//                 if (mesonero_mas_pedidos == NULL ||
//                     m->pedidos_tomados > mesonero_mas_pedidos->pedidos_tomados)
//                 {
//                     mesonero_mas_pedidos = m;
//                 }
//                 if (mesonero_mas_cobros == NULL ||
//                     m->cobros_realizados > mesonero_mas_cobros->cobros_realizados)
//                 {
//                     mesonero_mas_cobros = m;
//                 }
//             }
//             else
//             {
//                 // Similar para turno de noche
//             }
//             printf("Mesonero %d - Pedidos: %d, Cobros: %d, Descansos: %d\n",
//                    m->id, m->pedidos_tomados, m->cobros_realizados, m->descansos_tomados);
//         }

//         // Mostrar resultados
//         printf("Turno dia - Pedidos: %d, Cobros: %d\n", pedidos_turno_dia, cobros_turno_dia);
//         printf("Turno noche - Pedidos: %d, Cobros: %d\n", pedidos_turno_noche, cobros_turno_noche);
//         printf("Mesonero con más pedidos (dia): %d - %d pedidos\n",
//                mesonero_mas_pedidos->id, mesonero_mas_pedidos->pedidos_tomados);
//         printf("Mesonero con más cobros (dia): %d - %d cobros\n",
//                mesonero_mas_cobros->id, mesonero_mas_cobros->cobros_realizados);

//         ... // Esperar un tiempo y  repetir
//     }
// }

#endif // !MONITOR_H