
#include <stdlib.h>
#include <string.h>
#include "errorCodes.h"
#include "dataTypes.h"
#include "imdbTAD.h"

#define NO_FIELD 0 // Si el campo no aplica a ese tipo de entrada
#define PELI 1
#define SERIE 2

#define BLOCK 10

typedef struct TGenre {
    char * genre;
    unsigned cant;  // Cantidad de peliculas del genero
    struct TGenre * tail;
} TGenre;

typedef TGenre * TGenreL;

typedef struct TYear {
    unsigned year;
    unsigned cantPelis;
    unsigned cantSeries;
    TEntry peli;  // Peli mas votada
    TEntry serie; // Serie mas votada
    TGenreL firstG;  // Lista de generos de peliculas
    TGenreL currentG;
    struct TYear * tail;  // Puntero al siguiente año
} TYear;

typedef TYear * TYearL;

typedef struct imdbCDT {
    TYearL firstY;
    TYearL currentY;
} imdbCDT;



imdbADT newDataBase(int * error){
    imdbADT new = calloc(1, sizeof(imdbCDT));

    //!!!!!!!!!!!
    NO_MEM(*error)
    RETURN_IF_ERROR(*error, NULL)

    return new;
}

static void
freeADTGenre(TGenreL list)
{
    if( list == NULL )
        return;

    freeADTGenre(list->tail);
    if(list->genre != NULL)         // Caso en que no se pudo allocar nada
        free(list->genre);
    free(list);
}

static void
freeADTYear(TYearL list)
{
    if( list == NULL )
        return;

    freeADTYear(list->tail);
    freeADTGenre(list->firstG);

    if(list->peli.name != NULL)
        free(list->peli.name);

    if(list->serie.name != NULL)
        free(list->serie.name);

    free(list);
}

void freeADT(imdbADT data) {
    if( data == NULL )
        return;   //!!!!!!!!!!!!!!!!!!

    freeADTYear(data->firstY);
    free(data);
}


//BACK

void getAmountCurrY(imdbADT data, unsigned * year, unsigned * cantPelis, unsigned * cantSeries) {
    *year = data->currentY->year;
    *cantPelis = data->currentY->cantPelis;
    *cantSeries = data->currentY->cantSeries;
}

void getAmountG(imdbADT data, unsigned * year, char ** genero, unsigned * cantPelis) {
    *year = data->currentY->year;
    *genero = data->currentY->currentG->genre;
    *cantPelis = data->currentY->currentG->cant;
}

TEntry *
getMostPopular(imdbADT data, char type, int * error)
{
    TEntry * mostVoted = malloc(sizeof(TEntry));
    NO_MEM(*error)
    RETURN_IF_ERROR(*error, NULL)

    if( type == PELI )
        *mostVoted = data->currentY->peli;
    else // type == SERIE
        *mostVoted = data->currentY->serie;
    return mostVoted;
}

void toBeginYear(imdbADT data) {
    data->currentY = data->firstY;
}

int hasNextYear(imdbADT data) {
    return data->currentY != NULL;
}

void nextYear(imdbADT data) {
    if( hasNextYear(data) == 0 )
        return;

    data->currentY = data->currentY->tail;
}

void toBeginG(imdbADT data) {
    data->currentY->currentG = data->currentY->firstG;
}

int hasNextG(imdbADT data) {
    return data->currentY->currentG != NULL;
}

void nextG(imdbADT data){
    if( hasNextG(data) == 0 )
        return;

    data->currentY->currentG = data->currentY->currentG->tail;
}

//FRONT
static TGenreL
addGenres(TGenreL list, char ** genres, unsigned cantGenres, int * error)
{
    int c;
    if( !cantGenres )
        return list;
    if( list == NULL || (c =strcmp(list->genre, *genres)) > 0 )
    {
        TGenreL newGenre = malloc(sizeof(TGenre));
        NO_MEM(*error)
        RETURN_IF_ERROR(*error, NULL)

        newGenre->genre = copyText(*genres,EMPTY, error);
        RETURN_IF_ERROR(*error, newGenre)

        newGenre->cant = 1;
        newGenre->tail = addGenres(list, genres + 1, cantGenres - 1, error);
        return newGenre;
    }
    else if( c == 0 )
    {
        list->cant++;
        list->tail = addGenres(list->tail, genres + 1, cantGenres - 1, error);
        return list;
    }
    list->tail = addGenres(list->tail, genres,  cantGenres, error);
    return list;
}


