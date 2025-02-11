#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATE_LEN 11

typedef struct _temperature* TemperatureP;
typedef struct _temperature {
    char date[MAX_DATE_LEN]; 
    float depth;              
    float temperature;         
    TemperatureP next;         
} Temperature;

TemperatureP createElement(char* date, float depth, float temperature);
void insertSorted(TemperatureP head, char* date, float depth, float temperature);
void printList(TemperatureP head);
double averageTemperature(TemperatureP head);
void removeBelowAvg(TemperatureP head, double avgTemp);
void freeList(TemperatureP head);

int main() {
    TemperatureP head = malloc(sizeof(Temperature));
    if (!head) return -1;

    head->next = NULL;

    FILE* file = fopen("temperature.txt", "r");
    if (!file) {
        printf("Greska pri otvaranju datoteke!\n");
        return -1;
    }

    char date[MAX_DATE_LEN];
    float depth, temperature;

    while (fscanf(file, "%s %f %f", date, &depth, &temperature) == 3) {
        insertSorted(head, date, depth, temperature);
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

TemperatureP createElement(char* date, float depth, float temperature) {
    TemperatureP newElement = malloc(sizeof(Temperature));
    if (!newElement) {
        printf("Greska pri alociranju memorije!\n");
        return NULL;
    }

    strcpy(newElement->date, date);
    newElement->depth = depth;
    newElement->temperature = temperature;
    newElement->next = NULL;

    return newElement;
}

void insertSorted(TemperatureP head, char* date, float depth, float temperature) {
    TemperatureP newElement = createElement(date, depth, temperature);
    if (!newElement) return;

    TemperatureP curr = head;
    while (curr->next != NULL && strcmp(curr->next->date, date) < 0) {
        curr = curr->next;
    }

    newElement->next = curr->next;
    curr->next = newElement;
}

void printList(TemperatureP head) {
    TemperatureP curr = head->next;
    int i = 1;
    while (curr != NULL) {
        printf("%d. Mjerenje: \nDatum: %s\nDubina: %.2f m\nTemperatura: %.2f C\n\n", i, curr->date, curr->depth, curr->temperature);
        i++;
        curr = curr->next;
    }
}

double averageTemperature(TemperatureP head) {
    TemperatureP curr = head->next;
    double totalTemp = 0;
    int count = 0;

    while (curr != NULL) {
        if (curr->depth > 50 && strncmp(curr->date, "2023", 4) == 0) {
            totalTemp += curr->temperature;
            count++;
        }
        curr = curr->next;
    }

    if (count > 0) {
        return totalTemp / count;
    }
    else {
        return 0;
    }
}

void removeBelowAvg(TemperatureP head, double avgTemp) {
    TemperatureP curr = head->next;
    TemperatureP prev = head;

    while (curr != NULL) {
        if (curr->temperature < avgTemp) {
            prev->next = curr->next;
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
        free(temp);
    }
}
