#ifndef TPE_IMDBTAD_H
#define TPE_IMDBTAD_H

#include <stdio.h>
#include "dataType.h"

enum TQuery {Q1 = 1, Q2, Q3};

typedef struct imdbCDT * imdbADT;

/* Crea una nueva Base de Datos de peliculas y series */
imdbADT newDataBase();

/* Libera el espacio utilizado por el TAD */
void freeADT(imdbADT data);

//FRONT
void updateData(imdbADT data, TEntry * entry);

//BACK
/* Resetea el iterador que recorre los años de manera descendente */
void toBeginYear(imdbADT data);
/* Se fija si hay año siguiente */
int hasNextYear(imdbADT data);
/*
 * Se fija si hay año proximo.
 * - Si hay año proximo mueve el iterador y retorna 1.
 * - Si no hay año proximo retorna 0.
 */
int nextYear(imdbADT data);

/* Resetea el iterador que recorre los generos de manera ascendente, de un determinado año */
void toBeginG(imdbADT data);
/* Se fija si hay un genero siguiente */
int hasNextG(imdbADT data);
/*
 * Se fija si hay un genero siguiente.
 * - Si lo hay, mueve el iterador y retorna 1.
 * - Si no lo hay retorna 0.
 */
int nextG(imdbADT data);

/*
 * Devuelve en 3 paramentros de salida:
 * - cantPelis : la cantidad de peliculas del año year.
 * - cantSeries : la cantidad de series del año year.
 * - year : año del que estamos sacando la informacion
 */
void getAmountCurrY(imdbADT data, size_t * cantPelis, size_t * cantSeries, unsigned * year);

/*
 * Devuelve en 3 paramentros de salida:
 * - year : año del que estamos sacando la informacion
 * - genero : genero que estamos consultando
 * - cantPelis : la cantidad de peliculas del año year del genero.
 */
void getAmountG(imdbADT data, unsigned * year, const char * genero, size_t * cantPelis);

/* Devuelve una copia de la peli/serie con mas votos */
TEntry * getMostPopular(imdbADT data, char type);

#endif
