//
// Created by on 6/7/2021.
//
#include <stdio.h>
#include <stdlib.h>
#include "imdbTAD.h"

/* Crea el archivo CSV en el que se cargarán los datos */
/* fields = los campos que va a tener el .csv */
FILE *
createCSV(const char * fileName, const char * fields)
{
    // Si no me pasan el nombre del archivo
    if( fileName == NULL || fields == NULL ) return NULL; // Error

    // Creamos el archivo para escritura, y cargamos los datos.
    FILE * newFile = fopen(fileName, "wt");
    fputs(fields, newFile);
    fclose(newFile);
    return newFile;
}

