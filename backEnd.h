#ifndef TPE_IMDB_BACKEND_H
#define TPE_IMDB_BACKEND_H
#include <stdio.h>
#include <stdlib.h>
#include "imdbTAD.h"

#define FNAME_Q1 "query1.csv"
#define FNAME_Q2 "query2.csv"
#define FNAME_Q3 "query3.csv"

/* Crea un archivo "filename" */
FILE * createCSV(const char * fileName);

/* Crea los archivos .csv de las 3 queries y carga en ellos los datos con loadData */
void processData(imdbADT data);

/* Carga los datos en los archivos respectivos:
 * - data : estructura que contiene todos los datos procesados del archivo dado
 * - query1 : file en el que se vuelcan los datos respectivos al primer query
 * - query2 : file en el que se vuelcan los datos respectivos al segundo query
 * - query3 : file en el que se vuelcan los datos respectivos al tercer query
*/
int loadData(imdbADT data, FILE * query1, FILE * query2, FILE * query3);

/* Carga los datos del query1 en su respectivo archivo */
void solveQuery1(imdbADT data, FILE * query1);
/* Carga los datos del query2 en su respectivo archivo */
void solveQuery2(imdbADT data, FILE * query2);
/* Carga los datos del query3 en su respectivo archivo */
void solveQuery3(imdbADT data, FILE * query3);


#endif //TPE_IMDB_BACKEND_H
