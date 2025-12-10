# Tarea 4 — Estructura de datos y algoritmos

Grupo: Grupo 4  
Integrantes:
- Gabriel
- Luis
- Rodolfo
- Felipe

## Resumen
Repositorio con dos variantes de la implementación del Problema del Vendedor Viajante (TSP) en C. Mantener y compilar la variante deseada entrando en su carpeta y ejecutando make.

Nota de entrega: La versión a entregar es la variante B (ver_B_bactraking_como_ultimo_recurso).

## Organización actual
- ver_A_con_backtraking_inmediato/
  - src/
    - graph.h
    - graph.c
    - tsp.h
    - tsp.c
    - commands.h
    - commands.c
    - main.c
  - Makefile

- ver_B_bactraking_como_ultimo_recurso/
  - src/
    - estructuras.h
    - funciones.h
    - comandos.h
    - main.c
  - Makefile

- README.md (este archivo)
- Ficheros de ejemplo en cada variante: ruta1.txt, ruta_no_conexa.txt, ruta_grado_invalido.txt, etc.

## Compilación y ejecución
Selecciona la variante que quieras usar:

Ejemplo — variante A:
```bash
cd ver_A_con_backtraking_inmediato
make
```

Ejemplo — variante B:
```bash
cd ver_B_bactraking_como_ultimo_recurso
make
```

## Descripción de la variante A

Implementación en C del problema del vendedor viajante (Travelling Salesman Problem) que encuentra la ruta más corta para visitar todas las ciudades exactamente una vez y regresar al origen.

Esta versión maneja grafos no completos, verificando primero si existe un circuito hamiltoniano viable antes de calcular la ruta óptima.

### Características

- Representación de grafos mediante matriz de adyacencia
- Verificación de existencia de circuito hamiltoniano
- Cálculo de ruta de costo mínimo usando backtracking
- Manejo dinámico de memoria sin variables globales
- Soporte para grafos de 4 a 9 nodos

### Compilacion y ejecucion
para compilar
```bash
make
```
para ejecutar
```bash
make run
```

para limpiar archivos objeto y ejecutable:

```bash
make clean
```
#### Comandos disponibles

- `start <n>` - crea un grafo con n nodos (4-9)
- `read <archivo>` - carga enlaces desde archivo y calcula ruta optima
- `graph` - muestra la matriz de adyacencia actual
- `exit` - libera memoria y sale del programa

#### Formato de archivo de entrada

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

### Verificacion de circuito hamiltoniano

utiliza backtracking para determinar si existe al menos un camino que:
- comienza en el nodo a (indice 0)
- visita todos los nodos exactamente una vez
- regresa al nodo inicial

### Calculo de ruta optima

si existe un circuito valido, explora todas las permutaciones posibles mediante backtracking con poda:
- genera todas las rutas hamiltonianas
- calcula el costo total de cada ruta
- mantiene la ruta de costo minimo
- aplica poda cuando el costo parcial supera el minimo actual

## Gestion de memoria

- asignacion dinamica de matrices y arreglos
- liberacion apropiada de memoria en todas las rutas de ejecucion
- manejo de errores en asignaciones fallidas
- sin fugas de memoria (verificable con valgrind)

## Limitaciones

- maximo 9 nodos (definido en `PVV_MAX_NODOS`)
- complejidad exponencial: o(n!) para calcular ruta optima
- enfoque de fuerza bruta (adecuado para n <= 9)

## Descripción de la variante B

Esta variante del problema del vendedor viajante implementa un enfoque diferente, donde el backtracking se utiliza como último recurso. Esto significa que se verifican las condiciones que son vitales para un grafo hamiltoniano y luego de recurre al backtracking para encontrar la ruta óptima.
Las condiciones son:
- que el grafo sea conexo
- que todos los nodos del grafo posean grados válidos (grado >= 2; se consideran inválidos grado 0 o 1)
- que G−S tenga un número de componentes conexas menor o igual al tamaño de S para todo subconjunto S (condición de corte relacionada con Tutte)

### Características de la variante B

- Representación: listas de adyacencia (memoria eficiente para grafos dispersos).
- Identificadores por letra: soporta A..Z (A→0 … Z→25). Antes de `read` ejecutar `pvv start N` con N ≥ (índice mayor + 1).
- Validaciones previas al backtracking:
  - Comprobación de conectividad (DFS).
  - Comprobación de grados: detecta nodos con grado 0 o 1 y rechaza si existen.
  - Comprobación de la condición de cortes (G−S vs |S|) para reducir casos donde no puede existir ciclo hamiltoniano.
- Si las validaciones pasan, entonces se ejecuta backtracking como último recurso para encontrar el ciclo/hamiltoniano mínimo.
- Mensajes de error claros indicando:
  - índice fuera de rango (necesita un `start` mayor),
  - nodos con grado inválido (lista de nodos afectados),
  - número de componentes en G−S y el S que provoca fallo.
- Adecuada para grafos algo más grandes que la versión A, ya que evita backtracking en muchos casos mediante podas/chequeos previos.

### Uso y recomendaciones (variante B)

- Siempre ejecutar:
  - `pvv start <num_ciudades>`
  - `pvv read <archivo>`
  - `pvv graph` para inspeccionar la representación
- Si `read` falla por "índice fuera de rango", aumentar N en `start`.
- Si `read` indica "grado inválido" (0 ó 1), revisar el archivo de entrada — el grafo no puede tener hojas ni aislados.
- Si la condición G−S > |S| falla, el programa reportará el subconjunto S y el número de componentes que provocan la violación.

### Limitaciones específicas

- Las comprobaciones previas no garantizan existencia de ciclo hamiltoniano (solo filtran casos imposibles); si pasan, el coste final sigue siendo exponencial en el peor caso.
- Para entradas con muchas letras altas (p. ej. hasta Z) usar `pvv start 26`.
