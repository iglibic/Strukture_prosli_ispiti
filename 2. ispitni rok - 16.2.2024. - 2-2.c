#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct _date* DateP;
typedef struct _date {
    int day;
    int month;
    int year;
} Date;

typedef struct _temperature* TemperatureP;
typedef struct _temperature {
    int depth;
    DateP dateOfMeasurments;
    TemperatureP next;
} Temperature;

TemperatureP createElement(int day, int month, int year, int depth);
int insertSorted(TemperatureP head, int day, int month, int year, int depth);
void printList(TemperatureP head);
double averageTemperature(TemperatureP head);
void removeBelowAvg(TemperatureP head, double avgTemp);
void freeList(TemperatureP head);
int compareDates(DateP d1, DateP d2);

int main() {
    TemperatureP head = malloc(sizeof(Temperature));
    if (!head) return -1;
    head->next = NULL;

    FILE* file = fopen("temperature.txt", "r");
    if (!file) {
        printf("Greska pri otvaranju datoteke!\n");
        return -1;
    }

    int day, month, year, depth;

    while (fscanf(file, "%d %d %d %d", &day, &month, &year, &depth) == 4) {
        insertSorted(head, day, month, year, depth);
    }

    fclose(file);

    printf("Sva mjerenja:\n");
    printList(head);

    double avgTemp = averageTemperature(head);
    printf("\nProsjecna temperatura u 2023. godini na dubinama vecim od 50 metara: %.2f\n", avgTemp);

    removeBelowAvg(head, avgTemp);
    printf("\nLista nakon brisanja mjerenja ispod prosjecne temperature:\n");
    printList(head);

    freeList(head);
    return 0;
}

TemperatureP createElement(int day, int month, int year, int depth) {
    TemperatureP newElement = malloc(sizeof(Temperature));
    if (!newElement) {
        printf("Greska pri alociranju memorije!\n");
        return NULL;
    }

    newElement->dateOfMeasurments = malloc(sizeof(Date));
    if (!newElement->dateOfMeasurments) {
        printf("Greska pri alociranju memorije!\n");
        free(newElement);
        return NULL;
    }

    newElement->dateOfMeasurments->day = day;
    newElement->dateOfMeasurments->month = month;
    newElement->dateOfMeasurments->year = year;
    newElement->depth = depth;
    newElement->next = NULL;

    return newElement;
}

int insertSorted(TemperatureP head, int day, int month, int year, int depth) {
    TemperatureP newElement = createElement(day, month, year, depth);
    if (!newElement) return -1;

    TemperatureP curr = head;
    
    while (curr->next != NULL && compareDates(curr->next->dateOfMeasurments, newElement->dateOfMeasurments) < 0) {
        curr = curr->next;
    }

    newElement->next = curr->next;
    curr->next = newElement;

    return 0;
}

void printList(TemperatureP head) {
    TemperatureP curr = head->next;
    int i = 1;
    while (curr != NULL) {
        printf("%d. Mjerenje: \nDatum: %02d-%02d-%d\nDubina: %d m\n\n", 
               i, curr->dateOfMeasurments->day, curr->dateOfMeasurments->month, 
               curr->dateOfMeasurments->year, curr->depth);
        i++;
        curr = curr->next;
    }
}

double averageTemperature(TemperatureP head) {
    TemperatureP curr = head->next;
    double totalTemp = 0;
    int count = 0;

    while (curr != NULL) {
        if (curr->depth > 50 && curr->dateOfMeasurments->year == 2023) {
            totalTemp += curr->depth;  
            count++;
        }
        curr = curr->next;
    }

    return count > 0 ? totalTemp / count : 0;
}

void removeBelowAvg(TemperatureP head, double avgTemp) {
    TemperatureP curr = head->next;
    TemperatureP prev = head;

    while (curr != NULL) {
        if (curr->depth < avgTemp) {
            prev->next = curr->next;
            free(curr->dateOfMeasurments);
            free(curr);
            curr = prev->next;
        }
        else {
            prev = curr;
            curr = curr->next;
        }
    }
}

void freeList(TemperatureP head) {
    TemperatureP curr = head;
    while (curr != NULL) {
        TemperatureP temp = curr;
        curr = curr->next;
        free(temp->dateOfMeasurments);
        free(temp);
    }
}

int compareDates(DateP d1, DateP d2) {
    if (d1->year != d2->year) return d1->year - d2->year;
    if (d1->month != d2->month) return d1->month - d2->month;
    return d1->day - d2->day;
}
