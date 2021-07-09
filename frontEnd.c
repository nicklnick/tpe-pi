#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "frontEnd.h"
#include "imdbTAD.h"
#include "dataTypes.h"
#include "backEnd.h"

#define LINE_MAX 400

#define SEPARADOR ";"
#define SEPARADOR_2 ','

#define OK 0
#define ERROR_DE_FILE 5
#define INVALID_YEAR 3

#define EMPTY_FIELD "\\N"

#define UPDATE_TOKEN token = strtok(NULL, SEPARADOR);

#define CHECK_ENTRY(token, field, type){ if(!strcmp(token, EMPTY_FIELD))\
                                         entry->field = NO_FIELD;       \
                                       else                             \
                                         entry->field = type(token);    \
};


static char ** loadGenres(char * line, unsigned * cant){
    int size, dim, i;
    char ** genres = NULL;

    for(i=0, size=0, dim=0; line[i]!=0; ){

        if(size%BLOCK==0) {
            genres = realloc(genres, (size + BLOCK) * sizeof(char *));              //!!!!!!!!
            size += BLOCK;
        }
        genres[dim++] = copyText(line + i, SEPARADOR_2);        // + i cantidad de offset

        for(;line[i]!=SEPARADOR_2 && line[i]!=0; i++);      // Pasa al proximo genero Ej: Drama Comedia => Comedia
        if(line[i]!=0){
            i++;
        }
    }
    genres = realloc(genres, dim*sizeof(char *));                                  //!!!!!!!!
    *cant = dim;
    return genres;
}


static int updateEntry(TEntry * entry, char * line){
    char * token = strtok(line, SEPARADOR);

    if(!strcmp(token, "movie"))      // Tipo de entry
        entry->type = PELI;
    else
        entry->type = SERIE;

    UPDATE_TOKEN
    entry->name = copyText(token, 0);        // Nombre de entry

    UPDATE_TOKEN
    if(!strcmp(token, EMPTY_FIELD))
        return INVALID_YEAR;                   // Verifica si el anio es valido, si no lo eso => corta
    else
        entry->startYear = atoi(token);        // Anio de comienzo

    UPDATE_TOKEN
    CHECK_ENTRY(token, endYear, atoi);

    UPDATE_TOKEN
    unsigned cant;
    entry->genre = loadGenres(token, &cant);        // Carga los generos en la lista
    entry->cantGenres = cant;

    UPDATE_TOKEN
    CHECK_ENTRY(token, avgRating, atof);

    UPDATE_TOKEN
    CHECK_ENTRY(token, numVotes, atoi);

    UPDATE_TOKEN
    CHECK_ENTRY(token, runtimeMin, atoi);

    return OK;
}

static void freeResources(TEntry * entry){
    if(entry==NULL)
        return;
    free(entry->name);
    for(int i=0; i<entry->cantGenres; i++){
        free(entry->genre[i]);
    }
    free(entry->genre);
}


int readFile(imdbADT data, char * fileName){
    FILE * imdbFile;
    imdbFile = fopen(fileName, "r");

    if(imdbFile == NULL){
        return ERROR_DE_FILE;
    }

    char line[LINE_MAX];        // Levanta hasta LINE_MAX caracteres del file

    TEntry * entry = malloc(sizeof(TEntry));                                            //!!!!!!!!

    fgets(line, sizeof(line), imdbFile);    // Ignora la primera linea

    while(fgets(line, sizeof(line), imdbFile)) {
        if(updateEntry(entry, line)!=INVALID_YEAR){
            updateData(data, entry);                    // Solo si el entry es valido se actualiza el ADT
            freeResources(entry);
        }
        else{
            free(entry->name);
        }
    }

    free(entry);
    fclose(imdbFile);       // Cierre final del file
    return OK;
}

static FILE *
createCSV(const char * fileName)
{
    // Si no me pasan el nombre del archivo
    if( fileName == NULL ) return NULL; // Error

    // Creamos el archivo para escritura, y cargamos los datos.
    FILE * newFile = fopen(fileName, "w");
    return newFile;
}

static void
loadQuery1( TQuery1 data, FILE * query1)
{
    fprintf(query1, "%d;%d;%d\n", data.year, data.cantPelis, data.cantSeries); // Para imprimir size_t tiene que ser %zu
}

static void
loadQuery2( TQuery2 data, FILE * query2)
{
    fprintf(query2, "%d;%d;%s\n", data.year, data.cantPelis, data.genero);
}

static void
loadQuery3(TQuery3 data, FILE * query3)
{
    if( data.serie->name != NULL )
    {
        fprintf(query3, "%d;%s;%d;%.2f;%s;%d;%.2f\n",
                data.peli->startYear, data.peli->name, data.peli->numVotes, data.peli->avgRating,
                data.serie->name, data.serie->numVotes, data.serie->avgRating);
    }
    else
    {
        fprintf(query3, "%d;%s;%d;%.2f;%s\n",
                data.peli->startYear, data.peli->name, data.peli->numVotes, data.peli->avgRating,
                "No hay serie.");
    }
    free(data.peli);
    free(data.serie);
}

static void
writeQueries(imdbADT data, FILE * query1, FILE * query2, FILE * query3)
{
    fputs("year;films;series\n", query1);
    fputs("year;genre;films\n", query2);
    fputs("startYear;film;votesFilm;ratingFilm;serie;votesSerie;ratingSerie\n", query3);
    int flag=1;
    toBeginYear(data);
    while( hasNextYear(data))
    {
        toBeginG(data);
        loadQuery1(queryOne(data), query1);

        while(flag){
            loadQuery2(queryTwo(data,&flag), query2);
        }

        loadQuery3(queryThree(data), query3);

        flag=1;
        nextYear(data);
    }
    fclose(query3);
    fclose(query2);
    fclose(query1);
}

/*
 * Crea los archivos .csv para las distintas queries y carga en ellos los datos correspondientes
 */
void
writeData(imdbADT data)
{
    FILE * q1, * q2, * q3; // Punteros a los files en el que se volcaran los datos de las queries
    q1 = createCSV(FNAME_Q1);
    q2 = createCSV(FNAME_Q2);
    q3 = createCSV(FNAME_Q3);

    writeQueries(data, q1, q2, q3);
}
