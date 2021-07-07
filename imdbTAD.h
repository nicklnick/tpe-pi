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

//FRONT

void updateData(TEntry * entry);

//BACK

void toBeginYear(imdbADT data);
int nextYear(imdbADT data); //se fija si hay proximo anio llamando a hasNextYear y si hay aumenta el iterador
int hasNextYear(imdbADT data);

void toBeginG(imdbADT data);
int hasNextG(imdbADT data);
int nextG(imdbADT data); //se fija si hay proximo genero llamando a hasNextG y si hay aumenta el iterador


// Devuelve la cantidad de peliculas y series del anio que esta en el iterador en parametros de salida
void getAmountCurrY(imdbADT data, size_t * cantPelis, size_t * cantSeries, unsigned * year);

// Devuelve los generos y sus cantidades de peliculas del anio que esta en el iterador
void getAmountG(imdbADT data,unsigned * year, char * genero, size_t * cantPelis);

// Devuelve una copia de la peli/serie con mas votos
TEntry getMostPopular(imdbADT data, char type);

