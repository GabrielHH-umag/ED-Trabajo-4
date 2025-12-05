#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commands.h"
#include "tsp.h"

static int letra_a_indice(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a';
    }
    if (c >= 'A' && c <= 'Z') {
        return c - 'A';
    }
    return -1;
}

static char indice_a_letra(int idx) {
    return (char)('a' + idx);
}

static void comando_start(const char *arg, grafo *g) {
    if (g->costos != NULL) {
        printf("ya existe un grafo creado. debes salir y volver a iniciar si quieres otro.\n");
        return;
    }

    if (arg == NULL || *arg == '\0') {
        printf("uso: start <n>\n");
        return;
    }

    int n = atoi(arg);
    int estado = grafo_crear(g, n);
    if (estado == 0) {
        printf("grafo creado con %d nodos.\n", n);
    } else {
        printf("error al crear el grafo (codigo %d).\n", estado);
    }
}

static void comando_graph(grafo *g) {
    grafo_imprimir(g);
}

static void comando_read(const char *archivo, grafo *g, int *salir) {
    if (g->costos == NULL) {
        printf("primero debes crear un grafo con 'start n'.\n");
        return;
    }

    if (archivo == NULL || *archivo == '\0') {
        printf("uso: read <nombre_archivo>\n");
        return;
    }

    FILE *f = fopen(archivo, "r");
    if (f == NULL) {
        printf("no se pudo abrir el archivo '%s'.\n", archivo);
        return;
    }

    printf("agregando enlaces desde archivo.\n");

    char origen, destino;
    int costo;
    while (fscanf(f, " %c %c %d", &origen, &destino, &costo) == 3) {
        int u = letra_a_indice(origen);
        int v = letra_a_indice(destino);
        if (u < 0 || v < 0 || u >= g->n || v >= g->n) {
            printf("linea invalida en archivo: %c %c %d\n", origen, destino, costo);
            continue;
        }
        grafo_agregar_arista(g, u, v, costo);
    }

    fclose(f);

    printf("verificando que existe una ruta.\n");

    if (!tsp_existe_circuito(g)) {
        printf("no existe un camino que recorra todas las ciudades y regrese a la ciudad de origen.\n");
        *salir = 1;
        return;
    }

    printf("existe un camino que recorre todas las ciudades y regresa a la ciudad de origen.\n");

    int mejor_ruta[PVV_MAX_NODOS + 1];
    int mejor_costo;

    if (!tsp_mejor_ruta(g, mejor_ruta, &mejor_costo)) {
        printf("error al calcular la ruta de costo minimo.\n");
        return;
    }

    printf("ruta a seguir: ");
    for (int i = 0; i <= g->n; i++) {
        printf("%c ", indice_a_letra(mejor_ruta[i]));
    }
    printf("\n");

    printf("costo total minimo: %d\n", mejor_costo);
}

void procesar_comando(const char *line, grafo *g, int *salir) {
    char comando[32];
    char arg[256];

    *salir = 0;
    comando[0] = '\0';
    arg[0] = '\0';

    int cant = sscanf(line, "%31s %255s", comando, arg);

    if (cant <= 0) {
        return;
    }

    if (strcmp(comando, "start") == 0) {
        comando_start((cant >= 2) ? arg : NULL, g);
    } else if (strcmp(comando, "graph") == 0) {
        comando_graph(g);
    } else if (strcmp(comando, "read") == 0) {
        comando_read((cant >= 2) ? arg : NULL, g, salir);
    } else if (strcmp(comando, "exit") == 0) {
        printf("limpiando cache y saliendo ...\n");
        *salir = 1;
    } else {
        printf("comando no reconocido.\n");
    }
}
