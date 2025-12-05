#ifndef TSP_H
#define TSP_H

#include "graph.h"

int tsp_existe_circuito(const grafo *g);

int tsp_mejor_ruta(const grafo *g, int *mejor_ruta, int *mejor_costo);

#endif
