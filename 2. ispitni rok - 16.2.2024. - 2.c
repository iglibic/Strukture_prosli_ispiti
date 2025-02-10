#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct mjerenja* cvor;

typedef struct mjerenja {
    char datum[20];
    float dubina;
    float temperatura;
    cvor next;
} mjerenja;

void dodaj(cvor head, char* datum, float dubina, float temp);
void ispis(cvor head);
double prosjecna_temperatura(cvor head);
void ukloni_manje(cvor head, double temp);

int main() {
    cvor head = malloc(sizeof(mjerenja));
    if (!head) return -1;

    head->next = NULL;

    FILE* file = fopen("temperature.txt", "r");
    if (!file) return -1;

    char datum[20];
    float dubina, temperatura;

    while (fscanf(file, "%s %f %f", datum, &dubina, &temperatura) == 3) {
        dodaj(head, datum, dubina, temperatura);
    }

    // Ispis svih mjerenja
    ispis(head);

    // Izračunavanje prosječne temperature
    double pros = prosjecna_temperatura(head);
    printf("Prosejcna temperatura u 2023. je : %lf\n", pros);

    // Brisanje mjerenja s temperaturom manjom od prosječne
    ukloni_manje(head, pros);
    printf("\nLista nakon brisanja onih koji imaju manju temperaturu:\n");
    ispis(head);

    fclose(file);
    return 0;
}

void dodaj(cvor head, char* datum, float dubina, float temp) {
    cvor novi = malloc(sizeof(mjerenja));
    if (!novi) return;

    novi->dubina = dubina;
    novi->temperatura = temp;
    novi->next = NULL;
    strcpy(novi->datum, datum);

    cvor curr = head;
    while (curr->next != NULL && strcmp(curr->next->datum, datum) > 0) {
        curr = curr->next;
    }

    novi->next = curr->next;
    curr->next = novi;
}

void ispis(cvor head) {
    cvor curr = head->next;
    int i = 1;

    while (curr != NULL) {
        printf("%d. mjerenje:\n", i);
        printf("Datum: %s\n", curr->datum);
        printf("Dubina: %.2lf m\n", curr->dubina);
        printf("Temperatura: %.2lf°C\n\n", curr->temperatura);
        i++;
        curr = curr->next;
    }
}

double prosjecna_temperatura(cvor head) {
    cvor curr = head->next;
    double temp_sum = 0;
    int br = 0;

    while (curr != NULL) {
        // Provjera za 2023. godinu i dubinu veću od 50
        if (strncmp(curr->datum, "2023", 4) == 0 && curr->dubina > 50) {
            temp_sum += curr->temperatura;
            br++;
        }
        curr = curr->next;
    }

    return (br > 0) ? temp_sum / br : 0.0;  // Provjera kako bi se izbjegao dijeljeni s nulom
}

void ukloni_manje(cvor head, double temp) {
    cvor curr = head->next;
    cvor prev = head;

    while (curr != NULL) {
        if (curr->temperatura < temp) {
            prev->next = curr->next;
            free(curr);
            curr = prev->next;  // Pomicanje na sljedeći element
        }
        else {
            prev = curr;
            curr = curr->next;
        }
    }
}
