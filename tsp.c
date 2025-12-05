#include <limits.h>
#include <stdlib.h>
#include "tsp.h"

static void backtrack_existe(const grafo *g,
                             int nivel,
                             int actual,
                             int *visitado,
                             int *encontrado) {
    if (*encontrado) {
        return;
    }

    int n = g->n;

    if (nivel == n) {
        if (g->costos[actual][0] != PVV_SIN_ARISTA) {
            *encontrado = 1;
        }
        return;
    }

    for (int sig = 1; sig < n; sig++) {
        if (!visitado[sig] && g->costos[actual][sig] != PVV_SIN_ARISTA) {
            visitado[sig] = 1;
            backtrack_existe(g, nivel + 1, sig, visitado, encontrado);
            visitado[sig] = 0;
            if (*encontrado) {
                return;
            }
        }
    }
}

int tsp_existe_circuito(const grafo *g) {
    if (g == NULL || g->costos == NULL || g->n <= 2) {
        return 0;
    }

    int n = g->n;
    int *visitado = (int *) calloc(n, sizeof(int));
    if (visitado == NULL) {
        return 0;
    }

    int encontrado = 0;
    visitado[0] = 1;

    backtrack_existe(g, 1, 0, visitado, &encontrado);

    free(visitado);
    return encontrado;
}

static void backtrack_mejor(const grafo *g,
                            int nivel,
                            int actual,
                            int costo_actual,
                            int *visitado,
                            int *ruta_actual,
                            int *mejor_ruta,
                            int *mejor_costo) {
    int n = g->n;

    if (costo_actual >= *mejor_costo) {
        return;
    }

    if (nivel == n) {
        if (g->costos[actual][0] != PVV_SIN_ARISTA) {
            int costo_total = costo_actual + g->costos[actual][0];
            if (costo_total < *mejor_costo) {
                *mejor_costo = costo_total;
                for (int i = 0; i < n; i++) {
                    mejor_ruta[i] = ruta_actual[i];
                }
                mejor_ruta[n] = 0;
            }
        }
        return;
    }

    for (int sig = 1; sig < n; sig++) {
        if (!visitado[sig] && g->costos[actual][sig] != PVV_SIN_ARISTA) {
            visitado[sig] = 1;
            ruta_actual[nivel] = sig;

            backtrack_mejor(g,
                            nivel + 1,
                            sig,
                            costo_actual + g->costos[actual][sig],
                            visitado,
                            ruta_actual,
                            mejor_ruta,
                            mejor_costo);

            visitado[sig] = 0;
        }
    }
}

int tsp_mejor_ruta(const grafo *g, int *mejor_ruta, int *mejor_costo) {
    if (g == NULL || g->costos == NULL || g->n <= 2) {
        return 0;
    }

    int n = g->n;
    int *visitado = (int *) calloc(n, sizeof(int));
    int *ruta_actual = (int *) malloc((n + 1) * sizeof(int));
    if (visitado == NULL || ruta_actual == NULL) {
        free(visitado);
        free(ruta_actual);
        return 0;
    }

    for (int i = 0; i < n; i++) {
        visitado[i] = 0;
    }

    *mejor_costo = INT_MAX;

    visitado[0] = 1;
    ruta_actual[0] = 0;

    backtrack_mejor(g,
                    1,
                    0,
                    0,
                    visitado,
                    ruta_actual,
                    mejor_ruta,
                    mejor_costo);

    free(visitado);
    free(ruta_actual);

    if (*mejor_costo == INT_MAX) {
        return 0;
    }
    return 1;
}
