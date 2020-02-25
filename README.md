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

(+) Cars created (EAST) [->] west=1 east=0 car thread=140705463273216. En donde 
west: será la cantidad de carros creados actualmente que se dirigirán hacia el oeste.
east: será el número de carros creados que se dirigirán hacia el este.
thread: la identificación del hilo que se está ejecutando.

(...) East car waiting [<-] west_pass/east_pass=1 thread=140705454880512
west_pass/east_pass=: cantidad de carros esperando pasar hacia el este/oeste.

(...) East start to pass [<-] west_pass/east_pass=1 thread=140558106359552
west_pass/east_pass=: cantidad de carros esperando pasar hacia el oeste/este.
Este mensaje solo ocurre para el primer vehiculo en la fila, que esta esperando por usar el puente.

(...) East passing the bridge [<-] west_pass/east_pass=1 thread=140558106359552
west_pass/east_pass=: cantidad de carros pasando hacia el oeste/este.

(--) West/east car leave [->] direction=WEST/EAST east=2 west=1 thread=140302320432896. 
Indica: que la dirección del carro que pasó el puente en ese momento, la direccion de ve reflejada en las flechas y direction.

El símbolo [<-] -- significa este. El símbolo [->] -- significa oeste.

## Funcinamiento

Todos los puntos solicitados en las instrucciones funcionan correctamente.









