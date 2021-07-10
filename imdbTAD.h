/*
 *  Created on 09/07/2021
 *  Created by:
 *      - Marcos Gronda
 *      - Tomas Camilo Gay Bare
 *      - Nicolas Matias Margenat
 *  Copyright © 2021. All rights reserved.
*/

#ifndef TPE_IMDBTAD_H
#define TPE_IMDBTAD_H

#include <stdio.h>
#include "dataTypes.h"

/* Definimos el TAD para la base de datos */
typedef struct imdbCDT * imdbADT;

/*
 * Funcion:     newDataBase
 * Uso:         imdb = newDataBase();
 * ----------------------------------------------------------------------
 * Descripcion: Crea una nueva Base de Datos de peliculas y series.
 * ----------------------------------------------------------------------
 * Recibe:      Puntero a zona donde hay un flag de error
 * Devuelve:    Puntero a Base de Datos, o enciende el flag en caso de haber
 *              algun error
 */
imdbADT newDataBase(int * error);

/*
 * Funcion:     freeADT
 * Uso:         freeADT(imdb);
 * ----------------------------------------------------------------------
 * Descripcion: Libera el espacio utilizado por el TAD
 * ----------------------------------------------------------------------
 * Recibe:      Puntero a Base de Datos
 * Devuelve:    VOID
 */
void freeADT(imdbADT data);


/*
 * Funcion:     updateData
 * Uso:         updateData(imdb, entry, &error);
 * ----------------------------------------------------------------------
 * Descripcion: Se ocupa de actualizar la Base de Datos
 * ----------------------------------------------------------------------
 * Recibe:      Puntero a Base de Datos, nueva entrada de datos
 *              y puntero a zona donde hay un flag de error
 * Devuelve:    En caso de haber un error enciende el flag
 */
void updateData(imdbADT data, TEntry * entry, int * error);



/*
 * Conjunto de funciones que permiten iterar sobre la lista de años
 * PRECONDICION: Haber llamado antes a toBeginYear
 */
/* Resetea el iterador que recorre los años de manera descendente */
void toBeginYear(imdbADT data);

/* Se fija si hay año siguiente */
int hasNextYear(imdbADT data);

/* Pasa al siguiente año, o retorna si no lo hay */
void nextYear(imdbADT data);


/*
 * Conjunto de funciones que permiten iterar sobre la lista de generos de un determinado año.
 * PRECONDICION: Haber llamado antes a toBeginYear
 */
/* Resetea el iterador que recorre los generos de manera ascendente, de un determinado año */
void toBeginG(imdbADT data);

/* Se fija si hay un genero siguiente */
int hasNextG(imdbADT data);

/* Pasa al siguiente genero, o retorna si no lo hay */
void nextG(imdbADT data);


/* PRECONDICION: Haber llamado toBeginYear */
/*
 * Funcion:     getAmountCurrY
 * Uso:         getAmountCurrY(imdb, &year, &cantPelis, &cantSeries);
 * ----------------------------------------------------------------------
 * Descripcion: Obtiene la cantidad de peliculas y series que salieron en un año determinado
 * ----------------------------------------------------------------------
 * Recibe:      Puntero a Base de Datos,
 *              3 parametros de salida de tipo unsigned
 * Devuelve:    En 3 parametros de salida:
 *                  - cantPelis : la cantidad de peliculas del año year.
 *                  - cantSeries : la cantidad de series del año year.
 *                  - year : año del que estamos sacando la informacion
 */
void getAmountCurrY(imdbADT data, unsigned * year, unsigned * cantPelis, unsigned * cantSeries);

/* PRECONDICION: Haber llamado toBeginYear */
/*
 * Funcion:     getMostPopular
 * Uso:         mostPop = getMostPopular(imdb, type, &error)
 * ----------------------------------------------------------------------
 * Descripcion: Devuelve una copia de la peli/serie con mas votos
 * ----------------------------------------------------------------------
 * Recibe:      Puntero a Base de Datos, char que indica si queremos una PELI o SERIE
 *              y puntero a zona donde hay un flag de error
 * Devuelve:    Una copia con los datos de la PELI/SERIE mas votada en un determinado año,
 *              y enciende el flag en caso de haber error
 */
TEntry * getMostPopular(imdbADT data, char type, int * error);

/* PRECONDICION: Haber llamado toBeginYear y toBeginG */
/*
 * Funcion:     getAmountG
 * Uso:         getAmountG(imdb, &year, &genero, &cantPelis);
 * ----------------------------------------------------------------------
 * Descripcion: Obtiene la cantidad de peliculas del genero que salieron en determinado año
 * ----------------------------------------------------------------------
 * Recibe:      Puntero a Base de Datos,
 *              3 parametros de salida: 2 de tipo unsigned y uno de tipo char *
 * Devuelve:    En 3 parametros de salida:
 *                  - year : año del que estamos sacando la informacion
 *                  - genero : genero que estamos consultando
 *                  - cantPelis : la cantidad de peliculas del año year.
 */
void getAmountG(imdbADT data, unsigned * year, char ** genero, unsigned * cantPelis);

#endif
