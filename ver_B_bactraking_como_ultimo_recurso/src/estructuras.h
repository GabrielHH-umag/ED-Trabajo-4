#define MAX_CMD 128
#define MAX_ARG 64
#define MAX_NODOS 10

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
}Grafo;
#endif // ESTRUCTURAS_H