#include "functions.h"

int _stoi(string * str, int skip) {
    int i = skip, n = 0;
    while(i < str->length && isDigit(str->chars[i])) n = n * 10 + (str->chars[i++] - '0');
    return n;
}

string newString(char * data) {
    int len = 0;
    while(data[len]) len++;
    return (string) {.length = len, .chars = data};
}

bool stringEquals(string * _1, string  * _2) {
    if(_1->length != _2->length) return false;
    for(int i = 0; i < _1->length; i++) if(_1->chars[i] != _2->chars[i]) return false;
    return true;
}

inline date newDate(string * str) {
    return (date) {
        .year = stoi(str),
        .month = _stoi(str, 5),
        .day = _stoi(str, 8)
    };
}

inline bool isLeapYear(date * datum) {
    return datum->year % 400 == 0 || (datum->year % 4 == 0 && datum->year % 100 != 0);
}

inline char getMonthLength(date * datum) {
    return month[datum->month - 1] + // napok a hónapban
    (isLeapYear(datum) && datum->month == 2); // +1, ha szökőév és február van
}

void _printMessage(int code, bool askAgain) {
    if(code > OK) // hiba üzenetek
        printf(ERR_ASK[askAgain], ERR_MSG[code]);
    else if(code < OK) // normál üzenetek
        printf("%s\n", OK_MSG[-code - 2]);
}

void checkAlloc(char ** ptr, size_t size, char ** dealloc, size_t deallocSize) {
    *ptr = malloc(size);
    char i_n = 'i';
    while(!*ptr) {
        printMessage(ALLOC_FAIL);
        scanf(" %c", &i_n);
        if(i_n == 'i')
            *ptr = malloc(size);
        else { // feladta a felhasználó a memóriafoglalást
            for(int i = 0; i < deallocSize; i++) free(dealloc[i]);
            kilep(true);
        }
    }
}

bool tryUntilOK(char * buffer, const char * message, const char * format, int (*verify)(string *), string * result) {
    int status;
    char i_n = 'i';
    do {
        printf("%s", message);
        scanf(format, buffer);
        *result = newString(buffer);
        status = verify(result);
        printMessage(status);
        if(status > OK) scanf(" %c", &i_n);
    }while(status > OK && i_n == 'i');
    return i_n == 'i';
}

inline bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

ugyfel newUgyfel_str(string nev, string email, string tel, int egyenleg, string datum, int kamat) {

    // Ideiglenes változók
    char * _nev, * _email, * _tel,  * _datum;

    // Memóriafoglalás
    checkAlloc(&_nev,   nev.length   + 1, NULL,                           0);
    checkAlloc(&_email, email.length + 1, (char *[]){_nev},               1);
    checkAlloc(&_tel,   tel.length   + 1, (char *[]){_nev, _email},       2);
    checkAlloc(&_datum, datum.length + 1, (char *[]){_nev, _email, _tel}, 3);

    // Adatok másolása (i <= ... hogy a '\0' is meglegyen)
    for(int i = 0; i <= nev.length  ; i++) _nev  [i] = nev.chars  [i];
    for(int i = 0; i <= email.length; i++) _email[i] = email.chars[i];
    for(int i = 0; i <= tel.length  ; i++) _tel  [i] = tel.chars  [i];
    for(int i = 0; i <= datum.length; i++) _datum[i] = datum.chars[i];

    // Új ügyfél létrehozása
    return (ugyfel) {
        .nev = newString(_nev),
        .email = newString(_email),
        .telefon = newString(_tel),
        .egyenleg = egyenleg,
        .lekotesDatum = newString(_datum),
        .kamat = kamat
    };
}

int validateNev(string * nev) {
    if(nev->length == 0) return NEV_EMPTY;
    return OK;
}

