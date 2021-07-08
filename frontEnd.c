#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "frontEnd.h"
#include "imdbTAD.h"
#include "dataTypes.h"

#define LINE_MAX 400

#define SEPARADOR ";"
#define SEPARADOR_2 ','

#define OK 0
#define ERROR_DE_FILE 5
#define INVALID_YEAR 3

#define EMPTY_FIELD "\\N"

#define UPDATE_TOKEN token = strtok(NULL, SEPARADOR);

#define CHECK_ENTRY(token, field, type){ if(!strcmp(token, EMPTY_FIELD))\
                                         entry->field = NO_FIELD;       \
                                       else                             \
                                         entry->field = type(token);    \
};



static char * copyText(const char * text, char c){          // Copia hasta \0 o hasta el caracter c
    int i;
    char * new = NULL;

    for(i=0; text[i]!=0 && text[i]!=c; i++){
        if(i%BLOCK==0){
            new = realloc(new, (i+BLOCK)*sizeof(char));
        }
        new[i] = text[i];
    }

    new = realloc(new, (i+1)*sizeof(char));
    new[i]=0;
    return new;
}

static char ** loadGenres(char * line, unsigned * cant){
    int size, dim, i;
    char ** genres = NULL;

    for(i=0, size=0, dim=0; line[i]!=0; ){

        if(size%BLOCK==0) {
            genres = realloc(genres, (size + BLOCK) * sizeof(char *));
            size += BLOCK;
        }
        genres[dim++] = copyText(line + i, SEPARADOR_2);        // + i cantidad de offset

        for(;line[i]!=SEPARADOR_2 && line[i]!=0; i++);      // Pasa al proximo genero Ej: Drama Comedia => Comedia
        if(line[i]!=0){
            i++;
        }
    }
    genres = realloc(genres, dim*sizeof(char *));
    *cant = dim;
    return genres;
}


static int updateEntry(TEntry * entry, char * line){
    char * token = strtok(line, SEPARADOR);

    if(!strcmp(token, "movie"))      // Tipo de entry
        entry->type = PELI;
    else
        entry->type = SERIE;

    UPDATE_TOKEN
    entry->name = copyText(token, 0);        // Nombre de entry

    UPDATE_TOKEN
    if(!strcmp(token, EMPTY_FIELD))
        return INVALID_YEAR;                   // Verifica si el anio es valido, si no lo eso => corta
    else
        entry->startYear = atoi(token);        // Anio de comienzo

    UPDATE_TOKEN
    CHECK_ENTRY(token, endYear, atoi);

    UPDATE_TOKEN
    unsigned cant;
    entry->genre = loadGenres(token, &cant);        // Carga los generos en la lista
    entry->cantGenres = cant;

    UPDATE_TOKEN
    CHECK_ENTRY(token, avgRating, atof);

    UPDATE_TOKEN
    CHECK_ENTRY(token, numVotes, atoi);

    UPDATE_TOKEN
    CHECK_ENTRY(token, runtimeMin, atoi);

    return OK;
}

static void freeResources(TEntry * entry){
    if(entry==NULL)
        return;
    free(entry->name);
    for(int i=0; i<entry->cantGenres; i++){
        free(entry->genre[i]);
    }
    free(entry->genre);
}


int readFile(char fileName[], imdbADT data){
    FILE * imdbFile;
    imdbFile = fopen(fileName, "r");

    if(imdbFile == NULL){
        return ERROR_DE_FILE;
    }

    char line[LINE_MAX];        // Levanta hasta LINE_MAX caracteres del file

    TEntry * entry = malloc(sizeof(TEntry));

    fgets(line, sizeof(line), imdbFile);    // Ignora la primera linea

    while(fgets(line, sizeof(line), imdbFile)) {
        if(updateEntry(entry, line)!=INVALID_YEAR){
            updateData(data, entry);                    // Solo si el entry es valido se actualiza el ADT
            freeResources(entry);
        }
        else{
            free(entry->name);
        }
    }

    free(entry);
    fclose(imdbFile);       // Cierre final del file
    return OK;
}
