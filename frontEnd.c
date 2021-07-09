#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "frontEnd.h"
#include "imdbTAD.h"
#include "dataTypes.h"
#include "backEnd.h"
#include "errorCodes.h"

#define LINE_MAX 400            // Maxima cantidad de caracteres para levantar del file

#define SEPARADOR ";"
#define SEPARADOR_2 ','

#define EMPTY_FIELD "\\N"
#define STR_PELI "movie"

#define UPDATE_TOKEN token = strtok(NULL, SEPARADOR);

// Se fija que el dato no sea \N
#define CHECK_ENTRY(token, field, type) { if(!strcmp(token, EMPTY_FIELD)) \
                                            entry->field = NO_FIELD;      \
                                          else                            \
                                            entry->field = type(token);   \
                                        };

static char ** loadGenres(char * line, unsigned * cant, int * error)
{
    int size, dim, i;
    char ** genres = NULL, ** aux = NULL;

    // Crea un vector dinamico y completa con los generos
    for(i=0, size=0, dim=0; line[i]!=0; )
    {
        if(size%BLOCK==0)
        {
            /* Se tiene que actualizar por si hay corte por falta de memoria */
            /* asi puede liberar los generos que si se pudieron alocar.      */
            genres = realloc(aux, (size + BLOCK) * sizeof(char *));
            *cant = dim;
            NO_MEM(*error)
            RETURN_IF_ERROR(*error, aux)
            aux = genres;
            size += BLOCK;
        }
        genres[dim++] = copyText(line + i, SEPARADOR_2, error);
        RETURN_IF_ERROR(*error, genres)

        for( ; line[i] != SEPARADOR_2 && line[i] != 0 ; i++ );  // Pasa al proximo genero Ej: Drama Comedia => Comedia

        // Se asegura que el line que se le pasa no empiece con espacio Ej: " Comedia" => "Comedia"
        if( line[i] != 0 )
            i++;
    }
    // Este realloc nunca deberia tirar error porque "corta" lo que sobra
    genres = realloc(genres, dim*sizeof(char *));
    *cant = dim;
    return genres;
}

/* Se asume que atoi y atof no van a dar valores inesperados, pues cliente asegura que  */
/* la entrada (imdbv3.csv), es correcta.                                                */

static int updateEntry(TEntry * entry, char * line, int * error)
{
    char * token = strtok(line, SEPARADOR);

    if( !strcmp(token, STR_PELI) )
        entry->type = PELI;
    else
        entry->type = SERIE;

    UPDATE_TOKEN
    entry->name = copyText(token, EMPTY, error);
    RETURN_IF_ERROR(*error, *error)         // No se puedo alocar suficiente memoria

    UPDATE_TOKEN
    if( !strcmp(token, EMPTY_FIELD) )
        return INVALID_YEAR;                   // Verifica si el anio es valido, si no lo eso => corta
    else
        entry->startYear = atoi(token);

    UPDATE_TOKEN
    CHECK_ENTRY(token, endYear, atoi);

    UPDATE_TOKEN
    unsigned cant;
    entry->genre = loadGenres(token, &cant, error);         // Carga el vector dinamico de generos
    entry->cantGenres = cant;
    RETURN_IF_ERROR(*error, *error)        // No se puedo alocar suficiente memoria

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


void readFile(imdbADT data, char * fileName){
    int error = OK;                         // Lo inicializamos con que esta ok, por ende no hay errores

    FILE * imdbFile;
    imdbFile = fopen(fileName, "r");
    ABORT_IF_ERROR(error, MSG_FILE)         // Aborta por file no valido

    char line[LINE_MAX];                    // Levanta hasta LINE_MAX caracteres del file

    TEntry * entry = malloc(sizeof(TEntry));
    NO_MEM(error)
    FREE_ADT(error, data)
    ABORT_IF_ERROR(error, MSG_MEM)          // Aborta por falta de memoria

    fgets(line, sizeof(line), imdbFile);    // Ignora la primera linea

    while( !error && fgets(line, sizeof(line), imdbFile) )
    {
        // Error se actualiza en updateEntry y luego se verifica. Solo si el entry es valido se actualiza el ADT
        if(updateEntry(entry, line, &error)!=INVALID_YEAR && error==OK)
        {
            updateData(data, entry, &error);

            // Si hubo error liberamos el adt y cortamos el programa
            FREE_ADT(error, data)
            ABORT_IF_ERROR(error, MSG_MEM)
            freeResources(entry);
        }
        else
        {
            if(entry->name != NULL)         // Se fija si el primer realloc de copyText no funciona
                free(entry->name);
        }
    }

    free(entry);
    fclose(imdbFile);
}

static FILE *
createCSV(const char * fileName)
{
    // Creamos el archivo para escritura, y cargamos los datos.
    FILE * newFile = fopen(fileName, "w");
    return newFile;
}

static void
loadQuery1( TQuery1 data, FILE * query1)
{
    fprintf(query1, "%d;%d;%d\n", data.year, data.cantPelis, data.cantSeries);
}

static void
loadQuery2( TQuery2 data, FILE * query2)
{
    fprintf(query2, "%d;%d;%s\n", data.year, data.cantPelis, data.genero);
}

static void
loadQuery3(TQuery3 data, FILE * query3, int * error)
{
    // error es distinto a OK si no se puedo alocar memoria al usar queryThree, que a su vez lo pasa de getMostPopular
    if(*error==OK)
    {
        if( data.serie->name != NULL )
        {
            fprintf(query3, "%d;%s;%d;%.1f;%s;%d;%.1f\n",
                    data.peli->startYear, data.peli->name, data.peli->numVotes, data.peli->avgRating,
                    data.serie->name, data.serie->numVotes, data.serie->avgRating);
        }
        else        // Caso en cual año no tiene series
        {
            fprintf(query3, "%d;%s;%d;%.1f;%s\n",
                    data.peli->startYear, data.peli->name, data.peli->numVotes, data.peli->avgRating,
                    "No hay serie.");
        }
    }
    // Verifica que en ambos casos haya podido allocar
    if(data.peli!=NULL)
        free(data.peli);
    if(data.serie!=NULL)
        free(data.serie);
}

static void
writeQueries(imdbADT data, FILE * query1, FILE * query2, FILE * query3)
{
    fputs(HEADER_Q1, query1);
    fputs(HEADER_Q2, query2);
    fputs(HEADER_Q3, query3);
    int hasNextGenre=1, error = OK;

    toBeginYear(data);                      // Empieza el iterador de los años
    while( hasNextYear(data))
    {
        loadQuery1(queryOne(data), query1);

        toBeginG(data);                     // Empieza el iterador de los generos para el año actual

        //La variable hasNextGenre se actualiza con cada llamado de queryTwo
        while( hasNextGenre )
            loadQuery2(queryTwo(data,&hasNextGenre), query2);

        TQuery3 aux = queryThree(data, &error);
        loadQuery3(aux, query3, &error);
        ABORT_IF_ERROR(error, MSG_MEM)

        hasNextGenre = 1;               // Se resetea la variable para el proximo año
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
    // Punteros a los files en el que se volcaran los datos de las queries
    FILE * q1, * q2, * q3;
    q1 = createCSV(FNAME_Q1);
    q2 = createCSV(FNAME_Q2);
    q3 = createCSV(FNAME_Q3);

    writeQueries(data, q1, q2, q3);
}