int validateEmail(string * email) {
    if(email->length < 5)                      return EMAIL_SHORT;
    if(email->chars[0] == '.')                 return EMAIL_START_DOT;
    if(email->chars[0] == '@')                 return EMAIL_START_AT;
    if(email->chars[email->length - 1] == '.') return EMAIL_END_DOT;
    if(email->chars[email->length - 1] == '@') return EMAIL_END_AT;

    bool pont = false, kukac = false;

    for(int i = 0; i < email->length - 1; i++)
        if(email->chars[i] == '.') {
            if(pont) return EMAIL_TWO_DOT;
            pont = true;
            if(email->chars[i + 1] == '@')
                return EMAIL_AT_AFTER_DOT; // pont után kukac
        }else if(email->chars[i] == '@') {
            if(kukac) return EMAIL_TWO_AT;
            kukac = true;
            if(email->chars[i + 1] == '.')
                return EMAIL_DOT_AFTER_AT; // kukac után pont
        }

    return pont && kukac ? OK : EMAIL_MISSING_DOT_OR_AT; // volt pont ÉS kukac is megfelelő pozícióban
}

int validateTelefon(string * telefon) {
    if(telefon->length == 0) return TEL_EMPTY; // üres telefonszám
    int i = 0;
    while(i < telefon->length && isDigit(telefon->chars[i])) i++; // addig iterálunk, amíg numerikus az adott karakter
    return i == telefon->length ? OK : TEL_BAD_CHARS; // ha végigért a ciklus, akkor minden karakter numerikus
}

int validateDatum(string * datum) {

    if(!(isDigit(datum->chars[0]) && isDigit(datum->chars[1]) && isDigit(datum->chars[2]) && isDigit(datum->chars[3])) || // YYYY
       datum->chars[4] != '.' || // .
       !(isDigit(datum->chars[5]) && isDigit(datum->chars[6])) || // MM
       datum->chars[7] != '.' || // .
       !(isDigit(datum->chars[8]) && isDigit(datum->chars[9])) || // DD
       datum->chars[10] != '.' // .
    ) return DATE_BAD_FORMAT;

    date d = newDate(datum);
    if(d.year < 1000)                            return DATE_INVALID_YEAR;
    if(d.month == 0 || d.month > 12)             return DATE_INVALID_MONTH;
    if(d.day == 0 || d.day > getMonthLength(&d)) return DATE_INVALID_DAY;

    return OK;
}

int validateKamat(string * kamat) {
    int i = 0;
    while(kamat->chars[i]) if(!isDigit(kamat->chars[i++])) return KAMAT_LETTER;
    int k = stoi(kamat);
    return k < 0 || k > 100 ? KAMAT_OUT_OF_RANGE : OK;
}

int validateEgyenleg(string * egyenleg) {
    return egyenleg->length > 0 ? OK : EGYENLEG_EMPTY;
}

int readAndParseFile(string * filename) {

    FILE *f = fopen(filename->chars, "r");
    if(!f) return FILE_OPEN;
    int ok_items = 0;

    char nev[NEV_LEN], email[EMAIL_LEN], telefon[TEL_LEN], lekotesDatum[DATUM_LEN];
    int egyenleg, kamat;

    while((ok_items = fscanf(f, FILE_FORMAT, nev, email, telefon, &egyenleg, lekotesDatum, &kamat)) != EOF) { // amíg a sorban talált elemek száma nagyobb mint 0
        if(ok_items != 6) return FILE_BAD_FORMAT; // hibás formátum
        ugyfel * ujUgyfel = malloc(sizeof(ugyfel));
        if(!ujUgyfel) return ALLOC_FAIL; // memória foglalás hiba
        *ujUgyfel = newUgyfel_str(newString(nev), newString(email), newString(telefon), egyenleg, newString(lekotesDatum), kamat);
        addUgyfel(ujUgyfel);
    }

    fclose(f);
    char * _openedFile;
    checkAlloc(&_openedFile, filename->length + 1, NULL, 0);
    for(int i = 0; i <= filename->length; i++) _openedFile[i] = filename->chars[i];
    openedFile = newString(_openedFile);
    return FILE_READ_OK;
}

int writeFile() {
    char * x = openedFile.chars;
    FILE *f = fopen(openedFile.chars, "w");
    if(!f) return FILE_OPEN;
    FOREACH(fprintf(f, FILE_FORMAT, u.nev.chars, u.email.chars, u.telefon.chars, u.egyenleg, u.lekotesDatum.chars, u.kamat));
    fclose(f);
    return OK;
}

