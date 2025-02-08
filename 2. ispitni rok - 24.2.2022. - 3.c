#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ID_LEN (8)
#define MAX_NAME_LEN (64)

struct _date;
typedef struct _date *DateP;
typedef struct _date {
    int year;
    int month;
    int day;
} Date;

struct _vehicle;
typedef struct _vehicle *VehicleP;
typedef struct _vehicle {
    char brandName[MAX_NAME_LEN];
    char modelName[MAX_NAME_LEN];
    DateP date; // Datum registracije
    VehicleP next;
} Vehicle;

struct _person;
typedef struct _person* PersonP;
typedef struct _person {
    char id[MAX_NAME_LEN];
    char lastName[MAX_NAME_LEN];
    PersonP next;
    VehicleP child;  // Lista vozila ove osobe
} People;

// Funkcija za usporedbu datuma
int isExpired(Date* regDate) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    if (regDate->year < tm->tm_year + 1900) return 1; // Registracija istekla ako je godina manja
    if (regDate->year == tm->tm_year + 1900) {
        if (regDate->month < tm->tm_mon + 1) return 1;  // Registracija istekla ako je mjesec manji
        if (regDate->month == tm->tm_mon + 1) {
            if (regDate->day < tm->tm_mday) return 1;  // Registracija istekla ako je dan manji
        }
    }
    return 0;  // Registracija nije istekla
}

// Funkcija za ispisivanje podataka o osobama i vozilima
void printPersonsAndVehicles(PersonP peopleList) {
    PersonP currentPerson = peopleList;
    while (currentPerson != NULL) {
        printf("Osoba: %s %s\n", currentPerson->lastName, currentPerson->id);

        if (currentPerson->child == NULL) {
            printf("  NEMA VOZILA\n");
        } else {
            VehicleP currentVehicle = currentPerson->child;
            while (currentVehicle != NULL) {
                printf("  Vozilo: %s %s, Registracija: %d-%d-%d\n",
                    currentVehicle->brandName, currentVehicle->modelName,
                    currentVehicle->date->day, currentVehicle->date->month, currentVehicle->date->year);
                currentVehicle = currentVehicle->next;
            }
        }
        currentPerson = currentPerson->next;
    }
}

// Funkcija za ispis vozila s isteklim registracijama
void printExpiredVehicles(PersonP peopleList) {
    PersonP currentPerson = peopleList;
    printf("\nVozila s isteklim registracijama:\n");
    while (currentPerson != NULL) {
        VehicleP currentVehicle = currentPerson->child;
        while (currentVehicle != NULL) {
            if (isExpired(currentVehicle->date)) {
                printf("Osoba: %s %s, Vozilo: %s %s, Registracija istekla: %d-%d-%d\n",
                    currentPerson->lastName, currentPerson->id,
                    currentVehicle->brandName, currentVehicle->modelName,
                    currentVehicle->date->day, currentVehicle->date->month, currentVehicle->date->year);
            }
            currentVehicle = currentVehicle->next;
        }
        currentPerson = currentPerson->next;
    }
}

// Funkcija za učitavanje osoba iz datoteke
PersonP loadPeople(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Ne mogu otvoriti datoteku %s!\n", filename);
        return NULL;
    }

    PersonP head = NULL, tail = NULL;
    while (!feof(file)) {
        PersonP newPerson = (PersonP)malloc(sizeof(People));
        if (fscanf(file, "%s %s", newPerson->id, newPerson->lastName) != 2) {
            free(newPerson);
            break;
        }
        newPerson->child = NULL;
        newPerson->next = NULL;

        if (tail) {
            tail->next = newPerson;
            tail = newPerson;
        } else {
            head = tail = newPerson;
        }
    }
    fclose(file);
    return head;
}

// Funkcija za učitavanje vozila iz datoteke
void loadVehicles(const char* filename, PersonP peopleList) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Ne mogu otvoriti datoteku %s!\n", filename);
        return;
    }

    char personId[MAX_ID_LEN];
    while (!feof(file)) {
        char brand[MAX_NAME_LEN], model[MAX_NAME_LEN];
        int day, month, year;

        if (fscanf(file, "%s %s %s %d-%d-%d", personId, brand, model, &day, &month, &year) != 6) {
            break;
        }

        // Pronađi osobu prema ID-u
        PersonP person = peopleList;
        while (person != NULL && strcmp(person->id, personId) != 0) {
            person = person->next;
        }

        if (person) {
            VehicleP newVehicle = (VehicleP)malloc(sizeof(Vehicle));
            strcpy(newVehicle->brandName, brand);
            strcpy(newVehicle->modelName, model);
            newVehicle->date = (DateP)malloc(sizeof(Date));
            newVehicle->date->day = day;
            newVehicle->date->month = month;
            newVehicle->date->year = year;
            newVehicle->next = person->child;
            person->child = newVehicle;
        }
    }
    fclose(file);
}

int main() {
    // Učitaj osobe i vozila iz datoteka
    PersonP peopleList = loadPeople("people.txt");
    loadVehicles("vehicle.txt", peopleList);

    // Ispis svih osoba i njihovih vozila
    printPersonsAndVehicles(peopleList);

    // Ispis vozila s isteklim registracijama
    printExpiredVehicles(peopleList);

    // Oslobađanje memorije
    PersonP currentPerson = peopleList;
    while (currentPerson != NULL) {
        PersonP tempPerson = currentPerson;
        currentPerson = currentPerson->next;

        VehicleP currentVehicle = tempPerson->child;
        while (currentVehicle != NULL) {
            VehicleP tempVehicle = currentVehicle;
            currentVehicle = currentVehicle->next;
            free(tempVehicle->date);
            free(tempVehicle);
        }
        free(tempPerson);
    }

    return 0;
}
