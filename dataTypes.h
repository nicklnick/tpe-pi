#ifndef TPE_FINAL_DATATYPES_H
#define TPE_FINAL_DATATYPES_H

#include <stdlib.h>
#include "errorCodes.h"

/* Tipo de dato que tiene todos los datos de una serie o pelicula */
typedef struct TEntry {
    char * name;  // Nombre de la serie o pelicula
    char type;    // Serie = SERIE y Peli = PELI
    unsigned numVotes;
    unsigned startYear; // Año de lanzamiento (peli) o comienzo de emision (serie)
    unsigned endYear;   // Año de fin de emision, o NO_FIELD si sigue en emision o si es peli
    float avgRating;
    unsigned runtimeMin; // Runtime si es peli o NO_FIELD si es serie
    char ** genre;
    unsigned cantGenres;
} TEntry;

/* Estructura utilizada para devolver lo correspondiente al query1 */
typedef struct TQuery1 {
    unsigned year;
    unsigned cantPelis;
    unsigned cantSeries;
} TQuery1;

/* Estructura utilizada para devolver lo correspondiente al query2 */
typedef struct TQuery2 {
    unsigned year;
    unsigned cantPelis;
    char * genero;
} TQuery2;

/* Estructura utilizada para devolver lo correspondiente al query3 */
typedef struct TQuery3 {
    TEntry * peli;
    TEntry * serie;
} TQuery3;

/* Identificadores */
#define PELI 1
#define SERIE 2
#define NO_FIELD 0
/**/

#define EMPTY 0             // Si en copyText no se necesita la funcionalidad de SEPARADOR
#define BLOCK 10

/* Copia hasta '\0' o hasta el caracter c */
static char * copyText(const char * text, char c, int * error)
{
    int i;
    char * new = NULL, * aux = NULL;

    for( i = 0 ; text[i] != 0 && text[i] != c; i++ )
    {
        if( i % BLOCK == 0 )
        {
            new = realloc(aux, (i + BLOCK) * sizeof(char));
            NO_MEM(*error)
            RETURN_IF_ERROR(*error, aux)
            aux = new;
        }
        new[i] = text[i];
    }
    // Este realloc nunca deberia tirar error porque "corta" lo que sobra
    new = realloc(new, (i + 1) * sizeof(char));
    new[i]=0;
    return new;
}

#endif //TPE_FINAL_DATATYPES_H
