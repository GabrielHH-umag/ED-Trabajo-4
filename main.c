#include <stdio.h>
#include <string.h>
#include "graph.h"
#include "commands.h"

int main(void) {
    grafo grafo;
    grafo.n = 0;
    grafo.costos = NULL;
    grafo.nombres = NULL;

    char linea[512];
    int salir = 0;

    printf("tarea 4 - problema del viajante (tsp)\n");
    printf("comandos:\n");
    printf("  start <n>\n");
    printf("  read <archivo>\n");
    printf("  graph\n");
    printf("  exit\n");

    while (!salir) {
        printf(">pvv ");
        if (fgets(linea, sizeof(linea), stdin) == NULL) {
            break;
        }

        if (strcmp(linea, "\n") == 0) {
            continue;
        }

        procesar_comando(linea, &grafo, &salir);
    }

    grafo_destruir(&grafo);

    return 0;
}
