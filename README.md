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

- Cars created [->] west=1 east=0 car thread=140705463273216. En donde 
west: será la cantidad de carros creados que se dirigirán hacia el oeste.
east: será el número de carros creados que se dirigirán hacia el este.
thread: la identificación del hilo que se está ejecutando.

-East car waiting [<-] east=1 west/east=1 thread=140705454880512
east: cantidad de carros esperando pasar hacia el este/oeste.

-cars that have not passed the bridge yet westbound/eastbound=0
Al finalizar el programa estos contadores deberían de estar en 0 ya que corresponden a la cantidad de carros que faltan por pasar el puente hacia el oeste(westbound) o hacia el este(eastbound).


El símbolo [<-] -- significa que ingresa un carro/thread en dirección este.
La concatenación leave [<-] -- significa que el carro ya pasó por el puente.
La concatenación waiting [->] -- corresponde a los carroa que están esperando que el puente se desocupe para ir hacia el oeste.











