#define MAX_CMD 128
#define MAX_ARG 64
#define MAX_NODOS 10

//estados del grafo
#define GRAFO_ERR -1
#define GRAFO_VACIO 0
#define GRAFO_INICIALIZADO 1
#define GRAFO_LEIDO 2
#define GRAFO_PROCESADO 3
#define GRAFO_HAMILTONIANO 4
#define GRAFO_NO_HAMILTONIANO 5

#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

typedef struct 
{
    char cmd[16]; //pvv
    char arg1[MAX_ARG];
    char arg2[MAX_ARG];
} Comando;
typedef struct Nodo_ 
{
    int ciudad_vecina;
    int distancia;
    struct Nodo_* siguiente;
} Nodo;
typedef struct 
{
    int num_ciudades;
    Nodo** listas_adyacencia;
    int estado;
}Grafo;
#endif // ESTRUCTURAS_H