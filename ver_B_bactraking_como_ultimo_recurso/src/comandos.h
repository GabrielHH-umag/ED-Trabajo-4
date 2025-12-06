#include "funciones.h"

//------------------- Declaraciones implicitas ------------------//
int leer_comandos(Comando* c);
int ejecutar_comando(Comando* c, Grafo* grafo);
void pvv_help();
void pvv_start(const char* arg, Grafo *grafo);
void pvv_read(const char* arg, Grafo *grafo);
void pvv_graph(Grafo *grafo);

//------------------- Funciones para manejar comandos ------------------//

int leer_comandos(Comando* c)
{
    char buffer[MAX_CMD];
    printf("> ");
    if (!fgets(buffer, sizeof(buffer), stdin)) 
    {
        c->cmd[0] = c->arg1[0] = c->arg2[0] = '\0';
        return -1; //ERROR al leer el comando
    }
    // Parsear el comando y sus argumentos
    buffer[strcspn(buffer, "\n")] = 0; // Eliminar el salto de línea
    sscanf(buffer, "%15s %63s %63s", c->cmd, c->arg1, c->arg2);
    return 0;
}
int ejecutar_comando(Comando* c, Grafo* grafo)
{
    if (strcmp(c->cmd, "pvv") != 0) 
    {
        printf("Comando no reconocido. Use 'pvv <accion>'.\n");
        return 1;
    } 
    
    if (strcmp(c->arg1, "start") == 0) 
    {
        if (c->arg2[0] == '\0') 
        {
            fprintf(stderr, "Error: Use 'pvv start <num_ciudades>'\n");
            return 0;
        }
        if (atoi(c->arg2) > MAX_NODOS )
        {
            fprintf(stderr, "Error: Ha sobrepasado el limite de nodos, si desea modificar el limite cambie MAX_NODOS\n");
            return 0;
        }
        pvv_start(c->arg2, grafo);
    } 
    else if (strcmp(c->arg1, "read") == 0) 
    {
        if (c->arg2[0] == '\0') 
        {
            fprintf(stderr, "Error: Use 'pvv read <archivo>'\n");
            return 0;
        }
        pvv_read(c->arg2, grafo);
    } 
    else if (strcmp(c->arg1, "graph") == 0) 
    {
        pvv_graph(grafo);
    } 
    else if (strcmp(c->arg1, "exit") == 0) 
    {
        printf("Limpiando cache y saliendo...\n"); 
        return -1;
    }
    else if (strcmp(c->arg1, "help") == 0) 
    {
        pvv_help();
    }
    else 
    {
        printf("Accion '%s' no reconocida. use pvv help para ver los comandos validos\n", c->arg1);
    }
    return 0;
}

//------------------- Implementaciones de comandos ------------------//
void pvv_help()
{
    printf("Comandos disponibles:\n");
    printf("  pvv start <num_ciudades>  - Inicializa el grafo con el numero de ciudades.\n");
    printf("  pvv read <archivo>        - Lee el grafo desde un archivo de texto.\n");
    printf("  pvv graph                 - Muestra la matriz de adyacencia del grafo.\n");
    printf("  pvv exit                  - Sale del programa.\n");
}
void pvv_start(const char* arg, Grafo *grafo)
{
    if(atoi(arg) <= 0 )
    {
        fprintf(stderr, "Error: Ingrese un numero valido de ciudades\n");
        return; // ERROR
    }
    inicializar_grafo(grafo, atoi(arg));//usamos atoi para convertir a entero
    
}
void pvv_read(const char* arg, Grafo *grafo)
{
    if(grafo->num_ciudades <= 0)
    {
        fprintf(stderr, "Error: El grafo no ha sido inicializado correctamente, use pvv start <cantidad> para inicializar su grafo.\n");
        grafo->estado = GRAFO_ERR;
        return; // ERROR
    }
    printf("Leyendo archivo: %s...\n", arg);
    if(construir_grafo(grafo, arg)==-1)
    {
        grafo->estado = GRAFO_ERR;
        return; // ERROR
    }
    grafo->estado = GRAFO_LEIDO;
    verificar_hamiltoniano(grafo);
}
void pvv_graph(Grafo *g)
{
    if (g == NULL || g->listas_adyacencia == NULL || g->num_ciudades <= 0) 
    {
        printf("No hay grafo creado. Use 'pvv start <num>' primero.\n");
        return;
    }
    switch (g->estado) 
    {
        case GRAFO_ERR:
            printf("El grafo se encuentra en un estado de error y no puede ser mostrado.\n");
            return;
        case GRAFO_VACIO:
            printf("El grafo esta vacio.\n");
            return;
        case GRAFO_INICIALIZADO:
            printf("El grafo ha sido inicializado pero no se han agregado caminos aun.\n");
            return;
        case GRAFO_LEIDO:
            printf("El grafo ha sido leido pero no se ha procesado aun.\n");
            return;
        case GRAFO_PROCESADO:
            printf("El grafo ha sido procesado.\n");
            return;
        case GRAFO_HAMILTONIANO:
            printf("El grafo contiene un ciclo hamiltoniano.\n");
            break;
        case GRAFO_NO_HAMILTONIANO:
            printf("El grafo no contiene un ciclo hamiltoniano y no va a ser mostrado\n");
            return;
    }
    int n = g->num_ciudades;
    int i, j;

    printf("\nMatriz de adyacencia (%d nodos):\n", n);

    printf("   ");
    for (i = 0; i < n; i++) {
        printf(" %c ", index_a_char(i));
    }
    printf("\n");

    for (i = 0; i < n; i++) 
    {
        printf(" %c ", index_a_char(i));
        for (j = 0; j < n; j++) 
        {
            int costo = ver_costo_ciudad(g, i, j);
            if (costo == -1) 
            {
                printf(" - ");
            } 
            else 
            {
                printf("%2d ", costo);
            }
        }
        printf("\n");
    }
}

