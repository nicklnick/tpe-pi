
#include <stdlib.h>
#include <string.h>
#include "errorCodes.h"
#include "dataTypes.h"
#include "imdbTAD.h"

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

    // Si no hay memoria disponible, retorna que hay un error de memoria insuficiente
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

    // Proteccion contra el caso en donde el programa encontro un error sin poder asignar nada
    if(list->genre != NULL)
        free(list->genre);
    free(list);
}

static void
freeADTYear(TYearL list)
{
    if( list == NULL )
        return;

    freeADTYear(list->tail);

    // Liberamos los generos de cada ano
    freeADTGenre(list->firstG);

    // Chequeamos que no sean NULL en el caso de que haya habido un error antes de asignar
    // ese espacio de memoria
    if(list->peli.name != NULL)
        free(list->peli.name);

    if(list->serie.name != NULL)
        free(list->serie.name);

    free(list);
}

void freeADT(imdbADT data) {
    if( data == NULL )
        return;

    freeADTYear(data->firstY);
    free(data);
}


// Funciones que interactuan con otras funciones de backEnd

void getAmountCurrY(imdbADT data, unsigned * year, unsigned * cantPelis, unsigned * cantSeries) {
    // Devuelve los parametros pedidos en el query 1
    *year = data->currentY->year;
    *cantPelis = data->currentY->cantPelis;
    *cantSeries = data->currentY->cantSeries;
}

void getAmountG(imdbADT data, unsigned * year, char ** genero, unsigned * cantPelis) {
    // Devuelve los parametros pedidos en el query 2
    *year = data->currentY->year;
    *genero = data->currentY->currentG->genre;
    *cantPelis = data->currentY->currentG->cant;
}

TEntry *
getMostPopular(imdbADT data, char type, int * error)
{
    TEntry * mostVoted = malloc(sizeof(TEntry));
    // Si no se puede asignarse retorna NULL
    NO_MEM(*error)
    RETURN_IF_ERROR(*error, NULL)

    // Se crea una copia de la peli o serie mas popular
    if( type == PELI )
        *mostVoted = data->currentY->peli;
    else // type == SERIE
        *mostVoted = data->currentY->serie;
    return mostVoted;
}

// Funciones para iterar el anio

void toBeginYear(imdbADT data) {
    data->currentY = data->firstY;
}

int hasNextYear(imdbADT data) {
    return data->currentY != NULL;
}

void nextYear(imdbADT data) {
    if( hasNextYear(data) == 0 )
        return;

    // Hace que currentY apunte al siguiente anio
    data->currentY = data->currentY->tail;
}

// Funciones para iterar el genero

void toBeginG(imdbADT data) {
    data->currentY->currentG = data->currentY->firstG;
}

int hasNextG(imdbADT data) {
    return data->currentY->currentG != NULL;
}

void nextG(imdbADT data){
    if( hasNextG(data) == 0 )
        return;

    // Hace que currentG apunte al siguiente genero
    data->currentY->currentG = data->currentY->currentG->tail;
}


// Funciones que interactuan con funciones de frontEnd

static TGenreL
addGenres(TGenreL list, char ** genres, unsigned cantGenres, int * error)
{
    int c;
    // Chequea que queden generos para agregar
    if( !cantGenres )
        return list;
    if( list == NULL || (c =strcmp(list->genre, *genres)) > 0 )
    {
        TGenreL newGenre = malloc(sizeof(TGenre));
        // Si no se puede asignarse retorna NULL
        NO_MEM(*error)
        RETURN_IF_ERROR(*error, NULL)

        newGenre->genre = copyText(*genres,EMPTY, error);
        // Si no se puede asignar la cantidad necesaria, se retorna la direccion
        // con la cantidad que ha sido posible reservar
        RETURN_IF_ERROR(*error, newGenre)

        newGenre->cant = 1;
        newGenre->tail = addGenres(list, genres + 1, cantGenres - 1, error);
        return newGenre;
    }
    else if( c == 0 ) // Ya existe el genero
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
    // Si no se puede asignar se retorna NULL
    NO_MEM(*error)
    RETURN_IF_ERROR(*error, NULL)

    newYear->year = entry->startYear;
    if (entry->type == PELI)
    {
        newYear->cantPelis++;
        newYear->peli = *entry;
        newYear->peli.genre = NULL;
        newYear->peli.name = copyText(entry->name,EMPTY, error);
        // Retorna la direccion con la cnatidad que se ha podido reservar
        RETURN_IF_ERROR(*error, newYear)
    }
    else // entry->type == SERIE
    {
        newYear->cantSeries++;
        newYear->serie = *entry;
        newYear->serie.genre = NULL;
        newYear->serie.name = copyText(entry->name,EMPTY, error);
        // Retorna la direccion con la cnatidad que se ha podido reservar
        RETURN_IF_ERROR(*error, newYear)
    }
    newYear->firstG = addGenres(newYear->firstG, entry->genre, entry->cantGenres, error);
    return newYear;
}

static void
updateMostPopular(TYearL current, TEntry * entry, int * error)
{
    if( entry->type == PELI )
    {
        if (entry->numVotes > current->peli.numVotes)
        {
            // Si se cambia el mostPopular, se tiene que liberar el anterior
            if(current->peli.name != NULL)
                free(current->peli.name);
            current->peli = *entry;
            current->peli.genre = NULL; // El genero no se usa
            current->peli.name = copyText(entry->name,EMPTY, error);
            // Retorna la direccion con la cnatidad que se ha podido reservar
            RETURN_IF_ERROR(*error,)
        }
    }
    else //entry->type == SERIE
    {
        if (entry->numVotes > current->serie.numVotes)
        {
            // Si se cambia el mostPopular, se tiene que liberar el anterior
            if(current->serie.name != NULL)
                free(current->serie.name);
            current->serie = *entry;
            current->serie.genre = NULL; // El genero no se usa
            current->serie.name = copyText(entry->name,EMPTY, error);
            // Retorna la direccion con la cnatidad que se ha podido reservar
            RETURN_IF_ERROR(*error,)
        }
    }
}


static void updateCant(TYearL current, TEntry * entry, int * error)
{
    if(entry->type == PELI)
        current->cantPelis++;
    else
        current->cantSeries++;

    // Agrega los generos
    current->firstG = addGenres(current->firstG,entry->genre,entry->cantGenres, error);
}


static TYearL
updateYear(TYearL firstYear, TEntry * entry, int * error)
{
    // Caso primer anio
   if( firstYear == NULL )
   {
        TYearL newYear = createYear(entry, error);
        return newYear;
    }

    TYearL current = firstYear;
    // Se usa un prev para encadenar la lista
    TYearL prev = NULL;
    int updatedList = 0;

    while( !updatedList )
    {
        // Caso en donde no existe el anio
        if( current == NULL || current->year < entry->startYear )
        {
            TYearL newYear = createYear(entry, error);
            RETURN_IF_ERROR(*error, newYear)

            newYear->tail = current;
            // Caso en donde se debe reemplazar el primer anio
            if( prev == NULL )
                return newYear;

            prev->tail = newYear;
            updatedList = 1;
        }
        // Caso sigue buscando el anio
        else if(current->year > entry->startYear)
        {
            prev = current;
            current = current->tail;
        }
        //Caso ya existe el anio
        else
        {
            updateMostPopular(current, entry,error);
            updateCant(current, entry, error);
            updatedList = 1;
        }
    }

    return firstYear;
}

void updateData(imdbADT data, TEntry * entry, int * error) {
    data->firstY = updateYear(data->firstY, entry, error);
    // CHECK_ERROR(error, );  /* No hace falta porque sale de todos modos */
}
