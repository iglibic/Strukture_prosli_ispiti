#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define maks 32

typedef struct kategorija* cvor;
typedef struct proizvod* cvor_pro;

typedef struct kategorija {
    char ime_kategorije[maks];
    int price_min;
    int price_max;
    double avgCijena;
    cvor next;
    cvor_pro proizvod_head;
} kategorija;

typedef struct proizvod {
    char proizvod_ime[maks];
    int cjena;
    cvor_pro next;
} proizvod;

void unos(cvor, char*, int, int);
void unos_proizvod(cvor_pro, char*);
void ispis(cvor);
void radnom_dodjeli(cvor_pro, int, int);
void give_rand(cvor);
void avg_price(cvor);
void prebaci_kategoriju(cvor);

int main() {
    srand(time(NULL));

    cvor head = malloc(sizeof(kategorija));
    if (!head) return -1;
    head->next = NULL;

    // Unos kategorija iz datoteke
    FILE* file = fopen("kategorije.txt", "r");
    if (!file) return -1;

    char temp_name[maks];
    int min, max;
    while (fscanf(file, "%s %d %d", temp_name, &min, &max) == 3) {
        unos(head, temp_name, min, max);
    }
    fclose(file);

    // Generiranje slučajnih cijena i prosječne cijene
    give_rand(head);

    // Ispis podataka - ispišite samo jednom
    ispis(head);

    return 0;
}

void unos(cvor head, char* ime_kategorije, int min, int max) {
    cvor novi = malloc(sizeof(kategorija));
    if (!novi) return;
    novi->next = NULL;
    novi->price_min = min;
    novi->price_max = max;
    strcpy(novi->ime_kategorije, ime_kategorije);

    // Sortiranje kategorija prema abecedi
    cvor curr = head;
    while (curr->next != NULL && strcmp(curr->next->ime_kategorije, ime_kategorije) < 0) {
        curr = curr->next;
    }

    novi->next = curr->next;
    curr->next = novi;

    // Inicijalizacija liste proizvoda za kategoriju
    novi->proizvod_head = malloc(sizeof(proizvod));
    if (!novi->proizvod_head) return;
    novi->proizvod_head->next = NULL;

    // Unos proizvoda iz datoteke
    FILE* file = fopen("proizvodi.txt", "r");
    if (!file) return;

    char kat_tmp[maks];
    char prozvod_tmp[maks];

    while (fscanf(file, "%s %s", kat_tmp, prozvod_tmp) == 2) {
        if (strcmp(novi->ime_kategorije, kat_tmp) == 0) {
            unos_proizvod(novi->proizvod_head, prozvod_tmp);
        }
    }
    fclose(file);
}

void unos_proizvod(cvor_pro head, char* ime_proizvoda) {
    cvor_pro novi = malloc(sizeof(proizvod));
    if (!novi) return;

    strcpy(novi->proizvod_ime, ime_proizvoda);
    novi->next = head->next;
    head->next = novi;
}

void ispis(cvor head) {
    cvor curr = head->next;
    while (curr != NULL) {
        printf("Kategorija: %s\n", curr->ime_kategorije);

        cvor_pro currp = curr->proizvod_head->next;
        while (currp != NULL) {
            printf("%s - %d\n", currp->proizvod_ime, currp->cjena);
            currp = currp->next;
        }

        printf("\n");
        curr = curr->next;
    }
}

void radnom_dodjeli(cvor_pro head, int min, int max) {
    cvor_pro curr = head->next;
    int* zauzeti = malloc((max - min) * sizeof(int));
    if (!zauzeti) return;

    memset(zauzeti, 0, (max - min) * sizeof(int));

    while (curr != NULL) {
        int trentuni;
        do {
            trentuni = min + rand() % (max - min);
        } while (zauzeti[trentuni - min]);

        zauzeti[trentuni - min] = 1;
        curr->cjena = trentuni;

        curr = curr->next;
    }

    free(zauzeti);
}

void give_rand(cvor head) {
    cvor curr = head->next;
    while (curr != NULL) {
        radnom_dodjeli(curr->proizvod_head, curr->price_min, curr->price_max);
        curr = curr->next;
    }
}
