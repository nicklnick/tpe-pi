//
// Created by on 5/7/2021.
//

#ifndef TPE_IMDBTAD_H
#define TPE_IMDBTAD_H

#include <stdio.h>

enum TQuery {Q1 = 1, Q2, Q3};

typedef imdbCDT * imdbADT;

imdbADT newDataBase();

void freeADT(imdbADT data);

/* Devuelve la cantidad de series y de pelis en un ano determinado que esta en el iterador.
 * VECTOR DE 3 posiciones (ano/cantPelis/cantSeries) */
unsigned int * getAmount(const imdbADT data);

/* Devuelve la pelicula o serie mas votada de un ano determinado que este en el iterador*/
TEntry getMostPopular(const imdbADT data, char type);


/* Devuelve la cantidad de peliculas y series de un genero especifico que esta en el iterador*/
size_t getAmountPerGenre(const imdbADT data);

/* devuelve los votos de la pelicula o serie con mas votos de un ano especifico*/
size_t getMostVotes(imdbADT data, unsigned year, char type);

// updateInicial para agregar peli y serie y su genero si ya existe

/* actualiza la cantidad de pelis/series que hay en un ano o la cantidad de peliculas o series con cierto genero en un ano)*/
//void updateCant(imdbADT data, unsigned year, char type, char * genre);

//void updateCantGenres(imdbADT data, char * genre);

//void updateMostVoted(imdbADT data);



/* funciones para iterar */

void toBeginYear(imdbADT data);

int hasNextYear(imdbADT data);

TYearL nextYear(imdbADT data);



/* funciones para iterar el genero */

void toBeginGenre(imdbADT data);

int hasNextGenre(imdbADT data);

TGenreL nextGenre(imdbADT data);

#endif //TPE_IMDBTAD_H
