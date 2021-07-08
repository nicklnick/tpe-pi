//
// Created by on 6/7/2021.
//
#include "backEnd.h"

/* Carga los datos del query1 en su respectivo archivo */
TQuery1
queryOne(imdbADT data)
{
    unsigned year, cantPelis, cantSeries;
    TQuery1 out;

    getAmountCurrY(data, &year, &cantPelis, &cantSeries);
    out.year = year;
    out.cantPelis = cantPelis;
    out.cantSeries = cantSeries;
    return out;
}

/* Carga los datos del query2 en su respectivo archivo */
TQuery2
queryTwo(imdbADT data, int * flag)
{
    unsigned year, cantPelis;
    char * genero;
    TQuery2 out;

    getAmountG(data, &year, &genero, &cantPelis);
    out.year = year;
    out.genero = genero;
    out.cantPelis = cantPelis;

    *flag = hasNextG(data);

    return out;
}

/* Carga los datos del query3 en su respectivo archivo */
TQuery3
queryThree(imdbADT data)
{
    TEntry * peli, * serie;
    TQuery3 out;

    peli = getMostPopular(data, PELI);
    serie = getMostPopular(data, SERIE);

    out.peli = peli;
    out.serie = serie;

    free(peli);
    free(serie);
    return out;
}
