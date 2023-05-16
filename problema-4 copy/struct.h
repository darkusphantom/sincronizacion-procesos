#ifndef STRUCT_H
#define STRUCT_H

/**
 * @brief Definición de la estructura Order
 *
 * @param id_table Identificador de la mesa donde se tomo el pedido.
 * @param waiter_id Identificador del mesonero que atendió el pedido
 */
typedef struct
{
    int id_table;
    int id_waiter;
} Order;

/**
 * @brief Estructura que representa a una mesa
 *
 * @param id Identificador de la mesa.
 * @param disponible. Indica si el mesonero está en descanso (1 = Sí, 0 = No).
 */
typedef struct
{
    int id;
    int disponible;
} Table;

/**
 * @brief Estructura que representa a una planilla
 *
 * @param pedidos_contabilizados Número total de pedidos atendidos.
 */
typedef struct
{
    int pedidos_contabilizados;
    int mayor_numero_pedidos;
    int total_pedidos_por_mesoneros;
    int total_cobros_por_mesoneros;
    int mesonero_del_dia;
} Spreadsheet;

/**
 * @brief Estructura que representa a un mesonero.
 *
 * @param id Identificador del mesonero.
 * @param id_mesa Identificador de la mesa a la que esta atendiendo.
 * @param en_caja Indica si el mesonero está en el área de caja (1 = Sí, 0 = No).
 * @param en_descanso Indica si el mesonero está en descanso (1 = Sí, 0 = No).
 * @param pedidos_atendidos Número total de pedidos atendidos.
 * @param cobros_en_caja Número de pedidos cobrados.
 * @param total_pedidos_atendidos Número total de pedidos atendidos.
 * @param total_descansos Número de descansos.
 */
typedef struct
{
    int id;
    int id_mesa;
    int en_caja;
    int en_descanso;
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
    Spreadsheet spreadsheet; // Número total de pedidos contabilizados

} Supervisor;

#endif // DEBUG