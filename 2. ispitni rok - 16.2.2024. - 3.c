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
    DateP dateOfMeasurment;
    int id;  // Dodan ID
    TemperatureP next;
} Temperature;

int ReadFile(TemperatureP head, char* filename);
int MakeList(TemperatureP head, int dubina, int temp, int dan, int mjesec, int godina);
TemperatureP CreateT(int dubina, int temp, int dan, int mjesec, int godina, int id);
int PrintList(TemperatureP head);
int DateCompare(TemperatureP head, TemperatureP head2);
int DeleteTemp(TemperatureP head, float prosjecna);
float ProsjecnaTEmperatura(TemperatureP head);
TemperatureP FindByID(TemperatureP head, int id);
int MoveToStart(TemperatureP head, int id);

int main() {
    Temperature head = {0, 0, NULL, 0, NULL};
    float prosjecnaTemp;

    ReadFile(&head, "temperature.txt");
    PrintList(head.next);

    prosjecnaTemp = ProsjecnaTEmperatura(head.next);
    printf("\nProsjecna temperatura u 2023.godini i na dubini >50 m je %.2f\n", prosjecnaTemp);

    DeleteTemp(&head, prosjecnaTemp);
    PrintList(head.next);

    // Ocjena 3: Unos ID-a i premještanje na početak
    int idToFind;
    printf("\nUnesite ID za premjestanje na pocetak: ");
    scanf("%d", &idToFind);
    MoveToStart(&head, idToFind);
    PrintList(head.next);

    // Oslobađanje memorije (oslobodite dinamicki alociranu memoriju prema potrebi)
    return 0;
}

int ReadFile(TemperatureP head, char* filename) {
    FILE* fptr = fopen(filename, "r");
    if (!fptr) {
        printf("File couldn't open\n");
        return EXIT_FAILURE;
    }

    int dan, mjesec, godina, dubina, temp, id = 500; // Inicijalizacija ID-a od 500
    while (fscanf(fptr, "%d.%d.%d %d %d", &dan, &mjesec, &godina, &dubina, &temp) != EOF) {
        MakeList(head, dubina, temp, dan, mjesec, godina, id++);
    }
    fclose(fptr);
    return EXIT_SUCCESS;
}

int MakeList(TemperatureP head, int dubina, int temp, int dan, int mjesec, int godina, int id) {
    TemperatureP newTemp = CreateT(dubina, temp, dan, mjesec, godina, id);
    while (head->next != NULL && DateCompare(head->next, newTemp) < 0) {
        head = head->next;
    }
    newTemp->next = head->next;
    head->next = newTemp;
    return EXIT_SUCCESS;
}

TemperatureP CreateT(int dubina, int temp, int dan, int mjesec, int godina, int id) {
    TemperatureP newTemp = (TemperatureP)malloc(sizeof(Temperature));
    if (!newTemp) {
        printf("Memory allocation error!\n");
        return NULL;
    }

    newTemp->depth = dubina;
    newTemp->temperature = temp;
    newTemp->dateOfMeasurment = (DateP)malloc(sizeof(Date));
    if (!newTemp->dateOfMeasurment) {
        printf("Memory allocation error!\n");
        free(newTemp);
        return NULL;
    }
    newTemp->dateOfMeasurment->day = dan;
    newTemp->dateOfMeasurment->month = mjesec;
    newTemp->dateOfMeasurment->year = godina;
    newTemp->id = id;
    newTemp->next = NULL;

    return newTemp;
}

int DateCompare(TemperatureP head, TemperatureP head2) {
    int rez = head->dateOfMeasurment->year - head2->dateOfMeasurment->year;
    if (rez == 0) {
        rez = head->dateOfMeasurment->month - head2->dateOfMeasurment->month;
        if (rez == 0) {
            rez = head->dateOfMeasurment->day - head2->dateOfMeasurment->day;
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
        printf("%d.%d.%d. Dubina: %d Temperatura: %d ID: %d\n", 
               head->dateOfMeasurment->day, head->dateOfMeasurment->month, head->dateOfMeasurment->year, 
               head->depth, head->temperature, head->id);
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
            free(temp->dateOfMeasurment);
            free(temp);
            current = prev->next;
        } else {
            prev = current;
            current = current->next;
        }
    }

    return EXIT_SUCCESS;
}

TemperatureP FindByID(TemperatureP head, int id) {
    TemperatureP current = head->next;
    while (current != NULL) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL; // Ako nije pronađen
}

int MoveToStart(TemperatureP head, int id) {
    TemperatureP temp = FindByID(head, id);
    if (temp == NULL) {
        printf("Measurement with ID %d not found.\n", id);
        return EXIT_FAILURE;
    }

    // Premještanje na početak
    TemperatureP current = head;
    while (current->next != temp) {
        current = current->next;
    }

    current->next = temp->next;
    temp->next = head->next;
    head->next = temp;

    return EXIT_SUCCESS;
}
