#include "imdbTAD.h"
#include "frontEnd.h"
#include "backEnd.h"

int main(int argc, char * argv[]) {
    char * file = "C:\\Users\\mbox\\CLionProjects\\TPE-FINAL\\imdbv3.csv";      ///!!! CAMBIAR DE ACUERDO

    int flag=0;
    imdbADT data = newDataBase(&flag);

    int c = readFile(data, file);

    if(c!=5)
        writeData(data);

    freeADT(data);
    return 0;
}
