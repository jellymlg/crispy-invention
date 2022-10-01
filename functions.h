#ifndef FUN
#define FUN

#include <stdlib.h>
#include <stdio.h>

#define true 1
#define false 0
#define bool char

#define clr() system("clear")
#define stoi(str) _stoi(str, 0)
#define printMessage(code) _printMessage(code, true)

#define FOREACH(ins) ugyfel_elem * current = listaStart; \
                     ugyfel u; \
                     while(current && current->ugyfel) { \
                        u = *current->ugyfel; \
                        ins; \
                        current = current->next; \
                     }

#define READ_UGYFEL char _nev[31]; \
                    string nev; \
                    if(!tryUntilOK(_nev, "Ugyfel neve: ", "%30s", &nevExists, &nev)) return GAVE_UP; \
                    ugyfel_elem * elem =  get(&nev);

// Segítő struct stringekhez
typedef struct {
    int length;
    char * chars;
} string;

// string -> int
int _stoi(string * str, int skip);

// Új karakterlánc létrehozása
string newString(char * data);

// Két string összehasonlítása
bool stringEquals(string * _1, string * _2);

typedef struct {
    short year;
    char month;
    char day;
} date;

// Hónapok napjai
char month[] = {
    31,
    28, // +1 if isLeapYear
    31,
    30,
    31,
    30,
    31,
    31,
    30,
    31,
    30,
    31
};

// Új dátum létrehozása stringből
static inline date newDate(string * str);

// Szökőév-e az adott dátum
static inline bool isLeapYear(date * datum);

// Napok száma egy dátumban lévő hónapban
static inline char getMonthLength(date * datum);

// Aktuális dátum
string aktualisDatum;

// Beolvasott fájl neve
string openedFile;

// Fájl formátuma
const char FILE_FORMAT[] = "%s\t%s\t%s\t%d\t%s\t%d\n";

// Ügyfelek formátuma
const char UGYFEL_HEADER_FORMAT[] = "%-30s%-30s%-15s%-15s%-20s%-15s\n";
const char UGYFEL_FORMAT[]        = "%-30s%-30s%-15s%-15d%-20s%-15d\n";

// Hosszabb sorok elválasztására
const char SEPARATOR[] = "________________________________________________________________________________________________________________________\n";

// Hibaüzenetek
const char * ERR_MSG[] = {

    "A nev nem lehet ures!",

    "Az email cimnek hosszabbnak kell lennie!",
    "Az email cim nem kezdodhet '.'-tal!",
    "Az email cim nem kezdodhet '@'-al!",
    "Az email cim nem végzodhet '.'-tal!",
    "Az email cim nem végzodhet '@'-al!",
    "Az email cimben nem kovetheti '.' a '@'-ot!",
    "Az email cimben nem kovetheti '@' a '.'-ot!",
    "Az email cim nem tartalmazott '.'-ot vagy '@'-ot!",
    "Az email cim csak egy '@'-ot tartalmazhat!",
    "Az email cim csak egy '.'-ot tartalmazhat!",

    "A telefonszam nem lehet ures!",
    "A telefonszam csak numerikus karaktereket tartalmazhat!",

    "A datum formatuma nem megfelelo!",
    "A megadott ev nem megfelelo!",
    "A megadott honap nem megfelelo!",
    "A megadott nap nem megfelelo!",

    "A kamat csak számokat tartalmazhat!",
    "A kamat 0 és 100 közötti szám kell legyen!",

    "Az egyenleg nem lehet ures!",

    "A fajlt nem sikerult megnyitni!",
    "A fajl hibás adatokat tartalmaz!",

    "Nem sikerult lefoglalni memoriat!",

    "Nincs ilyen nevu ugyfel!",
    "Az utalo ugyfelnek nincs megfelelo egyenlege!",

    "Nem letezik ilyen nevu ugyfel!"

};

// Általános üzenetek
const char * OK_MSG[] = {

    "Fajl beolvasva.",

    "Osszeg atutalva.",

    "Uj ugyfel rogzitve.",

    "Felhasznalo torolve.",

    "Megszakitva."

};

// Státuszkódok
enum msgcode {

    GAVE_UP = -6,

    UGYFEL_DELETED = -5,

    NEW_UGYFEL_OK = -4,

    SEND_OK = -3,

    FILE_READ_OK = -2,

    OK = -1,

    NEV_EMPTY,

