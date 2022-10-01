#include "functions.h"

int main(int argc, char ** argv) {

    clr();

    listaStart = malloc(sizeof(ugyfel_elem));
    char i_n = 'i';
    while(!listaStart) {
        printMessage(ALLOC_FAIL);
        scanf(" %c", &i_n);
        if(i_n == 'i')
            listaStart = malloc(sizeof(ugyfel_elem));
        else
            kilep(false);
    }
    *listaStart = (ugyfel_elem) {
        .ugyfel = NULL,
        .next = NULL,
        .prev = NULL
    };

    tryUntilOK(malloc(12), "Aktualis datum (YYYY.MM.DD.): ", "%11s", &validateDatum, &aktualisDatum);

    if(aktualisDatum.length != 11) kilep(false);

    if(argc > 1) {// argumentumként megadott állománynév
        string file = newString(argv[1]);
        _printMessage(readAndParseFile(&file), false);
    }else { // állománynév bekérése
        char fajlNev[100];
        string fileName;
        tryUntilOK(fajlNev, "Beolvasandó allomany neve (max 99 karakter): ", "%99s", &readAndParseFile, &fileName);
    }

    if(!listaStart->ugyfel) kilep(false); // ha nem lett fájl beolvasva

    int opcio;
    showOptions();
    do {
        scanf("%d", &opcio);
        clr();
        switch(opcio) {
            case 1: listUgyfel(); break;
            case 2: _printMessage(send(), false); break;
            case 3: _printMessage(newUgyfel(), false); break;
            case 4: _printMessage(deleteUgyfel(), false); break;
            case 5: _printMessage(printPersonal(), false); break;
            case 6: showUgyfelEgyenlegGraph(); break;
            case 7: kilep(true); break;
            default: printf("Ismeretlen opcio: %d\n", opcio);
        }
        showOptions();
    }while(true);

    return 0;
}