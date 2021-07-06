//
// Created by on 5/7/2021.
//

#include "imdbTAD.h"
#include <stdlib.h>

#define NO_FIELD 0 // Si el campo no aplica a ese tipo de entrada
#define PELI 1
#define SERIE 2

typedef struct TGenre {
    char * genre;
    size_t cant;  // Cantidad de peliculas del genero
    struct TGenre * tail;
} TGenre;

typedef struct TEntry {
    char * name;  // Nombre de la serie o pelicula
    char type;    // Serie = SERIE y Peli = PELI
    size_t numVotes;
    unsigned startYear; // Año de lanzamiento (peli) o comienzo de emision (serie)
    unsigned endYear;   // Año de fin de emision, o NO_FIELD si no termino o NO_FILED si es peli
    float avgRating;
    unsigned runtimeMin; // Runtime si es peli o NO_FIELD si es serie
    char ** genre;
} TEntry;

typedef struct TYear {
    size_t cantPelis;
    size_t cantSeries;
    TEntry peli;  // Peli mas votada
    TEntry serie; // Serie mas votada
    TGenre * firstGenre;  // Lista de generos de peliculas
    struct TYear * tail;  // Puntero al siguiente año
} TYear;

typedef struct imdbCDT {
    TYear * firstY;
} imdbCDT;

