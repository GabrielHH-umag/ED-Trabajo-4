#include "estructuras.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
/* El problema a abordar consiste en buscar una ruta que recorra todas las ciudades disponibles solamente una vez, además
regresando a la ciudad de origen, utilizando la ruta más corta posible. Sin embargo, para esta variante del problema en
particular, antes de realizar cálculo alguno, primero se debe verificar la existencia de una ruta que efectivamente permita
recorrer todas las ciudades volviendo a la original. Si esto no se cumple, el programa debe indicar que no es posible realizar
el circuito esperado en el viaje y terminar inmediatamente; solamente en caso de que exista un camino viable debe continuar
con el cálculo de la ruta más corta. Tenga presente que en caso de que exista una conexión entre un par de ciudades, el costo
de recorrerla es el mismo independiente al sentido en que se haga.
Para obtener este resultado, es necesario dividir el problema en tres partes:
1. Verificar que existe un camino en que se pueda recorrer cada ciudad y regresar a la de origen.
2. Encontrar los posibles caminos que recorren todas las ciudades, volviendo a la inicial.
3. Calcular cuál es la ruta de menor costo*/
int mejor_costo_global = INT_MAX; 
int mejor_ruta_global[MAX_NODOS + 1]; 
int ruta_encontrada = 0;

//------------------- Declaraciones implicitas ------------------//

int bienvenida();
void char_a_index(const char* c, int* index);
int inicializar_grafo(Grafo* grafo, int vertices);
Nodo* crear_nodo(int destino, int distancia);
int crear_camino_in_out(Grafo* grafo, int distancia, int origen, int destino);
int construir_grafo(Grafo* grafo, const char* txt);
void liberar_grafo(Grafo* grafo);
int verificar_hamiltoniano(Grafo* grafo);
int tiene_grado_invalido(Grafo* grafo);
int no_es_conexo(Grafo* grafo);
void dfs(Grafo* grafo, int nodo_actual, int* visitado, int* visitados);
int condicion_necesaria_de_corte(Grafo* grafo);
int contar_conjunto_g_menos_s(Grafo* grafo, int* s);
int ver_costo_ciudad(Grafo* grafo, int origen, int destino);
void ciclo_minimo_hamiltoniano(Grafo* grafo, int origen, int nodo_actual, int* visitado, int num_visitados, int costo_actual);
int backtracking_para_hamilton(Grafo* grafo);

//------------------- Funciones generales ------------------//

int bienvenida()
{
    printf("=========================================\n");
    printf("  Bienvenido al solucionador de rutas pvv\n");
    printf("=========================================\n\n");
    return 0;
}
void char_a_index(const char* c, int* index)
{
    if (*c >= 'A' && *c <= 'Z') 
    {
        *index = (int)(c[0] - 'A');
    } 
    else if (*c >= 'a' && *c <= 'z') 
    {
        *index = (int)(c[0] - 'a');
    } 
    else 
    {
        *index = -1; // Caracter inválido
    }
}
char index_a_char(int index)
{
    // Mapea 0 -> 'A', 1 -> 'B', 2 -> 'C'
    if (index >= 0 && index < 26) 
    {
        return (char)('A' + index); 
    }
    return '?'; // Carácter de error si el índice es inválido
}


