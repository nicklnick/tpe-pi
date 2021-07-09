/*
 *  Created on 09/07/2021
 *  Created by:
 *      - Marcos Gronda
 *      - Tomas Camilo Gay Bare
 *      - Nicolas Matias Margenat
 *  Copyright © 2021. All rights reserved.
*/

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

    nextG(data);
    *flag = hasNextG(data);

    return out;
}

/* Carga los datos del query3 en su respectivo archivo */
TQuery3
queryThree(imdbADT data, int * error)
{

    TQuery3 out;
    out.peli = getMostPopular(data, PELI, error);
    out.serie = getMostPopular(data, SERIE, error);

    return out;
}
