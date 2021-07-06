//
// Created by on 5/7/2021.
//

#include "imdbTAD.h"
#include <stdlib.h>
#include <string.h>

#define NO_FIELD 0 // Si el campo no aplica a ese tipo de entrada
#define PELI 1
#define SERIE 2

typedef struct TGenre {
    char * genre;
    size_t cant;  // Cantidad de peliculas del genero
    struct TGenre * tail;
} TGenre;

typedef TGenre * TGenreL;

typedef struct TEntry {
    char * name;  // Nombre de la serie o pelicula
    char type;    // Serie = SERIE y Peli = PELI
    size_t numVotes;
    unsigned startYear; // Año de lanzamiento (peli) o comienzo de emision (serie)
    unsigned endYear;   // Año de fin de emision, o NO_FIELD si no termino o NO_FILED si es peli
    float avgRating;
    unsigned runtimeMin; // Runtime si es peli o NO_FIELD si es serie
    char ** genre;
    unsigned cantGenres;
} TEntry;

typedef struct TYear {
    unsigned year;
    size_t cantPelis;
    size_t cantSeries;
    TEntry peli;  // Peli mas votada
    TEntry serie; // Serie mas votada
    TGenreL firstGenre;  // Lista de generos de peliculas
    TGenreL currentGenre;
    struct TYear * tail;  // Puntero al siguiente año
} TYear;

typedef TYear * TYearL;

typedef struct imdbCDT {
    TYear * firstY;
    TYear * currentY;
} imdbCDT;

imdbADT newDataBase(){
    return calloc(1, sizeof(imdbCDT));
}

static void freeADTGenre(TGenreL list){
    if (list == NULL){
        return;
    }
    freeADTGenre(list->tail);
    free(list->genre);
    free(list);
}

static void freeADTYear(TYearL list){
    if (list == NULL){
        return;
    }
    freeADTYear(list->tail);
    freeADTGenre(list->firstGenre);
    free(list->peli.name);
    free(list->serie.name);
    for (int i = 0; i < list->serie.cantGenres; ++i) {
        free(list->serie.genre[i]);
    }
    free(list->serie.genre);
    for (int i = 0; i < list->peli.cantGenres; ++i) {
        free(list->peli.genre[i]);
    }
    free(list->peli.genre);
    free(list);
}

void freeADT(imdbADT data){
    freeADTYear(data->firstY);
    free(data);
}

static size_t getMostVotesREC(const TYearL list, unsigned year, char type){
    if (list == NULL || list->year < year){
        return 0;
    }
    else if (type == PELI && list->year == year){ //el ano es el indicado y es una pelicula
        return list->peli.numVotes;
    }
    else if (list->year == year){ //el ano es el indicado y es una serie
        return list->serie.numVotes;
    }
    else{
        return getMostVotesREC(list->tail, year, type);
    }
}

size_t getMostVotes(imdbADT data, unsigned year, char type){
    return getMostVotesREC(data->firstY, year, type);
}

TEntry getMostPopular(const imdbADT data, char type){
    if (type == PELI){
        return data->currentY->peli;
    }
    else{
        return data->currentY->serie;
    }
}

/* size_t getAmountPerGenre(const imdbADT data){
    return data->currentY->currentGenre->cant;
}  NO SERIA LO MISMO QUE ITERAR????  */

/*TGenreL updateCantRECGenres(TGenreL list, char * genre) {
    if (list == NULL || strcmp(list->genre, genre) > 0){
        TGenreL new = malloc(sizeof(TGenre));
        new->genre = genre;
        new->cant = 1;
        new->tail = list;
        return new;
    }
    else if (strcmp(list->genre, genre) == 0) {
        list->cant++;
        return list;
    }
    else {
        list->tail = updateCantRECGenres(list->tail, genre);
        return list;
    }
}


void updateCantREC(TYearL list, unsigned year, char type, char * genre){
    if (list == NULL || list->year < year){
        addMov;
        return;
    }
    else if (type == PELI && list->year == year){ //el ano es el indicado y es una pelicula
        list->cantPelis++;
        return;
    }
    else if (type == SERIE && list->year == year){ //el ano es el indicado y es una serie
        list->cantSeries++;
        return;
    }
    else if (type == NO_FIELD && list->year == year){ //updatea la cant de generos
        updateCantRECGenres(list->firstGenre);
        return;
    }
    else{
        updateCantREC(list->tail, year, type);
    }
}

void updateNodes(imdbADT data, unsigned year, char type, char * genre){
    updateCantREC(data->firstY, year, type,genre);
}

void update()*/

void toBeginYear(imdbADT data){
    data->currentY = data->firstY;
}

int hasNextYear(imdbADT data){
    if (data->currentY == NULL){
        return 0;
    }
    return 1;
}

TYearL nextYear(imdbADT data){
    if (hasNextYear(data) == 0){
        // ????
    }
    else{
        TYearL aux = data->currentY;
        data->currentY = data->currentY->tail;
        return aux;
    }
}

void toBeginGenre(imdbADT data){
    data->currentY->currentGenre = data->currentY->firstGenre;
}

int hasNextGenre(imdbADT data){
    if (data->currentY->currentGenre == NULL){
        return 0;
    }
    return 1;
}

TGenreL nextGenre(imdbADT data){
    if (hasNextGenre(data) == 0){
        // ????
    }
    else{
        TGenreL aux = data->currentY->currentGenre;
        data->currentY->currentGenre = data->currentY->currentGenre->tail;
        return aux;
    }
}