//------------------- Funciones para manejo del grafo ------------------//
int inicializar_grafo(Grafo* grafo, int vertices)
{
    //Asigna el numero de ciudades
    grafo->num_ciudades = vertices;
    //Reservamos memoria para las listas de adyacencia
    grafo->listas_adyacencia = (Nodo**)malloc(vertices * sizeof(Nodo*));
    //Para ver si ocurrio un error al asignar memoria
    if (grafo->listas_adyacencia == NULL) 
    {
        fprintf(stderr, "Error: Falla al asignar memoria para las listas de adyacencia.\n");
        return -1; // ERROR
    }
    //Ok inicializamos las listas de adyacencia como NULL
    for (int i = 0; i < vertices; i++) 
    {
        grafo->listas_adyacencia[i] = NULL;
    }
    printf("Grafo inicializado con %d ciudades.\n", vertices);
    return 0;
}
Nodo* crear_nodo(int destino, int distancia)
{
    //Guardamos memoria para el nuevo nodo
    Nodo* nuevo_nodo = (Nodo*)malloc(sizeof(Nodo));
    //Error al asignar memoria
    if(nuevo_nodo == NULL)
    {
        fprintf(stderr, "Error: Falla al asignar memoria para un nuevo nodo.\n");
        return NULL; // ERROR
    }
    //Asignamos los valores al nodo
    nuevo_nodo->ciudad_vecina = destino;
    nuevo_nodo->distancia = distancia;
    //Decimos que es el ultimo nodo hasta ahora
    nuevo_nodo->siguiente = NULL;
    return nuevo_nodo;
}
int crear_camino_in_out(Grafo* grafo, int distancia, int origen, int destino)
{
    if (grafo == NULL) 
    {
        fprintf(stderr, "Error: grafo NULL en crear_camino_in_out.\n");
        return -1;
    }
    if (origen < 0 || origen >= grafo->num_ciudades || destino < 0 || destino >= grafo->num_ciudades) 
    {
        fprintf(stderr, "Error: Indices fuera de rango: origen=%d destino=%d num_ciudades=%d\n", origen, destino, grafo->num_ciudades);
        return -1;
    }
    Nodo* link_origen_destino = crear_nodo(destino, distancia);
    if (link_origen_destino == NULL)
    {
        fprintf(stderr, "Error: Falla al crear el enlace de %d a %d.\n", origen, destino);
        return -1; // ERROR al crear el nodo
    }
    link_origen_destino->siguiente = grafo->listas_adyacencia[origen];
    grafo->listas_adyacencia[origen] = link_origen_destino;

    Nodo* link_destino_origen = crear_nodo(origen, distancia);
    if (link_destino_origen == NULL) 
    {
        fprintf(stderr, "Error: Falla al crear el enlace de %d a %d.\n", destino, origen);
        return -1; // ERROR al crear el nodo
    }
    link_destino_origen->siguiente = grafo->listas_adyacencia[destino];
    grafo->listas_adyacencia[destino] = link_destino_origen;
    return 0;
}

int construir_grafo(Grafo* grafo, const char* txt)
{
    if (grafo == NULL || txt == NULL) {
        fprintf(stderr, "Error: argumentos invalidos a construir_grafo.\n");
        return -1;
    }
    FILE* archivo = fopen(txt, "r");
    if (archivo == NULL) 
    {
        fprintf(stderr, "Error: No se pudo abrir el archivo %s.\n", txt);
        return -1; // ERROR al abrir el archivo
    }
    char c_origen, c_destino;
    int origen, destino, distancia;
    int linea = 0;
    while (fscanf(archivo, " %c %c %d", &c_origen, &c_destino, &distancia) == 3) 
    {
        char_a_index(&c_origen, &origen);
        char_a_index(&c_destino, &destino);
        if (origen == -1 || destino == -1) 
        {
            fprintf(stderr, "Error Linea %d: caracter de ciudad invalido: '%c' '%c'\n", linea, c_origen, c_destino);
            fclose(archivo);
            return -1;
        }
        if (origen < 0 || origen >= grafo->num_ciudades || destino < 0 || destino >= grafo->num_ciudades) 
        {
            fprintf(stderr, "Error Linea %d: indices fuera de rango (A->0): origen=%d destino=%d num_ciudades=%d\n", linea, origen, destino, grafo->num_ciudades);
            fclose(archivo);
            return -1;
        }
        if (crear_camino_in_out(grafo, distancia, origen, destino) != 0) 
        {
            fclose(archivo);
            return -1; // ERROR al crear el camino
        }
    }
    fclose(archivo);
    return 0;
}
void dfs(Grafo* grafo, int nodo_actual, int* visitado, int* visitados)
{
    visitado[nodo_actual] = 1; //marcamos como visitado el nodo en el que estamos
    (*visitados)++;
    Nodo* actual = grafo->listas_adyacencia[nodo_actual]; //buscamos la lista con sus vecinos
    while (actual != NULL) //continuamos hasta que no haya mas vecinos
    {
        int vecino = actual->ciudad_vecina;
        if (!visitado[vecino])
        {
            dfs(grafo, vecino, visitado, visitados);
        }
        actual = actual->siguiente; //avanzamos al siguiente vecino
    }
}
void liberar_grafo(Grafo* grafo)
{
    for (int i = 0; i < grafo->num_ciudades; i++) 
    {
        Nodo* actual = grafo->listas_adyacencia[i];
        while (actual != NULL) 
        {
            Nodo* temp = actual;
            actual = actual->siguiente;
            free(temp);
        }
    }
    free(grafo->listas_adyacencia);
}

//------------------- Funciones para verificar si es hamiltoniano ------------------//

