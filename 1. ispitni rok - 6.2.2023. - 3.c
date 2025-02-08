#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 64
#define NUM_SUBJECTS 3

struct student;
typedef struct _student* StudentP;

typedef struct _student {
    char firstName[MAX_NAME_LEN];
    char lastName[MAX_NAME_LEN];
    int Subject[NUM_SUBJECTS];  // Ocjene za 3 predmeta
    StudentP Next;
} Student;

// Funkcija za izračun prosjeka ocjena
double calculateAverage(int subjectGrades[NUM_SUBJECTS]) {
    int sum = 0, count = 0;
    for (int i = 0; i < NUM_SUBJECTS; i++) {
        if (subjectGrades[i] != 0) {
            sum += subjectGrades[i];
            count++;
        }
    }
    return (count > 0) ? (double)sum / count : 0.0;
}

// Funkcija za umetanje studenta u listu, sortiranu po prosjeku i prezimenu
StudentP insertStudent(StudentP head, Student* newStudent) {
    StudentP current = head, prev = NULL;

    // Računanje prosjeka
    double avg = calculateAverage(newStudent->Subject);

    // Ako je lista prazna ili novo ime treba biti na početku
    if (head == NULL || calculateAverage(head->Subject) < avg ||
        (calculateAverage(head->Subject) == avg && strcmp(newStudent->lastName, head->lastName) < 0)) {
        newStudent->Next = head;
        return newStudent;
    }

    // Pronađi odgovarajuće mjesto za umetanje
    while (current != NULL && (calculateAverage(current->Subject) > avg ||
           (calculateAverage(current->Subject) == avg && strcmp(newStudent->lastName, current->lastName) < 0))) {
        prev = current;
        current = current->Next;
    }

    // Ako je student već prisutan u listi, grupiraj njegove ocjene
    if (current != NULL && strcmp(newStudent->lastName, current->lastName) == 0 && strcmp(newStudent->firstName, current->firstName) == 0) {
        for (int i = 0; i < NUM_SUBJECTS; i++) {
            if (current->Subject[i] == 0 && newStudent->Subject[i] != 0) {
                current->Subject[i] = newStudent->Subject[i];  // Dodaj ocjenu
            }
        }
        free(newStudent);  // Osiguraj da ne dupliramo podatke
        return head;
    }

    // Ako nije pronađen isti student, umetni novi čvor
    newStudent->Next = current;
    if (prev != NULL) {
        prev->Next = newStudent;
    }

    return head;
}

// Funkcija za ispisivanje liste
void printList(StudentP head) {
    StudentP current = head;
    while (current != NULL) {
        printf("- %s %s (", current->lastName, current->firstName);
        for (int i = 0; i < NUM_SUBJECTS; i++) {
            printf("%d", current->Subject[i]);
            if (i < NUM_SUBJECTS - 1) {
                printf(", ");
            }
        }
        printf(") - %.2f\n", calculateAverage(current->Subject));
        current = current->Next;
    }
}

// Funkcija za oslobađanje memorije
void freeList(StudentP head) {
    StudentP temp;
    while (head != NULL) {
        temp = head;
        head = head->Next;
        free(temp);
    }
}

// Funkcija za učitavanje podataka iz datoteke
StudentP loadDataFromFile(const char* fileName, StudentP head, int subjectIndex) {
    FILE *file = fopen(fileName, "r");
    if (!file) {
        printf("Failed to open file %s.\n", fileName);
        return head;
    }

    Student tempStudent;
    while (fscanf(file, "%s %s %d", tempStudent.firstName, tempStudent.lastName, &tempStudent.Subject[subjectIndex]) == 3) {
        tempStudent.Subject[0] = tempStudent.Subject[1] = tempStudent.Subject[2] = 0;  // Inicijaliziraj sve ocjene na 0
        tempStudent.Next = NULL;
        head = insertStudent(head, &tempStudent);
    }

    fclose(file);
    return head;
}

int main() {
    StudentP head = NULL;

    // Učitaj podatke iz svih triju datoteka
    head = loadDataFromFile("Subject1.txt", head, 0);
    head = loadDataFromFile("Subject2.txt", head, 1);
    head = loadDataFromFile("Subject3.txt", head, 2);

    // Ispis svih studenata
    printf("Sorted Student List:\n");
    printList(head);

    freeList(head);
    return 0;
}
