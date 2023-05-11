# Sincronizacion de Procesos

Proyecto universitario para manejo de la concurrencia entre procesos, utilizando herramientas para la sincronización y comunicación, aplicadas en la resolución de problemas en Sistemas Operativos.

## Condiciones

- El código deberá ser desarrollado en Lenguaje C, intradocumentado y la herramienta de sincronización entre procesos es semáforos.

## Organización de archivos

- problema-1:
  - main.c: Contiene el cuerpo y el codigo principal del proyecto
  - makefile: Contiene el compilador
- problema-4:
  - main.c: Contiene el cuerpo y el codigo principal del proyecto
  - makefile: Contiene el compilador
- .gitignore: Contiene los archivos que no se subiran al repositorio
- .vscode: Contiene la configuración de vs code con respecto al proyecto
- README.MD: Contiene los info base del proyecto como: Qué trata, los problemas a abordar, las condiciones y organización de archivos

## Problemas a resolver

### Problema 1

Durante el mantenimiento realizado en la autopista Maracay-La Victoria en un determinado tramo de la vía
por donde circulan carros y camiones, se están realizando diversas reparaciones y debido a que normalmente
hay mucho tráfico y falta de personal para controlarlo, se decidió generar una solución un poco más
sofisticada utilizando un software para realizar los controles.

Este tramo se encuentra dividido en subtramos (1, 2,3 y 4) que presentan diferentes situaciones:
• En el subtramo 1 pueden circular 4 vehículos de cualquier tipo al mismo tiempo.
• En el subtramo 2 pueden circular 2 autos o 1 camión al mismo tiempo por cuestiones de peso.
• En el subtramo 3 solo puede circular 1 vehículo de cualquier tipo ya que hay 1 solo carril habilitado
• En el subtramo 4 pueden circular 3 vehículos al mismo tiempo.
• Entre cada subtramo se cuenta con un hombrillo donde los vehículos pueden esperar si el siguiente
subtramo se encuentra congestionado.
• Cada hora transitan en promedio 1500 vehículos.
• Se debe tener en cuenta que la ruta es doble vía y por lo tanto los vehículos pueden circular desde el
subtramo 1 hasta el 4 (1->2->3->4) o desde el subtramo 4 hasta el 1 (4->3->2->1).

Se pide contabilizar:
a) Número de vehículos que han circulado en cada subtramo y en cada sentido por hora y al final del día, con
la finalidad de poder planificar mejor las reparaciones a futuro.
b) Tramo con circulación máxima al final del día.
c) Número máximo de vehículos y tiempo promedio que han tenido que esperar en el hombrillo en el día.

## Problema 4

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

---

Proyecto hecho por [Luis Rodriguez](github.com/darkusphantom) y Gustavo Herrera.

---
