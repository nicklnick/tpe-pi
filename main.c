/*
 *  Created on 09/07/2021
 *  Created by:
 *      - Marcos Gronda
 *      - Tomas Camilo Gay Bare
 *      - Nicolas Matias Margenat
 *  Copyright © 2021. All rights reserved.
 *
*/

#include "imdbTAD.h"
#include "frontEnd.h"

int
main(int argc, char * argv[])
{
    if( argc != 2 )
    {
        fprintf(stderr, "Cantidad de argumentos incorrecta");
        exit(ERROR_DE_ARGS);//Aborta porque no recibe un argumento
    }

    int flag = 0;

    imdbADT data = newDataBase(&flag);

    if( flag == INSUFFICIENT_MEM )
    {
        fprintf(stderr, "Cantidad de memoria insuficiente");
        exit(INSUFFICIENT_MEM);
    }

    readFile(data, argv[1]);

    writeData(data);

    freeADT(data);

    return 0;
}
