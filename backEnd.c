//
// Created by on 6/7/2021.
//
#include <stdio.h>
#include <stdlib.h>
#include "imdbTAD.h"

#define FNAME_Q1 "query1.csv"
#define FNAME_Q2 "query2.csv"
#define FNAME_Q3 "query3.csv"

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


void
solveQuery1(imdbADT data, FILE * query1)
{
    size_t cantPelis, cantSeries;
    unsigned year;

    getAmountCurrYear(data, &cantPelis, &cantSeries, &year);
    fprintf(query1, "%d;%zu;%zu\n", year, cantPelis, cantSeries); // Para imprimir size_t tiene que ser %zu
}

void
solveQuery2(imdbADT data, FILE * query2)
{
    size_t cantPelis;
    unsigned year;
    char * genero;

    toBeginG(data);
    while( nextG(data) != NULL )
    {
        getAmountG(data, &year, &genero, &cantPelis);
        fprintf(query2, "%d;%zu;%s\n", year, cantPelis, genero);
    }
}

void
solveQuery3(imdbADT data, FILE * query3)
{
    TEntry peli, serie;
    peli = getMostPopular(data, PELI);
    serie = getMostPopular(data, SERIE);
    fprintf(query3, "%d;%s;%zu;.2f;%s;zu;.2f",
            peli.startYear, peli.name, peli.numVotes, peli.avgRating,
            serie.name, serie.numVotes, serie.avgRating);
}
/*
 * Carga los datos en los archivos respectivos
 */
int
loadData(imdbADT data, FILE * query1, FILE * query2, FILE * query3)
{
    fopen(FNAME_Q1, "at");
    fputs("year;films;series", query1);
    fopen(FNAME_Q2, "at");
    fputs("year;genre;films", query2);
    fopen(FNAME_Q3, "at");
    fputs("startYear;film;votesFilm;ratingFilm;serie;votesSerie;ratingSerie", query3);
    toBeginYear(data);
    while( next(data) != NULL )
    {
        solveQuery1(data, FNAME_Q1);
        solveQuery2(data, FNAME_Q2);
        solveQuery3(data, FNAME_Q3);
    }
    fclose(query3);
    fclose(query2);
    fclose(query1);
}

/*
 * Crea los archivos .csv para las distintas queries y carga en ellos los datos correspondientes
 */
void
processData(imdbADT data)
{
    FILE * q1, * q2, * q3; // Punteros a los files en el que se volcaran los datos de las queries
    q1 = createCSV(FNAME_Q1);
    q2 = createCSV(FNAME_Q2);
    q3 = createCSV(FNAME_Q3);

    loadData(data, q1, q2, q3);
}