int addUgyfel(ugyfel * ujUgyfel) {
    if(!listaStart->ugyfel) { // üres a lista
        listaStart->ugyfel = ujUgyfel;
        return OK;
    }
    ugyfel_elem * last;
    FOREACH(if(!current->next) last = current);
    ugyfel_elem * ujElem = malloc(sizeof(ugyfel_elem));
    if(!ujElem) return ALLOC_FAIL; // nem lehet lefoglalni memóriát
    last->next = ujElem;
    ujElem->ugyfel = ujUgyfel;
    ujElem->next = NULL;
    ujElem->prev = last;
    return OK;
}

inline int nevExists(string * nev) {
    return validateNev(nev) && get(nev) ? OK : UGYFEL_NOT_EXISTS;
}

int send() {

    char _fromNev[NEV_LEN], _toNev[NEV_LEN], _amount[9];
    string fromNev, toNev, amountStr;

    if(!tryUntilOK(_fromNev, "Utalo neve: ", "%30s", &nevExists, &fromNev) ||
       !tryUntilOK(_toNev, "Kedvezmenyezett neve: ", "%30s", &nevExists, &toNev) ||
       !tryUntilOK(_amount, "Utalando osszeg: ", "%9s", &validateEgyenleg, &amountStr)
    ) return GAVE_UP;

    ugyfel_elem * from = get(&fromNev), * to = get(&toNev);
    int amount = stoi(&amountStr);

    if(from->ugyfel->egyenleg < amount) return NO_BALANCE;
    from->ugyfel->egyenleg -= amount;
    to->ugyfel->egyenleg += amount;
    return SEND_OK;
}

int newUgyfel() {

    char _email[EMAIL_LEN], _telefon[TEL_LEN], _nev[NEV_LEN];
    char _egyenleg[10], _kamat[4];

    string email, telefon, nev, egyenleg, kamat;

    if(!tryUntilOK(_email,   "Email cim: ",    "%30s", &validateEmail,   &email) ||
       !tryUntilOK(_telefon, "Telefonszam: ",  "%30s", &validateTelefon, &telefon) ||
       !tryUntilOK(_nev,     "Nev: ",          "%30s", &validateNev,     &nev) ||
       !tryUntilOK(_egyenleg,"Egyenleg: ",     "%9s",  &validateEgyenleg,&egyenleg) ||
       !tryUntilOK(_kamat,   "Eves kamat: ",   "%3s",  &validateKamat,   &kamat)) return GAVE_UP;

    ugyfel * ujElem = malloc(sizeof(ugyfel));
    if(!ujElem) return ALLOC_FAIL;
    *ujElem = newUgyfel_str(nev, email, telefon, stoi(&egyenleg), aktualisDatum, stoi(&kamat));
    if(addUgyfel(ujElem) > OK) {
        free(ujElem);
        return ALLOC_FAIL;
    }
    return NEW_UGYFEL_OK;

}

ugyfel_elem * get(string * nev) {
    FOREACH(if(stringEquals(&u.nev, nev)) return current);
    return NULL;
}

void listUgyfel() {
    printf("Ugyfelek listája:\n\n");
    printf(UGYFEL_HEADER_FORMAT, "Nev", "Email", "Telefonszam", "Egyenleg", "Lekotes datuma", "Eves kamat (%)");
    printf(SEPARATOR);
    FOREACH(printf(UGYFEL_FORMAT, u.nev.chars, u.email.chars, u.telefon.chars, u.egyenleg, u.lekotesDatum.chars, u.kamat))
}

int printPersonal() {
    READ_UGYFEL
    printf("\n%s penzugyi adatai:\n", nev.chars);
    printf(SEPARATOR);
    printf("Lekotes ideje: %-20s", elem->ugyfel->lekotesDatum.chars);
    printf("Eves kamat: %-3d", elem->ugyfel->kamat);
    printf("Nyito egyenleg: %d\n", elem->ugyfel->egyenleg);
    double kamattal = elem->ugyfel->egyenleg, evek = newDate(&aktualisDatum).year - newDate(&elem->ugyfel->lekotesDatum).year;
    for(int i = 0; i < evek; i++) kamattal *= 1 + (elem->ugyfel->kamat / 100.0F);
    printf("Jelenlegi - kamattal novelt - egyenlege: %d\n", (int) ++kamattal);
    printf(SEPARATOR);
    return OK;
}

