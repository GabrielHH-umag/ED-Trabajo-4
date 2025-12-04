#include <stdlib.h>
#include <stdio.h>
#include "comandos.h"


int main() 
{
    Grafo grafo;
    Comando c;
    bienvenida();
    while(1) 
    {
        leer_comandos(&c);
        if (ejecutar_comando(&c, &grafo) == -1) 
        {
            liberar_grafo(&grafo);
            break; // Salir del bucle si el comando es exit
        }
    }
    return 0;
}