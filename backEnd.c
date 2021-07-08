//
// Created by on 6/7/2021.
//
#include "backEnd.h"


/* Crea un archivo "filename" */
static FILE *
createCSV(const char * fileName)
{
    // Si no me pasan el nombre del archivo
    if( fileName == NULL ) return NULL; // Error

    // Creamos el archivo para escritura, y cargamos los datos.
    FILE * newFile = fopen(fileName, "w");
    return newFile;
}

/* Carga los datos del query1 en su respectivo archivo */
static void
solveQuery1(imdbADT data, FILE * query1)
{
    unsigned year, cantPelis, cantSeries;

    getAmountCurrY(data, &year, &cantPelis, &cantSeries);
    fprintf(query1, "%d;%d;%d\n", year, cantPelis, cantSeries); // Para imprimir size_t tiene que ser %zu
}

/* Carga los datos del query2 en su respectivo archivo */
static void
solveQuery2(imdbADT data, FILE * query2)
{
    unsigned year, cantPelis;
    char * genero;

    toBeginG(data);
    do
    {
        getAmountG(data, &year, &genero, &cantPelis);
        fprintf(query2, "%d;%d;%s\n", year, cantPelis, genero);
    }
    while( nextG(data) );
}

/* Carga los datos del query3 en su respectivo archivo */
static void
solveQuery3(imdbADT data, FILE * query3)
{
    TEntry * peli, * serie;
    peli = getMostPopular(data, PELI);
    serie = getMostPopular(data, SERIE);
    fprintf(query3, "%d;%s;%d;%.2f;%s;%d;%.2f\n",
            peli->startYear, peli->name, peli->numVotes, peli->avgRating,
            serie->name, serie->numVotes, serie->avgRating);
}

/* Carga los datos en los archivos respectivos:
 * - data : estructura que contiene todos los datos procesados del archivo dado
 * - query1 : file en el que se vuelcan los datos respectivos al primer query
 * - query2 : file en el que se vuelcan los datos respectivos al segundo query
 * - query3 : file en el que se vuelcan los datos respectivos al tercer query
*/
static void
loadData(imdbADT data, FILE * query1, FILE * query2, FILE * query3)
{
    fputs("year;films;series\n", query1);
    fputs("year;genre;films\n", query2);
    fputs("startYear;film;votesFilm;ratingFilm;serie;votesSerie;ratingSerie\n", query3);
    toBeginYear(data);
    while( nextYear(data) )
    {
        solveQuery1(data, query1);
        solveQuery2(data, query2);
        solveQuery3(data, query3);
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
