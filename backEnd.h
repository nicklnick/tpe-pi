#ifndef TPE_IMDB_BACKEND_H
#define TPE_IMDB_BACKEND_H

#include <stdio.h>
#include <stdlib.h>
#include "imdbTAD.h"

/* Nombres de los archivos a crear */
#define FNAME_Q1 "query1.csv"
#define FNAME_Q2 "query2.csv"
#define FNAME_Q3 "query3.csv"

/*
 * Funcion:     queryOne
 * Uso:         query1 = queryOne();
 * ----------------------------------------------------------------------
 * Descripcion: Resuelve el primer query
 * ----------------------------------------------------------------------
 * Recibe:      Puntero a base de datos
 * Devuelve:    Estructura con los datos necesarios para resolver el primer query
 */
TQuery1 queryOne(imdbADT data);

/*
 * Funcion:     queryTwo
 * Uso:         query2 = queryTwo();
 * ----------------------------------------------------------------------
 * Descripcion: Resuelve el segundo query
 * ----------------------------------------------------------------------
 * Recibe:      Puntero a base de datos y flag que indica si hay genero siguiente en
 *              el año que esta siendo consultado (genero ordenado ascendente)
 * Devuelve:    Estructura con los datos necesarios para resolver el segundo query
 */
TQuery2 queryTwo(imdbADT data, int * flag);

/*
 * Funcion:     queryThree
 * Uso:         query3 = queryThree();
 * ----------------------------------------------------------------------
 * Descripcion: Resuelve el tercer query
 * ----------------------------------------------------------------------
 * Recibe:      Puntero a base de datos
 *              y puntero a zona donde hay un flag de error
 * Devuelve:    Estructura con los datos necesarios para resolver el tercer query
 */
TQuery3 queryThree(imdbADT data, int * error);

#endif //TPE_IMDB_BACKEND_H
