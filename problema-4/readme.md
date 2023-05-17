Actua como un experto en C y programación concurrente y aplicando semaforos. Además, revisas de inicio a fin para que el codigo funcione correctamente y apliques al pie de la letra el contexto o lo que se te pide

El programa dispone de (04) pantallas de registro de pedido, (01) caja de cobro, (10) mesoneros por cada turno (diurno y nocturno), (20) mesas, (02) supervisores y (01) taquilla donde se anuncia el número de pedido que está listo para ser llevado a la mesa.

El programa debe cumplir:

- Los mesoneros toman el pedido en la mesa, registran el pedido en la pantalla, llevan el pedido a las mesas cuando está listo en la taquilla y cobran en caja a los clientes.
- Los mesonero no debe dejar la caja sola, debe haber otro mesonero para poder ir a tomar el pedido.
- No puede haber más de (02) mesoneros en el área de caja a la vez. Uno de ellos solo puede estar atendiendo.
- Los mesoneros solo toman pedidos cuando no están en la caja.
- Los mesoneros deben atender a los clientes lo antes posible, no se debe hacer esperar a un cliente si la caja esta libre.
- Los mesoneros avisarán a cualquiera de los supervisores cada 10 pedidos cobrados por ese mesonero, para tomar un descanso.
- Los supervisores estarán esperando ser avisados para llenar la planilla.
- El primer supervisor libre recibirá el número de mesonero y con ese dato llenará la planilla.
- Los supervisores estarán atentos para realizar el cambio de turno de los mesoneros realizando un receso por un tiempo determinado para el cambio de guardia.

Se requiere contabilizar y mostrar en pantalla:
a) Número total de pedidos contabilizados por cada supervisor, en cada turno.
b) Turno con mayor número de pedidos contabilizados.
c) Número total de pedidos por mesonero.
d) Número de cobros en caja por mesonero.
e) Número de descansos por mesonero.
f) Mesonero del turno del día: máximo número de pedidos y máximo número de cobros

Necesito a que me ayudes a cómo resolver y abarcar este problema, enfocandose solo en los puntos mencionados

---

---

Este problema puede ser resuelto utilizando semáforos y la implementación del algoritmo del problema del barbero dormilón. Se pueden utilizar varios semáforos para controlar el acceso a los recursos compartidos, como las pantallas de registro de pedido, la caja de cobro, las mesas, los supervisores y la taquilla. También se pueden utilizar semáforos para controlar el número de mesoneros en la caja y para asegurarse de que los mesoneros solo tomen pedidos cuando no están en la caja.

Para implementar el algoritmo del problema del barbero dormilón, se puede utilizar un semáforo contador para controlar el número de clientes que están esperando. Cuando un cliente llega, se intenta tomar un asiento y esperar a que el barbero esté disponible. Pero si no hay asientos disponibles, el cliente se va.

Para cumplir con los requisitos del problema, se pueden utilizar los siguientes semáforos:

semáforo de mesoneros: contador de mesoneros disponibles.
semáforo de caja: contador de mesoneros en la caja.
semáforo de clientes: contador de clientes esperando.
semáforo de supervisores: contador de supervisores disponibles.

Además, se pueden utilizar variables compartidas para registrar los datos necesarios:

variable total_pedidos_supervisor: número total de pedidos contabilizados por cada supervisor.
variable pedidos_turno: número total de pedidos contabilizados por turno.
variable max_pedidos_mesonero: máximo número de pedidos por mesonero.
variable max_cobros_mesonero: máximo número de cobros por mesonero.
variable descansos_mesonero: número de descansos por mesonero.
variable mesonero_max_pedidos: mesonero con el máximo número de pedidos del turno diurno.

La implementación del programa podría seguir los siguientes pasos:

Inicializar los semáforos y variables compartidas.
- Crear los hilos correspondientes a los mesoneros, supervisores y barbero.
- La función del mesonero espera a que haya mesoneros disponibles y toma el pedido en la mesa. Luego registra el pedido en la pantalla, lo lleva a la taquilla y cobra en la caja.
- La función del supervisor espera a que un mesonero le avise que ha contabilizado 10 pedidos, y luego registra los datos necesarios.
- La función del barbero espera a que haya clientes esperando y luego los atiende. Si no hay clientes, el barbero duerme.
- El programa muestra en pantalla los datos contabilizados.
- Para asegurarse de que los mesoneros no dejen la caja sola y de que no haya más de dos mesoneros en la caja, se pueden utilizar semáforos para controlar el acceso a la caja. Para asegurarse de que los mesoneros no tomen pedidos cuando están en la caja, se puede utilizar un semáforo para controlar el acceso a los recursos compartidos.

Para asegurarse de que los mesoneros atiendan a los clientes lo antes posible, se pueden utilizar semáforos para controlar el acceso a los recursos compartidos y para asegurarse de que los clientes sean atendidos en el orden en que llegan.

Para asegurarse de que los supervisores estén atentos para realizar el cambio de turno de los mesoneros, se pueden utilizar semáforos para controlar el acceso a los recursos compartidos y para asegurarse de que los supervisores estén disponibles para realizar el cambio de turno.

Con la implementación adecuada de los semáforos y la utilización del algoritmo del problema del barbero dormilón, se puede resolver el problema y cumplir con todos los requisitos solicitados.
