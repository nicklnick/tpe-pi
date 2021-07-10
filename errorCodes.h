/*
 *  Created on 09/07/2021
 *  Created by:
 *      - Marcos Gronda
 *      - Tomas Camilo Gay Bare
 *      - Nicolas Matias Margenat
 *  Copyright © 2021. All rights reserved.
*/

#ifndef TPE_IMDB_ERRORCODES_H
#define TPE_IMDB_ERRORCODES_H

#include <errno.h>

/* Mensajes de error */
#define MSG_MEM "Cantidad de memoria insuficiente"
#define MSG_FILE "File no existente"
#define MSG_ARG "Cantidad de argumentos incorrecta"

/* Codigos de error */
#define ERROR_DE_ARGS 20
#define INSUFFICIENT_MEM 10
#define INVALID_YEAR 3

#define OK 0

/* CONJUNTO DE MACROS QUE SE ENCARGAN DEL MANEJO DE ERRORES */
/* No hay suficiente espacio en memoria */
#define NO_MEM(error)  {                                    \
                            if( errno == ENOMEM )           \
                                (error) = INSUFFICIENT_MEM; \
                       };

/* Sale de la funcion si hubieron errores */
#define RETURN_IF_ERROR(error, retValue) {                          \
                                            if( (error) != OK )     \
                                              return retValue;      \
                                         };

/* Si hubo un error imprime a salida de error y aborta el programa */
#define ABORT_IF_ERROR(error, errorMsg) {                                   \
                                            if( (error) != OK ) {           \
                                                fprintf(stderr, errorMsg);  \
                                                exit(error);                \
                                            }                               \
                                        };

/* Libera el espacio del TAD en caso de error */
#define FREE_ADT(error, data) {                         \
                                if( (error) != OK )     \
                                    freeADT(data);      \
                              };

#endif //TPE_IMDB_ERRORCODES_H
