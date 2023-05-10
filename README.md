# sincronizacion-procesos
Manejo de la concurrencia entre procesos, utilizando herramientas para la sincronización y comunicación, aplicadas en la resolución de problemas en Sistemas Operativos
# sincronizacion-procesos

Manejo de la concurrencia entre procesos, utilizando herramientas para la sincronización y comunicación, aplicadas en la resolución de problemas en Sistemas Operativos

## Problema

En la cafetería CHARLOTTIX recientemente inaugurada, están implementando un sistema automatizado para agilizar sus procesos. Esta cafetería dispone de (04) pantallas de registro de pedido, (01) caja de cobro, (10) mesoneros por cada turno (diurno y nocturno), (20) mesas, (02) supervisores y (01) taquilla donde se anuncia el número de pedido que está listo para ser llevado a la mesa.

La dinámica del local puede explicarse de la siguiente forma:
- Los mesoneros toman el pedido en la mesa, registran el pedido en la pantalla, llevan el pedido a las mesas cuando está listo en la taquilla y cobran en caja a los clientes. Tienen orden de no dejar la caja sola, por lo que para poder ir a elaborar el pedido debe haber otro vendedor en el área de la caja.
- Por razones de espacio no puede haber más de (02) mesoneros en el área de caja a la vez, de los cuales solo uno de ellos puede estar atendiendo.
- Los mesoneros solo toman pedidos cuando no están en la caja.
- Los mesoneros deben atender a los clientes lo antes posible, no se debe hacer esperar a un cliente si la caja esta libre.
- Los mesoneros avisarán a cualquiera de los supervisores cada 10 pedidos cobrados por ese mesonero, para tomar un descanso.
- Los supervisores estarán esperando ser avisados para llenar la planilla.
- El primer supervisor libre recibirá el número de mesonero y con ese dato llenará la planilla.
- Los supervisores estarán atentos para realizar el cambio de turno de los mesoneros realizando un receso por un tiempo determinado para el cambio de guardia.

Se requiere contabilizar:
a) Número total de pedidos contabilizados por cada supervisor, en cada turno.
b) Turno con mayor número de pedidos contabilizados.
c) Número total de pedidos por mesonero.
d) Número de cobros en caja por mesonero.
e) Número de descansos por mesonero.
f) Mesonero del turno del día: máximo número de pedidos y máximo número de cobros
