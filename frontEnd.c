#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "frontEnd.h"
#include "imdbTAD.h"
#include "dataTypes.h"
#include "backEnd.h"
#include "errorCodes.h"

#define LINE_MAX 400

#define SEPARADOR ";"
#define SEPARADOR_2 ','

#define OK 0
#define EMPTY_FIELD "\\N"

#define UPDATE_TOKEN token = strtok(NULL, SEPARADOR);

#define CHECK_ENTRY(token, field, type) { if(!strcmp(token, EMPTY_FIELD)) \
                                            entry->field = NO_FIELD;     \
                                          else                            \
                                            entry->field = type(token);  \
                                        };


static char ** loadGenres(char * line, unsigned * cant, int * error)
{
    int size, dim, i;
    char ** genres = NULL, ** aux = NULL;

    for(i=0, size=0, dim=0; line[i]!=0; )
    {
        if(size%BLOCK==0)
        {
            genres = realloc(aux, (size + BLOCK) * sizeof(char *));
            NO_MEM(*error)
            RETURN_IF_ERROR(*error, aux)
            aux = genres;
            size += BLOCK;
        }
        genres[dim++] = copyText(line + i, SEPARADOR_2, error);        // + i cantidad de offset
        RETURN_IF_ERROR(*error, genres)

        for( ; line[i] != SEPARADOR_2 && line[i] != 0 ; i++ ) // Pasa al proximo genero Ej: Drama Comedia => Comedia
            ;

        if( line[i] != 0 )
            i++;
    }
    // Este realloc nunca deberia tirar error porque "corta" lo que sobra
    genres = realloc(genres, dim*sizeof(char *));
    *cant = dim;                //!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    return genres;
}


static int updateEntry(TEntry * entry, char * line, int * error){
    char * token = strtok(line, SEPARADOR);

    if( !strcmp(token, "movie") )      // Tipo de entry
        entry->type = PELI;
    else
        entry->type = SERIE;

    UPDATE_TOKEN
    entry->name = copyText(token, EMPTY, error);        // Nombre de entry
    RETURN_IF_ERROR(*error, *error)

    UPDATE_TOKEN
    if( !strcmp(token, EMPTY_FIELD) )
        return INVALID_YEAR;                   // Verifica si el anio es valido, si no lo eso => corta
    else
        entry->startYear = atoi(token);        // Anio de comienzo

    UPDATE_TOKEN
    CHECK_ENTRY(token, endYear, atoi);

    UPDATE_TOKEN
    unsigned cant;
    entry->genre = loadGenres(token, &cant, error);        // Carga los generos en la lista
    entry->cantGenres = cant;
    RETURN_IF_ERROR(*error, *error)

    UPDATE_TOKEN
    CHECK_ENTRY(token, avgRating, atof);

    UPDATE_TOKEN
    CHECK_ENTRY(token, numVotes, atoi);

    UPDATE_TOKEN
    CHECK_ENTRY(token, runtimeMin, atoi);

    return OK;
}

static void
freeResources(TEntry * entry)
{
    if( entry == NULL )
        return;

    free(entry->name);
    // Si no hubo problemas con memoria
    if( entry->genre != NULL )
    {
        for(int i=0; i<entry->cantGenres; i++)
            if(entry->genre[i] != NULL)         // Verifica que haya allocado
                free(entry->genre[i]);
        free(entry->genre);
    }
}


int readFile(imdbADT data, char * fileName){
    int error = OK; // Lo inicializamos con que esta ok, por ende no hay errores
    FILE * imdbFile;
    imdbFile = fopen(fileName, "r");
    FILE_ERROR(imdbFile, ERROR_DE_FILE);

    char line[LINE_MAX];        // Levanta hasta LINE_MAX caracteres del file

    TEntry * entry = malloc(sizeof(TEntry));
    NO_MEM(error)
    FREE_ADT(error, data)
    RETURN_IF_ERROR(error, INSUFFICIENT_MEM)

    fgets(line, sizeof(line), imdbFile);    // Ignora la primera linea

    while( !error && fgets(line, sizeof(line), imdbFile) ) {
        if(updateEntry(entry, line, &error)!=INVALID_YEAR && error==OK){        // Error se actualiza en updateEntry y luego se verifica
            updateData(data, entry, &error);                    // Solo si el entry es valido se actualiza el ADT

            // Si hubo error liberamos el adt y cortamos el programa
            FREE_ADT(error, data)
            freeResources(entry);
        }
        else
        {
            if(entry->name != NULL)         // Si el primer realloc de copyText no funciona
                free(entry->name);          //!!!!!!!!!!!!!!!
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
    if( fileName == NULL )
        return NULL; // Error

    // Creamos el archivo para escritura, y cargamos los datos.
    FILE * newFile = fopen(fileName, "w");
    return newFile;
}

static void
loadQuery1( TQuery1 data, FILE * query1) {
    fprintf(query1, "%d;%d;%d\n", data.year, data.cantPelis, data.cantSeries);
}

static void
loadQuery2( TQuery2 data, FILE * query2) {
    fprintf(query2, "%d;%d;%s\n", data.year, data.cantPelis, data.genero);
}

static void
loadQuery3(TQuery3 data, FILE * query3, int * error)
{
    if(*error==OK){
        if( data.serie->name != NULL )
        {
            fprintf(query3, "%d;%s;%d;%.1f;%s;%d;%.1f\n",
                    data.peli->startYear, data.peli->name, data.peli->numVotes, data.peli->avgRating,
                    data.serie->name, data.serie->numVotes, data.serie->avgRating);
        }
        else
        {
            fprintf(query3, "%d;%s;%d;%.1f;%s\n",
                    data.peli->startYear, data.peli->name, data.peli->numVotes, data.peli->avgRating,
                    "No hay serie.");
        }
    }
    if(data.peli!=NULL)     // Verifica que en ambos casos haya podido allocar
        free(data.peli);
    if(data.serie!=NULL)
        free(data.serie);
}

static void
writeQueries(imdbADT data, FILE * query1, FILE * query2, FILE * query3)
{
    fputs("year;films;series\n", query1);
    fputs("year;genre;films\n", query2);
    fputs("startYear;film;votesFilm;ratingFilm;serie;votesSerie;ratingSerie\n", query3);
    int hasNextGenre=1, error = OK;
    toBeginYear(data);
    while( hasNextYear(data))
    {
        toBeginG(data);
        loadQuery1(queryOne(data), query1);

        while( hasNextGenre )
            loadQuery2(queryTwo(data,&hasNextGenre), query2);

        TQuery3 aux = queryThree(data, &error);
        loadQuery3(aux, query3, &error);
        RETURN_IF_ERROR(error,)

        hasNextGenre = 1;
        nextYear(data);
    }
    fclose(query3);
    fclose(query2);
    fclose(query1);
}

/* Crea los archivos .csv para las distintas queries y carga en ellos los datos correspondientes */
void
writeData(imdbADT data)
{
    FILE * q1, * q2, * q3; // Punteros a los files en el que se volcaran los datos de las queries
    q1 = createCSV(FNAME_Q1);
    q2 = createCSV(FNAME_Q2);
    q3 = createCSV(FNAME_Q3);

    writeQueries(data, q1, q2, q3);
}
