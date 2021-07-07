//
// Created by on 5/7/2021.
//

#include "imdbTAD.h"
#include <dataType.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define NO_FIELD 0 // Si el campo no aplica a ese tipo de entrada
#define PELI 1
#define SERIE 2

typedef struct TGenre {
    char * genre;
    size_t cant;  // Cantidad de peliculas del genero
    struct TGenre * tail;
} TGenre;

typedef TGenre * TGenreL;

typedef struct TYear {
    unsigned year;
    size_t cantPelis;
    size_t cantSeries;
    TEntry * peli;  // Peli mas votada
    size_t votosPeli;
    TEntry * serie; // Serie mas votada
    size_t votosSerie;
    TGenreL firstG;  // Lista de generos de peliculas
    TGenreL currentG;
    struct TYear * tail;  // Puntero al siguiente año
} TYear;

typedef TYear * TYearL;

typedef struct imdbCDT {
    TYearL firstY;
    TYearL currentY;
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
    freeADTGenre(list->firstG);
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


//BACK

void getAmountCurrY(imdbADT data, size_t * cantPelis, size_t * cantSeries, unsigned * year){
    *year = data->currentY->year;
    *cantPelis = data->currentY->cantPelis;
    *cantSeries = data->currentY->cantSeries;
}

void getAmountG(imdbADT data, char * genero, size_t * cantPelis){
    genero = data->currentY->currentG->genre;
    *cantPelis = data->currentY->currentG->cant;
}

TEntry getMostPopular(imdbADT data, char type){
    TEntry mostVoted = malloc(sizeof(TEntry));

    if (type == PELI){
        mostVoted.name = malloc(strlen(data->currentY->peli->name) + 1); //para que sea una copia verdadera
        strcpy(mostVoted.name, data->currentY->peli->name);
        mostVoted.type = PELI;
        mostVoted.startYear = data->currentY;
        mostVoted.endYear = NO_FIELD;
        mostVoted.avgRating = data->currentY->peli->avgRating;
        mostVoted.runtimeMin = data->currentY->peli->runtimeMin;
        mostVoted.cantGenres = data->currentY->peli->cantGenres;
        mostVoted.genre = malloc(sizeof(char*) * mostVoted.cantGenres);
        for (int i = 0; i < mostVoted.cantGenres; ++i) {
            mostVoted.genre[i] = malloc(strlen(data->currentY->peli->genre[i]) + 1); //para que sea una copia verdadera
            strcpy(mostVoted.genre[i], data->currentY->peli->genre[i]);
        }
    }
    else{ //type == SERIE
        mostVoted.name = malloc(strlen(data->currentY->serie->name) + 1); //para que sea una copia verdadera
        strcpy(mostVoted.name, data->currentY->serie->name);
        mostVoted.type = SERIE;
        mostVoted.startYear = data->currentY;
        mostVoted.endYear = data->currentY->serie->endYear;
        mostVoted.avgRating = data->currentY->serie->avgRating;
        mostVoted.runtimeMin = NO_FIELD;
        mostVoted.cantGenres = data->currentY->serie->cantGenres;
        mostVoted.genre = malloc(sizeof(char*) * mostVoted.cantGenres);
        for (int i = 0; i < mostVoted.cantGenres; ++i) {
            mostVoted.genre[i] = malloc(strlen(data->currentY->serie->genre[i]) + 1); //para que sea una copia verdadera
            strcpy(mostVoted.genre[i], data->currentY->serie->genre[i]);
        }
    }
    return mostVoted;
}


void toBeginYear(imdbADT data){
    data->currentY = data->firstY;
}

int hasNextYear(imdbADT data){
    return data->currentY != NULL;
}

int nextYear(imdbADT data){
    if (hasNextYear(data) == 0)
        return 0;
    else{
        data->currentY = data->currentY->tail;
        return 1;
    }
}

void toBeginG(imdbADT data){
    data->currentY->currentG = data->currentY->firstG;
}

int hasNextG(imdbADT data){
    return data->currentY->currentG != NULL;
}

int nextG(imdbADT data){
    if (hasNextG(data) == 0)
        return 0;
    else{
        data->currentY->currentG = data->currentY->currentG->tail;
        return 1;
    }
}



//FRONT

//updateDATA




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
    data->currentY != NULL;
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
    return data->currentY->currentGenre != NULL;
}

TGenreL nextGenre(imdbADT data){
    if (hasNextGenre(data) == 0){

    }
    else{
        TGenreL aux = data->currentY->currentGenre;
        data->currentY->currentGenre = data->currentY->currentGenre->tail;
        return aux;
    }
}

