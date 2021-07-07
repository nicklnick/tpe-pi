#ifndef TPE_IMDB_BACKEND_H
#define TPE_IMDB_BACKEND_H
#include <stdio.h>
#include <stdlib.h>
#include "imdbTAD.h"

#define FNAME_Q1 "query1.csv"
#define FNAME_Q2 "query2.csv"
#define FNAME_Q3 "query3.csv"

/* Crea los archivos .csv de las 3 queries y carga en ellos los datos con loadData */
void processData(imdbADT data);

#endif //TPE_IMDB_BACKEND_H
