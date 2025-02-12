#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  

#define MAX_NAME_LEN (32)  
#define MAX_SUBJECTS (3)  

typedef struct _student {
    char firstName[MAX_NAME_LEN];
    char lastName[MAX_NAME_LEN];
    int grades[MAX_SUBJECTS]; // Ocene iz različitih predmeta  
    struct _student* Next;
} Student;

typedef Student* StudentP;

StudentP createStudent(char* firstName, char* lastName, int grades[]);
void insertStudent(StudentP head, StudentP newStudent);
void loadStudents(const char* filename, StudentP head, int subjectIndex);
StudentP combineLists(StudentP lists[], int numberOfSubjects);
void printList(StudentP head);
void freeList(StudentP head);

int main() {
    StudentP heads[MAX_SUBJECTS]; // Liste za svaku temu  
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        heads[i] = (StudentP)malloc(sizeof(Student));
        heads[i]->Next = NULL; // Prvi čvor kao "dummy" čvor  
    }

    loadStudents("Subject1.txt", heads[0], 0);
    loadStudents("Subject2.txt", heads[1], 1);
    loadStudents("Subject3.txt", heads[2], 2);

    StudentP combinedList = combineLists(heads, MAX_SUBJECTS);
    printf("Finalna lista studenata:\n");
    printList(combinedList);

    for (int i = 0; i < MAX_SUBJECTS; i++) {
        freeList(heads[i]);
    }
    freeList(combinedList);

    return 0;
}

StudentP createStudent(char* firstName, char* lastName, int grades[]) {
    StudentP newStudent = (StudentP)malloc(sizeof(Student));
    if (newStudent == NULL) {
        printf("ERROR! Could not allocate the memory for a student!");
        return NULL;
    }

    strcpy(newStudent->firstName, firstName);
    strcpy(newStudent->lastName, lastName);
    memcpy(newStudent->grades, grades, sizeof(int) * MAX_SUBJECTS);
    newStudent->Next = NULL;
    return newStudent;
}

void insertStudent(StudentP head, StudentP newStudent) {
    newStudent->Next = head->Next; // Umetni kao prvi čvor nakon dummy  
    head->Next = newStudent;
}

void loadStudents(const char* filename, StudentP head, int subjectIndex) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Ne mogu otvoriti datoteku");
        return;
    }

    char firstName[MAX_NAME_LEN];
    char lastName[MAX_NAME_LEN];
    int grades[MAX_SUBJECTS] = { 1, 1, 1 }; // Podrazumevane ocene su 1  

    while (fscanf(file, "%s %s %d", firstName, lastName, &grades[subjectIndex]) == 3) {
        StudentP newStudent = createStudent(firstName, lastName, grades);
        insertStudent(head, newStudent);
    }

    fclose(file);
}

StudentP combineLists(StudentP lists[], int numberOfSubjects) {
    StudentP combinedHead = (StudentP)malloc(sizeof(Student));
    combinedHead->Next = NULL; // dummy head  

    // Koristimo hash mapu da pratimo koji studenti su već dodati  
    for (int i = 0; i < numberOfSubjects; i++) {
        StudentP current = lists[i]->Next;
        while (current != NULL) {
            // Proveravamo da li je student već u kombinovanoj listi  
            StudentP exist = combinedHead->Next;
            int found = 0;
            while (exist != NULL) {
                if (strcmp(exist->firstName, current->firstName) == 0 &&
                    strcmp(exist->lastName, current->lastName) == 0) {
                    // Ako postoji, samo ažuriramo ocenu iz trenutnog predmeta  
                    exist->grades[i] = current->grades[i];
                    found = 1;
                    break;
                }
                exist = exist->Next;
            }

            // Ako student nije pronađen, umetnemo novog  
            if (!found) {
                StudentP newStudent = createStudent(current->firstName, current->lastName, current->grades);
                insertStudent(combinedHead, newStudent);
            }

            current = current->Next;
        }
    }

    return combinedHead; // Vraća kombinovanu listu  
}

void printList(StudentP head) {
    if (head == NULL || head->Next == NULL) {
        printf("Lista je prazna.\n");
        return;
    }

    StudentP current = head->Next; // Preskoči dummy  
    while (current != NULL) {
        printf("%s %s (", current->lastName, current->firstName);
        for (int i = 0; i < MAX_SUBJECTS; i++) {
            printf("%d", current->grades[i]);
            if (i < MAX_SUBJECTS - 1) {
                printf(", "); // Dodaj zarez između ocena  
            }
        }
        printf(")\n");
        current = current->Next;
    }
}

void freeList(StudentP head) {
    StudentP current = head;
    while (current != NULL) {
        StudentP next = current->Next;
        free(current);
        current = next;
    }
}
