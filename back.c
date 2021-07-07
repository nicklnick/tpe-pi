//
// Created by on 6/7/2021.
//
#include <stdio.h>
#include <stdlib.h>
#include "imdbTAD.h"

/* Crea el archivo CSV en el que se cargarán los datos */
/* fields = los campos que va a tener el .csv */
FILE *
createCSV(const char * fileName)
{
    // Si no me pasan el nombre del archivo
    if( fileName == NULL ) return NULL; // Error

    // Creamos el archivo para escritura, y cargamos los datos.
    FILE * newFile = fopen(fileName, "wt");
    fclose(newFile);
    return newFile;
}

/*
 * Carga los datos en los archivos respectivos
 */
int
loadData(imdbADT data, FILE * query1, FILE * query2, FILE * query3)
{
    toBeginYear(data);
    while( next(data) != NULL )
    {
        solveQuery1(data, query1);
        solveQuery2(data, query2);
        solveQuery3(data, query3);
    }
}

/*
 * Crea los archivos .csv para las distintas queries y carga en ellos los datos correspondientes
 */
void
processData(imdbADT data)
{
    FILE * q1, * q2, * q3; // Punteros a los files en el que se volcaran los datos de las queries
    q1 = createCSV("query1.csv");
    q2 = createCSV("query2.csv");
    q3 = createCSV("query3.csv");

    loadData(data, q1, q2, q3);
}
