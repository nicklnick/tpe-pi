
#include "imdbTAD.h"
#include "dataType.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define NO_FIELD 0 // Si el campo no aplica a ese tipo de entrada
#define PELI 1
#define SERIE 2

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
    for (int i = 0; i < list->serie.cantGenres; i++) {
        free(list->serie.genre[i]);
    }
    free(list->serie.genre);
    for (int i = 0; i < list->peli.cantGenres; i++) {
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

void getAmountCurrY(imdbADT data, unsigned * cantPelis, unsigned * cantSeries, unsigned * year){
    *year = data->currentY->year;
    *cantPelis = data->currentY->cantPelis;
    *cantSeries = data->currentY->cantSeries;
}

void getAmountG(imdbADT data, unsigned * year, const char * genero, unsigned * cantPelis){
    *year = data->currentY->year;
    genero = data->currentY->currentG->genre;           //!!!!!!!!!!!
    *cantPelis = data->currentY->currentG->cant;
}

TEntry * getMostPopular(imdbADT data, char type){
    TEntry * mostVoted = malloc(sizeof(TEntry));

    if (type == PELI){
        *mostVoted = data->currentY->peli;
    }
    else{ // type == SERIE
        *mostVoted = data->currentY->serie;
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

static TGenreL addGenres(TGenreL list, char ** genres, int * cont, unsigned cantGenres){
    int c;
    if (*cont == cantGenres){
        return list;
    }
    if (list == NULL || (c =(strcmp(list->genre, *genres))) > 0){
        TGenreL newGenre = malloc(sizeof(TGenre));
        newGenre->genre = *genres;                          //!!!!!!!
        newGenre->cant = 1;
        *cont += 1;
        newGenre->tail = addGenres(list, genres + 1, cont, cantGenres);         //!!!!!!
        return newGenre;
    }
    else if (c == 0){
        list->cant++;
        *cont += 1;
        list->tail = addGenres(list->tail, genres + 1, cont, cantGenres);
    }
    list->tail = addGenres(list->tail, genres, cont, cantGenres);
    return list;
}

static TYearL updateDataNewYear(TYearL list, TEntry * entry){
    if (list == NULL || list->year > entry->startYear){
        TYearL newYear = calloc(1, sizeof(TYear));
        newYear->year = entry->startYear;
        if (entry->type == PELI){
            newYear->cantPelis++;
            newYear->peli = *entry;
        }
        else{ //entry->type == SERIE8
            newYear->cantSeries++;
            newYear->serie = *entry;
        }
        int cont = 0;
        addGenres(list->firstG, entry->genre, &cont, entry->cantGenres);        //!!!!!!!!
        newYear->tail = list;
        return newYear;
    }
    list->tail = updateDataNewYear(list->tail, entry);
    return list;

}

static int updateExistingData(TYearL list, TEntry * entry){             //!!!!!!!! hacer 2 funciones distintas
    if (list == NULL || list->year > entry->startYear){
        return 0;
    }
    if (list->year == entry->startYear){
        entry->type == PELI ? list->cantPelis++ : list->cantSeries++;
        int cont = 0;
        addGenres(list->firstG,entry->genre,&cont,entry->cantGenres );
        return 1;
    }
    return updateExistingData(list->tail, entry);
}

static void updateMostVoted(TYearL list, TEntry * entry){
    if (list == NULL || list->year > entry->startYear){
        return;
    }
    if(list->year < entry->startYear){
        updateMostVoted(list->tail, entry);                 //!!!!!!!!
        return;
    }
    if (entry->type == PELI){
        if (entry->numVotes > list->peli.numVotes){
            list->peli = *entry;
            return;
        }
    }
    else { //entry->type == SERIE
        if (entry->numVotes > list->serie.numVotes) {
            list->serie = *entry;
            return;
        }
    }
}

void updateData(imdbADT data, TEntry * entry){
    if (updateExistingData(data->firstY, entry) == 0){
        data->firstY = updateDataNewYear(data->firstY,entry);
        return;                     // ya se updateo el anio, la cant de pelis/series y los generos.
    }
    updateMostVoted(data->firstY ,entry); // solo si el anio ya existia
}