int deleteUgyfel() {
    READ_UGYFEL
    char i_n = 'n';
    printf("Biztos torli \"%s\" nevu ugyfelet? (i/n) ", nev.chars);
    scanf(" %c", &i_n);
    if(i_n == 'i') {
        if(elem == listaStart) // ha a legelső elemet akarja a felhasználó törölni
            if(elem->next)
                listaStart = elem->next;
            else {
                listaStart->ugyfel = NULL;
                listaStart->next = NULL;
                listaStart->prev = NULL;
            }
        if(elem->prev) elem->prev->next = elem->next;
        if(elem->next) elem->next->prev = elem->prev;
        if(elem->ugyfel) deallocUgyfel(elem);
        return UGYFEL_DELETED;
    }
    return GAVE_UP;
}

void showUgyfelEgyenlegGraph() {

    int count = 0, n = 0, max = 0;
    {FOREACH(count++)} // scope limit
    int egyenlegek[count];
    string nevek[count];
    FOREACH(nevek[n] = u.nev;
            egyenlegek[n++] = u.egyenleg;
            if(u.egyenleg > max) max = u.egyenleg);
    int unit = max / ROWS_IN_GRAPH;
    int ugyfelSorok[count];
    for(int i = 0; i < count; i++) ugyfelSorok[i] = egyenlegek[i] / unit;

    printf("\nUgyfelek egyenleg statisztikaja\n\n");

    for(int sor = ROWS_IN_GRAPH; sor >= 0; sor--) {
        bool margo = false;
        for(int oszlop = -1; oszlop < count * 3; oszlop++) {
            if(oszlop == -1) { // bal margó
                int i = -1;
                while(++i < count)
                    if(ugyfelSorok[i] == sor) {
                        printf("%9d|------------", egyenlegek[i]);
                        margo = true;
                        break; // csak egyszer van kiírva, ha több egyenleg végződne ebben a sorban
                    }
                if(i == count) printf("         |            "); // nem végződött ebben a sorban egy egyenleg sem
            }else printf(ugyfelSorok[oszlop / 3] >= sor ? ((oszlop + 1) / 3 > oszlop / 3 ? FILL[margo][0] : "O") : FILL[margo][1]);
        }
        printf("\n");
    }

    printf("---------|------------");
    for(int i = 0; i < count * 3; i++) printf("-----");
    printf("\n         |      ");
    for(int i = 0, j; i < count; i++) {
        for(j = 0; j < 7 - (nevek[i].length / 2); j++) printf(" "); 
        printf("%*s", -15 + j, nevek[i].chars); // -15 (teljes hossz) + j (bal padding) 
        j = 0;
    }
}

void deallocUgyfel(ugyfel_elem * elem) {
    free(elem->ugyfel->nev.chars);
    free(elem->ugyfel->email.chars);
    free(elem->ugyfel->telefon.chars);
    free(elem->ugyfel);
    free(elem);
}

void dealloc() {
    if(!listaStart->ugyfel) { // üres a lista
        free(listaStart);
        return;
    }
    ugyfel_elem * current = listaStart, * tmp;
    while(current) {
        tmp = current->next;
        deallocUgyfel(current);
        current = tmp;
    }
}

void kilep(bool askFile) {

    if(askFile) {
        char i_n = 'n';
        printf("Az ugyfeleket nyilvantarto lista adatai megvaltozhattak.\n\nMenti a valtozasokat? (i/n) ");
        scanf(" %c", &i_n);
        if(i_n == 'i') writeFile();
    }

    dealloc();
    printf("A program kilep.\n");
    exit(0);
}

void showOptions() {
    printf("\n\nValaszthato opciok:\n\n");
    printf(SEPARATOR);
    printf("\n1: Ugyfellista    2: Utalas    3: Uj ugyfel    4: Ugyfel torlese    5: Sajat penzugyek    6: Statisztika    7: Kilepes \n");
    printf(SEPARATOR);
}