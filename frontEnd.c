#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "frontEnd.h"
#include "imdbTAD.h"

#define ERROR_DE_FILE 5
#define LINE_MAX 200
#define SEPARADOR ";"
#define MAX_FIELDS 8
#define BLOCK 10
#define EMPTY_FIELD "\\N"
#define SEPARADOR_2 " "
#define OK 1

#define PELI 1
#define SERIE 2
#define NO_FIELD 0

enum TField {titleType = 0, title, startYear, endYear, genres, rating, numVotes, runtime};

typedef struct TEntry {
    char * name;  // Nombre de la serie o pelicula
    char type;    // Serie = SERIE y Peli = PELI
    size_t numVotes;
    unsigned startYear; // Año de lanzamiento (peli) o comienzo de emision (serie)
    unsigned endYear;   // Año de fin de emision, o NO_FIELD si no termino o NO_FILED si es peli
    float avgRating;
    unsigned runtimeMin; // Runtime si es peli o NO_FIELD si es serie
    char ** genre;
    unsigned cantGenres;
} TEntry;


static char * copyText(const char * text){
    int i;
    char * new = NULL;

    for(i=0; text[i]!=0; i++){
        if(i%BLOCK==0){
            new = realloc(new, (i+BLOCK)*sizeof(char));
        }
        new[i] = text[i];
    }

    new = realloc(new, (i+1)*sizeof(char));
    new[i]=0;
    return new;
}

char ** getAllFields(char * line){
    char ** entryVec = malloc(sizeof(char *)*MAX_FIELDS);
    char * token = strtok(line, SEPARADOR);

    for(int i =0; i < MAX_FIELDS; i++){
        entryVec[i] = copyText(token);
    }
    return entryVec;
}

char ** loadGenres(char ** genres, char * line){
    int i, size, dim;
    char * token = strtok(line, SEPARADOR_2);

    for(i=0, size=0, dim=0; line[i]!=0; i++){

        if(size%BLOCK==0) {
            genres = realloc(genres, (size + BLOCK) * sizeof(char *));
            size += BLOCK;
        }

        genres[dim++] = copyText(token);

        token = strtok(NULL, SEPARADOR_2);
    }
    genres = realloc(genres, dim*sizeof(char *));

    return genres;
}


void updateEntry(TEntry * entry, char * line){
    char ** fields = getAllFields(line);

    if(!strcmp(fields[titleType], "movie"))      // Tipo de entry
        entry->type = PELI;
    else
        entry->type = SERIE;

    entry->name = copyText(fields[title]);        // Nombre de entry

    entry->startYear = atoi(fields[startYear]);        // Anio de comienzo

    if(!strcmp(fields[endYear], EMPTY_FIELD))      // Verifica que el valor no sea \N
        entry->endYear = NO_FIELD;
    else
        entry->endYear = atoi(fields[endYear]);

    entry->genre = loadGenres(entry->genre, fields[genres]);

    entry->avgRating = atoi(fields[rating]);

    entry->numVotes = atoi(fields[numVotes]);

    if(!strcmp(fields[runtime], EMPTY_FIELD))      // Verifica que el valor no sea \N
        entry->runtimeMin = NO_FIELD;
    else
        entry->runtimeMin = atoi(fields[runtime]);

}

void freeResources(TEntry * entry){
    free(entry->name);
    for(int i=0; i<entry->cantGenres; i++){
        free(entry->genre[i]);
    }
}


int readFile(char fileName[]){
    FILE * imdbFile;
    imdbFile = fopen(fileName, "r");

    if(imdbFile == NULL){
        return ERROR_DE_FILE;
    }

    char line[LINE_MAX];        // Levanta hasta LINE_MAX caracteres del file
    TEntry * entry = malloc(sizeof(TEntry));

    while(fgets(line, sizeof(line), imdbFile)) {
        updateEntry(entry, line);
        updateData(entry);

        freeResources(entry);
    }

    fclose(imdbFile);       // Cierre final del file

    return OK;
}
