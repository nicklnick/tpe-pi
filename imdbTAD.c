
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

static char * copyText(const char * text){          // Copia hasta \0 o hasta el caracter c
    int i;
    char * new = NULL;

    for( i = 0 ; text[i]!=0 ; i++ )
    {
        if( i % BLOCK == 0 )
            new = realloc(new, (i+BLOCK)*sizeof(char));
        new[i] = text[i];
    }

    new = realloc(new, (i+1) * sizeof(char));
    new[i] = 0;
    return new;
}


imdbADT newDataBase(){
    return calloc(1, sizeof(imdbCDT));
}

static void
freeADTGenre(TGenreL list)
{
    if( list == NULL )
        return;

    freeADTGenre(list->tail);
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

    free(list->peli.name);
    free(list->serie.name);

    // Liberamos los generos de las series
    for(int i = 0; i < list->serie.cantGenres; i++)
        free(list->serie.genre[i]);
    free(list->serie.genre);

    // Liberamos los generos de las pelis
    for(int i = 0; i < list->peli.cantGenres; i++)
        free(list->peli.genre[i]);
    free(list->peli.genre);

    free(list);
}

void freeADT(imdbADT data) {
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
getMostPopular(imdbADT data, char type)
{
    TEntry * mostVoted = malloc(sizeof(TEntry));

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
    return data->currentY->tail != NULL;
}

int nextYear(imdbADT data) {
    if( hasNextYear(data) == 0 )
        return 0;

    data->currentY = data->currentY->tail;
    return 1;
}

void toBeginG(imdbADT data) {
    data->currentY->currentG = data->currentY->firstG;
}

int hasNextG(imdbADT data) {
    return data->currentY->currentG->tail != NULL;
}

int nextG(imdbADT data){
    if( hasNextG(data) == 0 )
        return 0;

    data->currentY->currentG = data->currentY->currentG->tail;
    return 1;
}

//FRONT
static TGenreL
addGenres(TGenreL list, char ** genres, unsigned cantGenres)
{
    int c;
    if( !cantGenres )
        return list;
    if( list == NULL || (c =strcmp(list->genre, *genres)) > 0 )
    {
        TGenreL newGenre = malloc(sizeof(TGenre));
        newGenre->genre = copyText(*genres);
        newGenre->cant = 1;
        newGenre->tail = addGenres(list, genres + 1, cantGenres - 1);
        return newGenre;
    }
    else if( c == 0 )
    {
        list->cant++;
        list->tail = addGenres(list->tail, genres + 1, cantGenres - 1);
        return list;
    }
    list->tail = addGenres(list->tail, genres,  cantGenres);
    return list;
}

static void
createNewYear(TYearL prevList, TEntry * entry) {

    TYearL newYear = calloc(1, sizeof(TYear));
    newYear->year = entry->startYear;
    if (entry->type == PELI) {
        newYear->cantPelis++;
        newYear->peli = *entry;
        newYear->peli.name = copyText(entry->name);
    }
    else //entry->type == SERIE8
    {
        newYear->cantSeries++;
        newYear->serie = *entry;
        newYear->serie.name = copyText(entry->name);
    }

    newYear->firstG = addGenres(newYear->firstG, entry->genre, entry->cantGenres);

    // Caso en donde la lista esta vacia
    if (prevList == NULL){
        newYear->tail = NULL;
        return;
    }
    newYear->tail = prevList->tail;
    prevList->tail = newYear;
}



static void updateExistingData(TYearL list, TEntry * entry) {
    if(entry->type == PELI)
        list->cantPelis++;
    else
        list->cantSeries++;
    list->firstG = addGenres(list->firstG,entry->genre,entry->cantGenres);
}

static TYearL checkExisting(TYearL list, TEntry * entry, char * flag) {
    TYearL auxList = list;
    TYearL prevAuxList = NULL;

    while (auxList != NULL && auxList->year >= entry->startYear) {
        if (auxList->year == entry->startYear) {
            *flag = 1;
            return auxList;
        }
        prevAuxList = auxList;
        auxList = auxList->tail;
    }
    return prevAuxList; // no encontro que exista
}


static void updateMostVoted(TYearL list, TEntry * entry) {
    TYearL auxList = list;

    while (auxList != NULL && auxList->year >= entry->startYear) {
        if (auxList->year == entry->startYear) {
            if (entry->type == PELI) {
                if (entry->numVotes > auxList->peli.numVotes) {
                    auxList->peli = *entry;
                    auxList->peli.name = copyText(entry->name);
                }

            }
            else {  //entry->type == SERIE
                if (entry->numVotes > auxList->serie.numVotes){
                    auxList->serie = *entry;
                    auxList->serie.name = copyText(entry->name);
                }
            }
        }
        auxList = auxList->tail;
    }
}



void updateData(imdbADT data, TEntry * entry){
    TYearL c;
    // Chequea si existe el año, si no existe lo crea
    char found = 0; // se usa para conocer si checkExisting encontro el anio creado
    c = checkExisting(data->firstY, entry, &found);
    if ( !found ) {
        createNewYear(c,entry);
        return;
    }
     // Si el año ya existia
    updateExistingData(c, entry);
    updateMostVoted(data->firstY ,entry);
}