int verificar_hamiltoniano(Grafo* grafo)
{
    //usaremos las condiciones mas faciles de cumplir para descartar la existencia de un ciclo hamiltoniano, para despues usar Backtracking

    printf("Verificando ciclo hamiltoniano...\n");
    if(no_es_conexo(grafo))
    {
        printf("No existe un ciclo hamiltoniano en el grafo, ya que no es conexo.\n");
        return 0; // No existe ciclo hamiltoniano
    }
    if(tiene_grado_invalido(grafo))
    {
        printf("No existe un ciclo hamiltoniano en el grafo, ya que posee un nodo con grado menor a 2.\n");
        return 0; // No existe ciclo hamiltoniano
    }
    if(condicion_necesaria_de_corte(grafo))
    {
        printf("No existe un ciclo hamiltoniano en el grafo, ya que no cumple la condicion necesaria de corte de Tutte de G-S <= |S|.\n");
        return 0; // No existe ciclo hamiltoniano
    }
    if(backtracking_para_hamilton(grafo) == 0)
    {
        printf("No existe un ciclo hamiltoniano en el grafo, ya que no se encontro ninguna ruta valida que cumpla con las condiciones.\n");
        return 0; // No existe ciclo hamiltoniano
    }
    printf("Se ha encontrado un ciclo hamiltoniano de menor coste en el grafo, la ruta es:\n");
    for (int i = 0; i <= grafo->num_ciudades; i++) {
    printf("%c", index_a_char(mejor_ruta_global[i])); // Imprimimos la ciudad
    
    // Verificamos si NO es el último elemento para imprimir el separador " -> "
    if (i < grafo->num_ciudades) 
    {
        printf(" -> ");
    }
    
}   
    printf("\nCosto total: %d\n", mejor_costo_global);
    return 1; // 1 si existe, 0 si no existe
}
int no_es_conexo(Grafo* grafo)
{
    if (grafo->num_ciudades <= 1)
    {
        return 0; //seria considerado conexo, sin embargo seria un grafo de grado 0 por lo que mas adelante se descartaria
    }
    int* visitado = (int*)calloc(grafo->num_ciudades, sizeof(int));
    if (visitado == NULL)
    {
        fprintf(stderr, "Error: Falla al asignar memoria para el arreglo de visitados.\n");
        return -1; // ERROR
    }
    //Para verificar la conexidad usamos DFS (Depth-First Search)
    int inicio = 0;
    int visitados = 0;
    dfs(grafo, inicio, visitado, &visitados);
    free(visitado);
    if (visitados == grafo->num_ciudades)
    {
        return 0; // El grafo es conexo
    }
    else
    {
        return 1; // El grafo no es conexo
    }
}
int tiene_grado_invalido(Grafo* grafo)
{
    //Verificamos que no haya ningun nodo de grado menor a 2 ya que si esto ocurre automaticamente no existe ciclo hamiltoniano
    for (int i = 0; i < grafo->num_ciudades; i++) 
    {
        int grado = 0;
        Nodo* actual = grafo->listas_adyacencia[i];
        while (actual != NULL) 
        {
            grado++;
            actual = actual->siguiente;
        }
        if (grado < 2) 
        {
            return 1; // Existe un nodo con grado menor a 2
        }
    }
    return 0; // Todos los nodos tienen grado al menos 2
}
int condicion_necesaria_de_corte(Grafo* grafo)//Condicion de Tutte para grafos hamiltonianos
{
    //Aqui implementamos la condicion necesaria de corte de Tutte
    //La cual dice que para cualquier subconjunto S de nodos, el numero de componentes conexas en G-S debe ser menor o igual al tamaño de S
    int* s = (int*)calloc(grafo->num_ciudades, sizeof(int));
    if (s == NULL)
    {
        fprintf(stderr, "Error: Falla al asignar memoria para el arreglo S.\n");
        return -1; // ERROR
    }

    //Empezamos a probar todos los subconjuntos S posibles con 2^n-s combinaciones
    for (int i = 1; i < (1<<grafo->num_ciudades); i++) //en este caso 1<<grafo->num_ciudades es 2 elevado a num_ciudades
    {
        int tamano_s = 0;
        //Construimos el conjunto S
        for (int j = 0; j < grafo->num_ciudades; j++)
        {
            if ((i >> j) & 1) //Si el bit j de i esta activo, incluimos el nodo j en S (si es parte de S)
            {
                s[j] = 1;
                tamano_s++;
            }
            else
            {
                s[j] = 0;
            }
        }
        //Contamos el numero de componentes conexas en G-S
        int num_componentes = contar_conjunto_g_menos_s(grafo, s);
        if (num_componentes > tamano_s)
        {
            free(s);
            return 1; // La condicion de Tutte no se cumple, por lo tanto no es hamiltoniano
        }
    }
    free(s);
    return 0; // La condicion de Tutte se cumple para todos los subconjuntos S
}
int contar_conjunto_g_menos_s(Grafo* grafo, int* s)
{
    //Lo que hacemos aqui es realizar la opercacion de G-S, donde S es un subconjunto de G (Grafo)
    int num_componentes = 0;
    //usaremos un DFS, la diferencia con el DFS en no_es_conexo es que aqui marcaremos como visitados los nodos en S antes de que se inicie el DFS
    int* visitado = (int*)calloc(grafo->num_ciudades, sizeof(int));
    if (visitado == NULL)
    {
        fprintf(stderr, "Error: Falla al asignar memoria para el arreglo de visitados.\n");
        return -1; // ERROR
    }
    //Marcaremos los nodos S como visitados para calcular G-S
    for (int i = 0; i < grafo->num_ciudades; i++)
    {
        if (s[i])
        {
            visitado[i] = 1;
        }
    }
    for(int i =0 ; i < grafo->num_ciudades; i++)
    {
        if(!visitado[i])
        {
            //Llamamos al DFS para marcar todos los nodos conectados a i (G-S)
            num_componentes++;
            int temp = 0;
            dfs(grafo, i, visitado, &temp);
        }
    }
    free(visitado);
    return num_componentes;
}
int ver_costo_ciudad(Grafo* grafo, int origen, int destino)
{
    Nodo* actual = grafo->listas_adyacencia[origen];
    while (actual != NULL)
    {
        if (actual->ciudad_vecina == destino) 
        {
            return actual->distancia; // Retornamos la distancia si encontramos el destino
        }
        actual = actual->siguiente;
    }
    return -1; // Retornamos -1 si no encontramos el destino
}
void ciclo_minimo_hamiltoniano(Grafo* grafo, int origen, int nodo_actual, int* visitado, int num_visitados, int costo_actual)
{
    visitado[nodo_actual] = 1; // Marcamos el nodo actual como visitado
    mejor_ruta_global[num_visitados] = nodo_actual; // Guardamos el nodo en la ruta actual
    num_visitados++;
    //Podamos si el costo actual ya es mayor o igual al mejor costo global encontrado
    if(mejor_costo_global<= costo_actual)
    {
        visitado[nodo_actual] = 0; //Reiniciamos, ya que se encontro anteriormente una ruta mejor
        return;
    }
    // Si hemos visitado todas las ciudades, verificamos si hay una arista de regreso al origen
    if (num_visitados == grafo->num_ciudades) 
    {
        // El último nodo visitado es nodo_actual. Verificamos la arista de cierre: nodo_actual -> origen
        int costo_cierre = ver_costo_ciudad(grafo, nodo_actual, origen); 
        if (costo_cierre != -1) 
        { 
            int costo_total = costo_actual + costo_cierre;
            // Si encontramos un ciclo más corto, actualizamos la global
            if (costo_total < mejor_costo_global) 
            {
                mejor_costo_global = costo_total;
                ruta_encontrada = 1;
                // Guardar la ruta óptima
                mejor_ruta_global[grafo->num_ciudades] = origen; 
            }
        }
        // Backtracking y regreso para probar otras ramas
        visitado[nodo_actual] = 0;
        return; 
    }
    // Explorar los nodos vecinos
    Nodo* actual = grafo->listas_adyacencia[nodo_actual];
    while (actual != NULL)
    {
        int vecino = actual->ciudad_vecina;
        int distancia = actual->distancia;
        if (!visitado[vecino]) 
        {
            ciclo_minimo_hamiltoniano(grafo, origen, vecino, visitado, num_visitados, costo_actual + distancia);
        }
        actual = actual->siguiente;
    }
    // Backtracking: desmarcar el nodo actual antes de regresar
    visitado[nodo_actual] = 0;
}
int backtracking_para_hamilton(Grafo* grafo)
{
    int* visitado = (int*)calloc(grafo->num_ciudades, sizeof(int));
    if (visitado == NULL)
    {
        fprintf(stderr, "Error: Falla al asignar memoria para el arreglo de visitados.\n");
        return -1; // ERROR
    }
    int origen = 0; // Empezamos desde la ciudad 0
    mejor_costo_global = INT_MAX;
    ruta_encontrada = 0;
    ciclo_minimo_hamiltoniano(grafo, origen, origen, visitado, 0, 0);
    free(visitado);
    return ruta_encontrada;
}
