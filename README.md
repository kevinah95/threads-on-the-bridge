# Tarea 1: Sincronización de *threads*

## Instrucciones de ejecución
1. El programa se corre ejecutando la instrucción ./main seguida de 2 números enteros que indican los vehículos que circularán por el puente. El primer número representa los carros en dirección oeste, el segundo número los carros en dirección este. Ejemplo:
```
./main 2 3
```
 - En caso de no indicar el número de carros para ninguna de las direcciones se creará un número de carros aleatorio entre 0 y 5. Si solo se indica una cantidad de carros se tomará como la cantidad de vehículos que circulan hacia el oeste; la cantidad de carros hacia el este será generada aleatoriamente entre 0 y 5.
 - Si se envía texto como parámetros en lugar de números enteros se asignará cero para ambas direcciones.
 - Si indican valores con decimales únicamente se tomará la parte entera.

2. La salida de consola se lee de la siguiente manera:

-Quantity of cars heading west: será el número de carros que se dirigirán hacia el oeste.
-Quantity of cars heading east: será el número de carros que se dirigirán hacia el este.

Los carros se crean 1 por 1.

(+) Cars created (WEST) [->] west=1 east=0 thread=140302320432896. En donde:
 
(+) Cars created (WEST/): la dirección en la que se crea el nuevo carro.
west: será la cantidad de carros creados hasta el momento que se dirigirán hacia el oeste.
east: será la cantidad de carros creados hasta el momento que se dirigirán hacia el este.
thread: la identificación del hilo que se está ejecutando.

(...)West/east car arrive to the bridge [->] west=1 east=2 west_pass/east_pass=1 thread=140302320432896
Este mensaje indica la cantidad de carros esperando pasar hacia el este/oeste. En donde:
(...)West car arrive to the bridge: indica que un carro que desea pasar el puente hacia el oeste ha llegado.
(...)East car arrive to the bridge: indica que un carro que desea pasar el puente hacia el este ha llegado.
west: indica la cantidad de carros que han llegado para pasar el puente hacia el oeste.
east: indica la cantidad de carros que han llegado para pasar el puente hacia el este.
west_pass: indica que de los carros que han llegado, la cantidad de carros que logra pasar hacia el oeste. 
east_pass: indica que de los carros que han llegado, la cantidad de carros que logra pasar hacia el este. 

(...)West/east start to pass [->] west_pass=1  thread=140302320432896. En donde:
(...)West start to pass: indica que hay un carro pasando hacia el oeste.
(...)East start to pass: indica que hay un carro pasando hacia el este.

(--)West/east car leave [->] direction=WEST east=2 west=1 thread=140302320432896. En donde:
(--)West car leave: indica que la dirección del carro que está pasando el puente en ese momento es oeste.
(--)East car leave: indica que la dirección del carro que está pasando el puente en ese momento es este.
east: cantidad de carros que deben pasar hacia el este.
west: cantidad de carros que deben pasar hacia el oeste.


El símbolo [<-] -- significa este.
El símbolo [->] -- significa oeste.












