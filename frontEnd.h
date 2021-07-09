
#ifndef TPE_FINAL_FRONTEND_H
#define TPE_FINAL_FRONTEND_H

#include "dataTypes.h"
#include "imdbTAD.h"

/*
 * Funcion:     readFile
 * Uso:         c = readFile(fileName, data);
 * ----------------------------------------------------------------------
 * Descripcion: Lee los datos del .csv y la almacena en el imdbTAD.
 * ----------------------------------------------------------------------
 * Recibe:      Puntero a estructura de datos, y nombre del archivo a leer
 * Devuelve:    Devuelve flag prendido si tuvo error al abrir el file
 */
void readFile(imdbADT data, char * fileName);

/*
 * Funcion:     writeData
 * Uso:         c = writeData(fileName, data);
 * ----------------------------------------------------------------------
 * Descripcion: Escribe los datos de la estructura en los archivos
 *              correspondientes a los queries
 * ----------------------------------------------------------------------
 * Recibe:      Puntero a estructura de datos
 * Devuelve:    VOID
 */
void writeData(imdbADT data);

#endif //TPE_FINAL_FRONTEND_H
