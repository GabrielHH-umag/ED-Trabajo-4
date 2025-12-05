#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

int grafo_crear(grafo *g, int n) {
    int i, j;

    if (g == NULL) {
        return 1;
    }

    if (n <= 0 || n > PVV_MAX_NODOS) {
        return 2;
    }

    g->n = n;
    g->costos = NULL;
    g->nombres = NULL;

    g->costos = (int **) malloc(n * sizeof(int *));
    if (g->costos == NULL) {
        return 3;
    }

    for (i = 0; i < n; i++) {
        g->costos[i] = (int *) malloc(n * sizeof(int));
        if (g->costos[i] == NULL) {
            int k;
            for (k = 0; k < i; k++) {
                free(g->costos[k]);
            }
            free(g->costos);
            g->costos = NULL;
            return 3;
        }
    }

    g->nombres = (char *) malloc(n * sizeof(char));
    if (g->nombres == NULL) {
        for (i = 0; i < n; i++) {
            free(g->costos[i]);
        }
        free(g->costos);
        g->costos = NULL;
        return 3;
    }

    for (i = 0; i < n; i++) {
        g->nombres[i] = (char)('a' + i);
        for (j = 0; j < n; j++) {
            if (i == j) {
                g->costos[i][j] = 0;
            } else {
                g->costos[i][j] = PVV_SIN_ARISTA;
            }
        }
    }

    return 0;
}

void grafo_destruir(grafo *g) {
    if (g == NULL) {
        return;
    }

    if (g->costos != NULL) {
        for (int i = 0; i < g->n; i++) {
            free(g->costos[i]);
        }
        free(g->costos);
        g->costos = NULL;
    }

    if (g->nombres != NULL) {
        free(g->nombres);
        g->nombres = NULL;
    }

    g->n = 0;
}

int grafo_agregar_arista(grafo *g, int u, int v, int costo) {
    if (g == NULL || g->costos == NULL) {
        return 1;
    }
    if (u < 0 || v < 0 || u >= g->n || v >= g->n) {
        return 2;
    }
    if (costo <= 0) {
        return 3;
    }

    g->costos[u][v] = costo;
    g->costos[v][u] = costo;

    return 0;
}

void grafo_imprimir(const grafo *g) {
    if (g == NULL || g->costos == NULL) {
        printf("no hay grafo creado.\n");
        return;
    }

    int n = g->n;
    int i, j;

    printf("matriz de adyacencia (%d nodos):\n", n);

    printf("   ");
    for (i = 0; i < n; i++) {
        printf(" %c ", g->nombres[i]);
    }
    printf("\n");

    for (i = 0; i < n; i++) {
        printf(" %c ", g->nombres[i]);
        for (j = 0; j < n; j++) {
            if (g->costos[i][j] == PVV_SIN_ARISTA) {
                printf(" - ");
            } else {
                printf("%2d ", g->costos[i][j]);
            }
        }
        printf("\n");
    }
}
