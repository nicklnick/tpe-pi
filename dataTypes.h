
#ifndef TPE_FINAL_DATATYPES_H
#define TPE_FINAL_DATATYPES_H

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

#define PELI 1
#define SERIE 2

#endif //TPE_FINAL_DATATYPES_H
