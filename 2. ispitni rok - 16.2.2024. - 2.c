#include <stdio.h>
#include <stdlib.h>

struct _date;
typedef struct _date* DateP;
typedef struct _date {
    int day;
    int month;
    int year;
} Date;

struct _temperature;
typedef struct _temperature* TemperatureP;
typedef struct _temperature {
    int depth;
    int temperature;
    DateP dateOfMeasurments;
    TemperatureP next;
};

// Funkcije koje se koriste u programu
int ReadFile(TemperatureP head, char* filename);
int MakeList(TemperatureP head, int dubina, int temp, int dan, int mjesec, int godina);
TemperatureP CreateT(int dubina, int temp, int dan, int mjesec, int godina);
int PrintList(TemperatureP head);
float ProsjecnaTemperatura(TemperatureP head);
int DeleteTemp(TemperatureP head, float prosjecna);
void FreeList(TemperatureP head);

int main() {
    Temperature head = { .depth = 0, .dateOfMeasurments = NULL, .next = NULL };
    float prosjecnaTemp;

    // Čitanje podataka iz datoteke
    ReadFile(&head, "temperature.txt");
    
    // Ispisivanje originalne liste
    printf("Originalna lista:\n");
    PrintList(head.next);

    // Izračun prosječne temperature i brisanje mjerenja manjih od prosječne temperature
    prosjecnaTemp = ProsjecnaTemperatura(head.next);
    printf("\nProsjecna temperatura u 2023. godini i na dubini >50 m je %.2f\n", prosjecnaTemp);
    DeleteTemp(&head, prosjecnaTemp);
    
    // Ispisivanje liste nakon brisanja
    printf("\nLista nakon brisanja mjerenja manjih od prosječne temperature:\n");
    PrintList(head.next);

    // Oslobađanje memorije
    FreeList(head.next);

    return 0;
}

int ReadFile(TemperatureP head, char* filename) {
    FILE* fptr = fopen(filename, "r");
    if (!fptr) {
        printf("File couldn't be opened\n");
        return EXIT_FAILURE;
    }

    int dan, mjesec, godina, dubina, temp;
    while (fscanf(fptr, "%d.%d.%d %d %d", &dan, &mjesec, &godina, &dubina, &temp) == 5) {
        MakeList(head, dubina, temp, dan, mjesec, godina);
    }

    fclose(fptr);
    return EXIT_SUCCESS;
}

int MakeList(TemperatureP head, int dubina, int temp, int dan, int mjesec, int godina) {
    TemperatureP newTemp = CreateT(dubina, temp, dan, mjesec, godina);
    while (head->next != NULL && DateCompare(head->next, newTemp) < 0) {
        head = head->next;
    }

    newTemp->next = head->next;
    head->next = newTemp;
    return EXIT_SUCCESS;
}

TemperatureP CreateT(int dubina, int temp, int dan, int mjesec, int godina) {
    TemperatureP newTemp = (TemperatureP)malloc(sizeof(Temperature));
    if (!newTemp) {
        printf("Error in memory allocation!\n");
        return NULL;
    }

    newTemp->depth = dubina;
    newTemp->next = NULL;
    newTemp->temperature = temp;

    newTemp->dateOfMeasurments = (DateP)malloc(sizeof(Date));
    if (!(newTemp->dateOfMeasurments)) {
        printf("Error in memory allocation!\n");
        free(newTemp);
        return NULL;
    }

    newTemp->dateOfMeasurments->day = dan;
    newTemp->dateOfMeasurments->month = mjesec;
    newTemp->dateOfMeasurments->year = godina;
    return newTemp;
}

int DateCompare(TemperatureP head, TemperatureP head2) {
    int rez = head->dateOfMeasurments->year - head2->dateOfMeasurments->year;
    if (rez == 0) {
        rez = head->dateOfMeasurments->month - head2->dateOfMeasurments->month;
        if (rez == 0) {
            rez = head->dateOfMeasurments->day - head2->dateOfMeasurments->day;
        }
    }
    return rez;
}

int PrintList(TemperatureP head) {
    if (head == NULL) {
        printf("List is empty!\n");
        return EXIT_SUCCESS;
    }

    while (head != NULL) {
        printf("%d.%d.%d. Depth: %d, Temperature: %d\n",
               head->dateOfMeasurments->day, head->dateOfMeasurments->month,
               head->dateOfMeasurments->year, head->depth, head->temperature);
        head = head->next;
    }
    return EXIT_SUCCESS;
}

float ProsjecnaTEmperatura(TemperatureP head) {
    int count = 0;
    float sum = 0.0;
    float prosj_temp = 0.0;

    while (head != NULL) {
        if (head->dateOfMeasurment->year == 2023 && head->depth > 50) {
            sum += head->temperature;
            count++;
        }
        head = head->next;
    }

    if (count > 0) {
        prosj_temp = sum / count;
    } else {
        prosj_temp = 0.0;
    }

    return prosj_temp;
}


int DeleteTemp(TemperatureP head, float prosjecna) {
    TemperatureP temp = NULL;
    TemperatureP current = head->next;
    TemperatureP prev = head;

    while (current != NULL) {
        if (current->temperature < prosjecna) {
            temp = current;
            prev->next = current->next;
            free(temp->dateOfMeasurments);
            free(temp);
            current = prev->next;
        } else {
            prev = current;
            current = current->next;
        }
    }
    return EXIT_SUCCESS;
}

void FreeList(TemperatureP head) {
    TemperatureP current = head;
    TemperatureP next;

    while (current != NULL) {
        next = current->next;
        free(current->dateOfMeasurments);
        free(current);
        current = next;
    }
}
