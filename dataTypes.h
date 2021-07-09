
#ifndef TPE_FINAL_DATATYPES_H
#define TPE_FINAL_DATATYPES_H

#include <stdlib.h>

typedef struct TEntry {
    char * name;  // Nombre de la serie o pelicula
    char type;    // Serie = SERIE y Peli = PELI
    unsigned numVotes;
    unsigned startYear; // Año de lanzamiento (peli) o comienzo de emision (serie)
    unsigned endYear;   // Año de fin de emision, o NO_FIELD si no termino o NO_FILED si es peli
    float avgRating;
    unsigned runtimeMin; // Runtime si es peli o NO_FIELD si es serie
    char ** genre;
    unsigned cantGenres;
} TEntry;

typedef struct TQuery1 {
    unsigned year;
    unsigned cantPelis;
    unsigned cantSeries
} TQuery1;

typedef struct TQuery2 {
    unsigned year;
    unsigned cantPelis;
    char * genero;
} TQuery2;

// OKE
typedef struct TQuery3 {
    TEntry * peli;
    TEntry * serie;
} TQuery3;

#define PELI 1
#define SERIE 2
#define NO_FIELD 0

#define EMPTY 0             // Si en copyText no se necesita la funcionalidad de SEPARADOR
#define BLOCK 10

static char * copyText(const char * text, char c){          // Copia hasta \0 o hasta el caracter c
    int i;
    char * new = NULL;

    for(i=0; text[i]!=0 && text[i]!=c; i++){
        if(i%BLOCK==0){
            new = realloc(new, (i+BLOCK)*sizeof(char));
        }
        new[i] = text[i];
    }

    new = realloc(new, (i+1)*sizeof(char));
    new[i]=0;
    return new;
}


#endif //TPE_FINAL_DATATYPES_H