static TYearL
createYear(TEntry * entry, int * error)
{
    TYearL newYear = calloc(1, sizeof(TYear));
    NO_MEM(*error)
    RETURN_IF_ERROR(*error, NULL)

    newYear->year = entry->startYear;
    if (entry->type == PELI)
    {
        newYear->cantPelis++;
        newYear->peli = *entry;
        newYear->peli.genre = NULL;
        newYear->peli.name = copyText(entry->name,EMPTY, error);
        RETURN_IF_ERROR(*error, newYear)
    }
    else //entry->type == SERIE
    {
        newYear->cantSeries++;
        newYear->serie = *entry;
        newYear->serie.genre = NULL;
        newYear->serie.name = copyText(entry->name,EMPTY, error);
        RETURN_IF_ERROR(*error, newYear)
    }
    newYear->firstG = addGenres(newYear->firstG, entry->genre, entry->cantGenres, error);
    return newYear;
}

//UPDATE MOST VOTED
static void
updateMostPopular(TYearL current, TEntry * entry, int * error)
{
    if (entry->numVotes > current->peli.numVotes)
    {
        if(current->peli.name != NULL)                         //Si se cambia el mostPopular, se tiene que liberar el anterior
            free(current->peli.name);
        current->peli = *entry;
        current->peli.genre = NULL;
        current->peli.name = copyText(entry->name,EMPTY, error);
        RETURN_IF_ERROR(*error,)
    }
    else //entry->type == SERIE
    {
        if (entry->numVotes > current->serie.numVotes) {
            if(current->serie.name != NULL)                 //Si se cambia el mostPopular, se tiene que liberar el anterior
                free(current->serie.name);
            current->serie = *entry;
            current->serie.genre = NULL;
            current->serie.name = copyText(entry->name,EMPTY, error);
            RETURN_IF_ERROR(*error,)
        }
    }
}

// UPDATE CANTIDADES
static void updateCant(TYearL current, TEntry * entry, int * error)
{
    if(entry->type == PELI)
        current->cantPelis++;
    else
        current->cantSeries++;

    current->firstG = addGenres(current->firstG,entry->genre,entry->cantGenres, error);
    // CHECK_ERROR(error, );  /* No hace falta porque sale de todos modos */
}


static TYearL
updateYear(TYearL firstYear, TEntry * entry, int * error)
{
    //CASO PRIMER ANIO
   if( firstYear == NULL )
   {
        TYearL newYear = createYear(entry, error);
        return newYear;
    }

    TYearL current = firstYear;
    TYearL prev = NULL;
    int updatedList = 0;

    while( !updatedList )
    {
        //CASO NO EXISTE
        if( current == NULL || current->year < entry->startYear )
        {
            TYearL newYear = createYear(entry, error);
            RETURN_IF_ERROR(*error, newYear)

            newYear->tail = current;
            //CASO REMPLAZAR EL PRIMER ANIO
            if( prev == NULL )
                return newYear;

            prev->tail = newYear;
            updatedList = 1;
        }
        //CASO SIGUE BUSCANDO
        else if(current->year > entry->startYear)
        {
            prev = current;
            current = current->tail;
        }
        //CASO YA EXISTE
        else
        {
            updateMostPopular(current, entry,error);
            updateCant(current, entry, error);
            updatedList = 1;
            // CHECK_ERROR(error, );  /* No hace falta porque sale de todos modos */
        }
    }

    return firstYear;
}

void updateData(imdbADT data, TEntry * entry, int * error) {
    data->firstY = updateYear(data->firstY, entry, error);
    // CHECK_ERROR(error, );  /* No hace falta porque sale de todos modos */
}
