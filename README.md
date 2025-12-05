## tarea 4 - estructura de datos y algoritmos


grupo: [Grupo 4]
integrantes:
- [Gabriel]
- [Luis]
- [nombre 3]
- [nombre 4]





# problema del vendedor viajante (tsp)

## descripcion

implementacion en c del problema del vendedor viajante (travelling salesman problem) que encuentra la ruta mas corta para visitar todas las ciudades exactamente una vez y regresar al origen.

esta version maneja grafos no completos, verificando primero si existe un circuito hamiltoniano viable antes de calcular la ruta optima.

## caracteristicas

- representacion de grafos mediante matriz de adyacencia
- verificacion de existencia de circuito hamiltoniano
- calculo de ruta de costo minimo usando backtracking
- manejo dinamico de memoria sin variables globales
- soporte para grafos de 4 a 9 nodos

## estructura del proyecto

```
.
├── graph.h          - definicion del tad grafo
├── graph.c          - implementacion del grafo
├── tsp.h            - definicion de funciones tsp
├── tsp.c            - algoritmos de backtracking para tsp
├── commands.h       - definicion de procesador de comandos
├── commands.c       - implementacion de comandos interactivos
├── main.c           - programa principal
├── Makefile         - compilacion del proyecto
└── README.md        - este archivo
```

## compilacion

```bash
make
```

para limpiar archivos objeto y ejecutable:

```bash
make clean
```

## uso

ejecutar el programa:

```bash
./pvv
```

### comandos disponibles

- `start <n>` - crea un grafo con n nodos (4-9)
- `read <archivo>` - carga enlaces desde archivo y calcula ruta optima
- `graph` - muestra la matriz de adyacencia actual
- `exit` - libera memoria y sale del programa

### formato de archivo de entrada

cada linea debe contener un enlace con el formato:

```
<origen> <destino> <costo>
```

ejemplo (ruta.txt):
```
a b 15
a c 10
b c 25
b e 15
c d 20
c e 16
d e 30
```

- los nodos se identifican con letras (a, b, c, etc.)
- el costo debe ser un numero entero positivo
- las rutas son bidireccionales

## ejemplos de ejecucion

### caso 1: no existe ruta valida

```
>pvv start 5
grafo creado con 5 nodos
>pvv read ruta1.txt
agregando enlaces desde archivo.
verificando que existe una ruta.
no existe un camino que recorra todas las ciudades y regrese a la ciudad de origen.
>pvv exit
limpiando cache y saliendo ...
```

### caso 2: ruta valida encontrada

```
>pvv start 5
grafo creado con 5 nodos
>pvv read ruta2.txt
agregando enlaces desde archivo.
verificando que existe una ruta.
existe un camino que recorre todas las ciudades y regresa a la ciudad de origen.
ruta a seguir: a b e c d a
costo total minimo: 76
>pvv exit
limpiando cache y saliendo ...
```

## algoritmo

### verificacion de circuito hamiltoniano

utiliza backtracking para determinar si existe al menos un camino que:
- comienza en el nodo a (indice 0)
- visita todos los nodos exactamente una vez
- regresa al nodo inicial

### calculo de ruta optima

si existe un circuito valido, explora todas las permutaciones posibles mediante backtracking con poda:
- genera todas las rutas hamiltonianas
- calcula el costo total de cada ruta
- mantiene la ruta de costo minimo
- aplica poda cuando el costo parcial supera el minimo actual

## gestion de memoria

- asignacion dinamica de matrices y arreglos
- liberacion apropiada de memoria en todas las rutas de ejecucion
- manejo de errores en asignaciones fallidas
- sin fugas de memoria (verificable con valgrind)

## limitaciones

- maximo 9 nodos (definido en `PVV_MAX_NODOS`)
- complejidad exponencial: o(n!) para calcular ruta optima
- enfoque de fuerza bruta (adecuado para n <= 9)