    EMAIL_SHORT,
    EMAIL_START_DOT,
    EMAIL_START_AT,
    EMAIL_END_DOT,
    EMAIL_END_AT,
    EMAIL_DOT_AFTER_AT,
    EMAIL_AT_AFTER_DOT,
    EMAIL_MISSING_DOT_OR_AT,
    EMAIL_TWO_AT,
    EMAIL_TWO_DOT,

    TEL_EMPTY,
    TEL_BAD_CHARS,

    DATE_BAD_FORMAT,
    DATE_INVALID_YEAR,
    DATE_INVALID_MONTH,
    DATE_INVALID_DAY,

    KAMAT_LETTER,
    KAMAT_OUT_OF_RANGE,

    EGYENLEG_EMPTY,

    FILE_OPEN,
    FILE_BAD_FORMAT,

    ALLOC_FAIL,
    
    SEND_NO_EXIST,
    NO_BALANCE,

    UGYFEL_NOT_EXISTS

};

char * ERR_ASK[] = {
    "HIBA: %s\n",
    "HIBA: %s Megprobalja ujra? (i/n) "
};

// Üzenet kiírása státuszkód alapján
void _printMessage(int code, bool askAgain);

// Biztonságos memóriafoglalás
void checkAlloc(char ** ptr, size_t size, char ** dealloc, size_t deallocSize);

// msg üzenet kiírása, majd format alapján beolvasás amíg verify hibakódot ad, vagy a felhasználó ki nem lép
bool tryUntilOK(char * buffer, const char * message, const char * format, int (*verify)(string *), string * result);

// Eldönti, hogy numerikus-e egy karakter
static inline bool isDigit(char c);

// Az ügyfelek információi
typedef struct {
    string nev;
    string email;
    string telefon;
    int egyenleg;
    string lekotesDatum;
    int kamat;
} ugyfel;

const char NEV_LEN = 31, EMAIL_LEN = 31, TEL_LEN = 31, DATUM_LEN = 12;

// Ugyfel konstruktor
ugyfel newUgyfel_str(string nev, string email, string tel, int egyenleg, string datum, int kamat);

// A nevek nem lehetnek üresek
int validateNev(string * nev);

// Egy email címben egy '.' és '@' karakter lehet, de nem egymás mellett, és nem a cím elején/végén
int validateEmail(string * email);

// Egy telefonszámban csak szám karakterek lehetnek
int validateTelefon(string * telefon);

// Minden dátumnak a "YYYY.MM.DD." formátumot kell követnie
int validateDatum(string * datum);

// A kamatnak a [0-100] intervallumba kell tartoznia
int validateKamat(string * kamat);

// Az egyenleg 9 jegyű egész szám lehet
int validateEgyenleg(string * egyenleg);

// Ügyfél láncolt lista elem
typedef struct ugyfel_elem {
    ugyfel * ugyfel;
    struct ugyfel_elem * next;
    struct ugyfel_elem * prev;
} ugyfel_elem;

// Láncolt lista első eleme
ugyfel_elem * listaStart;

// Új ügyfél hozzáadása a láncolt listához
int addUgyfel(ugyfel * ujUgyfel);

// Jó formátumú-e a név és létezik-e ügyfél ilyen névvel
static inline int nevExists(string * nev);

// Egyenleg küldése két ügyfél között
int send();

// Új ügyfél adatainak bekérése
int newUgyfel();

// Ügyfelek listázása
void listUgyfel();

// Egy ügyfél adatainak kiírása
int printPersonal();

// Ügyfél törlése
int deleteUgyfel();

// Sorok az egyenlegek megjelenítésére
const int ROWS_IN_GRAPH = 20;

// Grafikon kitöltés
const char * FILL[2][2] = {{"O            ", "     "}, {"O------------", "-----"}};

// Egyenleg grafikon megjelenítése
void showUgyfelEgyenlegGraph();

// Ha tartalmaz a láncolt lista ilyen nevvél rekordot, azt visszaadja, különben NULL
ugyfel_elem * get();

// Egy ügyfél elem deallokálása
void deallocUgyfel(ugyfel_elem * elem);

// Deallokálja a láncolt listát
void dealloc();

// Kilépés a programból
void kilep(bool askFile);

// Beolvas egy szöveges állományt a megadott névvel (ha lehetséges) és a benne található adatokból láncolt listát készít
int readAndParseFile(string * filename);

// Meglévő adatok kiírása egy állományba
int writeFile();

// Választható opciók megjelenítése
void showOptions();

#include "functions.c"

#endif