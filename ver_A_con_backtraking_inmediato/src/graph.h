#ifndef GRAPH_H
#define GRAPH_H

#define PVV_MAX_NODOS 9
#define PVV_SIN_ARISTA -1

typedef struct {
    int n;
    int **costos;
    char *nombres;
} grafo;

int grafo_crear(grafo *g, int n);

void grafo_destruir(grafo *g);

int grafo_agregar_arista(grafo *g, int u, int v, int costo);

void grafo_imprimir(const grafo *g);

#endif
