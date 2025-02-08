#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ID_LEN 8
#define MAX_NAME_LEN 64

// Struktura za datum
struct _date;
typedef struct _date *DateP;
typedef struct _date {
    int year;
    int month;
    int day;
} Date;

// Struktura za vozilo
struct _vehicle;
typedef struct _vehicle *VehicleP;
typedef struct _vehicle {
    char brandName[MAX_NAME_LEN];
    char modelName[MAX_NAME_LEN];
    DateP date;
    VehicleP next;
} Vehicle;

// Struktura za osobu
struct _person;
typedef struct _person* PersonP;
typedef struct _person {
    char id[MAX_ID_LEN];
    char lastName[MAX_NAME_LEN];
    PersonP next;
    VehicleP child;
} People;

// Funkcija za umetanje osobe u listu sortiranu po prezimenu i imenu
PersonP insertPerson(PersonP head, People* newPerson) {
    PersonP current = head, prev = NULL;

    // Ako je lista prazna ili novo ime treba biti na početku
    if (head == NULL || strcmp(newPerson->lastName, head->lastName) < 0 || 
        (strcmp(newPerson->lastName, head->lastName) == 0 && strcmp(newPerson->id, head->id) < 0)) {
        newPerson->next = head;
        return newPerson;
    }

    // Pronađi odgovarajuće mjesto za umetanje
    while (current != NULL && (strcmp(newPerson->lastName, current->lastName) > 0 || 
           (strcmp(newPerson->lastName, current->lastName) == 0 && strcmp(newPerson->id, current->id) > 0))) {
        prev = current;
        current = current->next;
    }

    // Umetni novu osobu
    newPerson->next = current;
    if (prev != NULL) {
        prev->next = newPerson;
    }

    return head;
}

// Funkcija za učitavanje osoba iz datoteke
PersonP loadPeople(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file %s.\n", filename);
        return NULL;
    }

    PersonP head = NULL;
    People tempPerson;
    while (fscanf(file, "%s %s", tempPerson.id, tempPerson.lastName) == 2) {
        tempPerson.child = NULL;
        tempPerson.next = NULL;
        head = insertPerson(head, &tempPerson);
    }

    fclose(file);
    return head;
}

// Funkcija za umetanje vozila u listu vozila osobe
VehicleP insertVehicle(VehicleP head, Vehicle* newVehicle) {
    newVehicle->next = head;
    return newVehicle;
}

// Funkcija za učitavanje vozila iz datoteke
void loadVehicles(PersonP people, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file %s.\n", filename);
        return;
    }

    char personId[MAX_ID_LEN];
    Vehicle tempVehicle;
    PersonP person;

    while (fscanf(file, "%s %s %s %d %d %d", personId, tempVehicle.brandName, tempVehicle.modelName,
                  &tempVehicle.date->year, &tempVehicle.date->month, &tempVehicle.date->day) == 6) {
        // Potraži osobu s odgovarajućim ID-em
        person = people;
        while (person != NULL && strcmp(person->id, personId) != 0) {
            person = person->next;
        }

        // Ako osoba postoji, dodaj vozilo
        if (person != NULL) {
            tempVehicle.next = NULL;
            tempVehicle.date = malloc(sizeof(Date));
            *tempVehicle.date = tempVehicle.date;
            person->child = insertVehicle(person->child, &tempVehicle);
        }
    }

    fclose(file);
}

// Funkcija za ispis svih osoba i njihovih vozila
void printPeopleAndVehicles(PersonP head) {
    PersonP person = head;
    while (person != NULL) {
        printf("Osoba: %s %s\n", person->lastName, person->id);
        if (person->child != NULL) {
            VehicleP vehicle = person->child;
            while (vehicle != NULL) {
                printf("    Vozilo: %s %s, Registracija: %d-%d-%d\n", vehicle->brandName, vehicle->modelName,
                       vehicle->date->year, vehicle->date->month, vehicle->date->day);
                vehicle = vehicle->next;
            }
        } else {
            printf("    NEMA VOZILA\n");
        }
        person = person->next;
    }
}

// Glavni program
int main() {
    PersonP people = loadPeople("people.txt");
    loadVehicles(people, "vehicle.txt");
    printPeopleAndVehicles(people);

    // Oslobađanje memorije, zatvaranje datoteka itd. ide ovdje

    return 0;
}
