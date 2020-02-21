# Tarea 1: Sincronización de *threads*

## Instrucciones de ejecución
1. El programa se corre ejecutando la instrucción ./main seguida de 2 números enteros que indican los vehículos que circularán por el puente. El primer número representa los carros en dirección oeste, el segundo número los carros en dirección este. Ejemplo:
```
./main 2 3
```
 - En caso de no indicar el número de carros para ninguna de las direcciones se creará un número de carros aleatorio entre 0 y 5. Si solo se indica una cantidad de carros se tomará como la cantidad de vehículos que circulan hacia el oeste; la cantidad de carros hacia el este será generada aleatoriamente entre 0 y 5.
 - Si se envía texto como parámetros en lugar de números enteros se asignará cero para ambas direcciones.
 - Si indican valores con decimales únicamente se tomará la parte entera.
