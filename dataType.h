//
// Created by Gayba on 06/07/2021.
//

#ifndef TPE_IMDB_DATATYPE_H
#define TPE_IMDB_DATATYPE_H

typedef struct TEntry {
    char * name;  // Nombre de la serie o pelicula
    char type;    // Serie = SERIE y Peli = PELI
    unsigned startYear; // Año de lanzamiento (peli) o comienzo de emision (serie)
    unsigned endYear;   // Año de fin de emision, o NO_FIELD si no termino o NO_FILED si es peli
    float avgRating;
    size_t numVotes;
    unsigned runtimeMin; // Runtime si es peli o NO_FIELD si es serie
    char ** genre;
    unsigned cantGenres;
} TEntry;


#endif //TPE_IMDB_DATATYPE_H
