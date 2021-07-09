#include "imdbTAD.h"
#include "frontEnd.h"
#include "backEnd.h"

int main(int argc, char * argv[]) {
    char * file = "C:\\Users\\mbox\\CLionProjects\\TPE-FINAL\\imdbv3.csv";      ///!!! CAMBIAR DE ACUERDO

    imdbADT data = newDataBase();

    int c = readFile(file, data);

    if(c!=5)
        processData(data);

    freeADT(data);
    return 0;
}
