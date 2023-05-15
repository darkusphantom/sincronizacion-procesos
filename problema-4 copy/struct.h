#ifndef STRUCT_H
#define STRUCT_H

// Definición de la estructura Order
typedef struct
{
    int id_table;  // Número de mesa del pedido
    int waiter_id; // ID del mesonero que atendió el pedido
} Order;

/**
 * @brief Estructura que representa a un mesonero.
 *
 * @param id Identificador del mesonero.
 * @param en_caja Indica si el mesonero está en el área de caja (1 = Sí, 0 = No).
 * @param pedidos_atendidos Número total de pedidos atendidos.
 * @param cobros_en_caja Número de pedidos cobrados.
 * @param total_pedidos_atendidos Número total de pedidos atendidos.
 * @param total_descansos Número de descansos.
 */
typedef struct
{
    int id;
    int en_caja;
    int pedidos_atendidos;
    int cobros_en_caja;
    int total_pedidos_atendidos;
    int total_descansos;
} Waiter;

/**
 * @brief Estructura que representa a un supervisor
 *
 * @param id Identificador del mesonero.
 * @param total_pedidos Número total de pedidos atendidos.
 */
typedef struct
{
    int id;
    int total_orders; // Número total de pedidos contabilizados
} Supervisor;
#endif // DEBUG