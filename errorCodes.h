//
// Created by nicol on 9/7/2021.
//

#ifndef TPE_IMDB_ERRORCODES_H
#define TPE_IMDB_ERRORCODES_H

#include <errno.h>

#define OK 0
#define ERROR_DE_ARGS 20
#define INSUFFICIENT_MEM 10
#define ERROR_DE_FILE 5
#define INVALID_YEAR 3
#define MSG_MEM "Cantidad de memoria insuficiente"
#define MSG_FILE "File no existente"

/* No hay suficiente espacio en memoria */
#define NO_MEM(error)  {  if( errno == ENOMEM )      \
                             (error) = INSUFFICIENT_MEM;           \
                       };

/* Hubo error al manipular el file */
#define FILE_ERROR(file, retValue)  {  if( (file) == NULL )      \
                                            return retValue;       \
                                    };

/* Sale de la funcion si hubieron errores */
#define RETURN_IF_ERROR(error, retValue) { if( (error) != OK )          \
                                            return retValue; \
                                         };

#define FREE_ADT(error, data) { if( (error) != OK )      \
                                  freeADT(data); \
                              };

#define ABORT_IF_ERROR(error, errorMsg) { if( (error) != OK ) {     \
                                  fprintf(stderr, errorMsg);       \
                                  exit(error);                     \
                                  }                                 \
                              };


#endif //TPE_IMDB_ERRORCODES_H
