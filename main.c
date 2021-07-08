#include "imdbTAD.h"
#include "frontEnd.h"
#include "backEnd.h"

int main(int argc, char * argv[]) {
    char * file = "C:\\Users\\mbox\\CLionProjects\\TPE-FINAL\\imdbv2.csv";

    imdbADT data = newDataBase();

    readFile(file, data);

    processData(data);

    return 0;
}
