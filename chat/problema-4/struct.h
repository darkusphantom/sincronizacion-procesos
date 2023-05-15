#ifndef STRUCT_H
#define STRUCT_H

// Definición de la estructura Order
typedef struct
{
    int id_table;  // Número de mesa del pedido
    int waiter_id; // ID del mesonero que atendió el pedido
} Order;

// Definición de la estructura Waiter
typedef struct
{
    int id;
    int in_cashbox;     // Indica si el mesonero está en el área de caja (1 = Sí, 0 = No)
    int total_orders;   // Número total de pedidos atendidos
    int orders_claimed; // Número de pedidos cobrado
    int total_rests;          // Número de descansos
} Waiter;

// Definición de la estructura Supervisor
typedef struct
{
    int id;
    int total_orders; // Número total de pedidos contabilizados
} Supervisor;
#endif // DEBUG