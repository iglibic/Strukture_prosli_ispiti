#define _CRT_SECURE_NO_WARNINGS  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  

#define MAX_NAME_LEN (32)  
#define MAX_DATE_LEN (11)  

typedef struct _person;
typedef struct _person* PersonP;
typedef struct _person {
    char firstName[MAX_NAME_LEN];
    char lastName[MAX_NAME_LEN];
    int idNumber;
    char birthDate[MAX_DATE_LEN];
    PersonP Next;
} Person;

PersonP createPerson(char* firstName, char* lastName, char* birthDate, int idNumber);
PersonP insertSorted(PersonP head, PersonP newPerson);
void loadData(const char* filename, PersonP head);
PersonP findPrevious(PersonP head, PersonP toDelete);  
int deletePerson(PersonP head, int idNumber);  
void printList(PersonP head);
void freeList(PersonP head);

int main() {

    PersonP head = (PersonP)malloc(sizeof(Person));
    if (head == NULL) {
        printf("ERROR! Could not allocate the memory for head!");
        return 1;
    }
    head->Next = NULL; 

    loadData("Osobe.txt", head);  

    printf("Lista osoba:\n");
    printList(head->Next);  

    int idToDelete;
    printf("Unesite ID osobe koju želite obrisati: ");
    scanf("%d", &idToDelete);
    head = deletePerson(head, idToDelete);

    printf("\nNova lista osoba:\n");
    printList(head->Next);  

    freeList(head->Next);
    free(head);  

    return 0;
}

PersonP createPerson(char* firstName, char* lastName, char* birthDate, int idNumber) {
    PersonP newPerson = (PersonP)malloc(sizeof(Person));
    if (newPerson == NULL) {
        printf("ERROR! Could not allocate the memory!");
        return NULL;
    }

    strcpy(newPerson->firstName, firstName);
    strcpy(newPerson->lastName, lastName);
    strcpy(newPerson->birthDate, birthDate);
    newPerson->idNumber = idNumber;
    newPerson->Next = NULL;
    return newPerson;
}

PersonP insertSorted(PersonP head, PersonP newPerson) {
    PersonP current = head;
    while (current->Next != NULL && strcmp(newPerson->birthDate, current->Next->birthDate) > 0) {
        current = current->Next;
    }

    newPerson->Next = current->Next;
    current->Next = newPerson;
    return head;
}

void loadData(const char* filename, PersonP head) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Ne mogu otvoriti datoteku");
        return;
    }

    char firstName[MAX_NAME_LEN];
    char lastName[MAX_NAME_LEN];
    char birthDate[MAX_DATE_LEN];
    int assignedID;

    while (fscanf(file, "%s %s %s", firstName, lastName, birthDate) == 3) {
        do {
            assignedID = (rand() % 81) + 100;
        } while ((head->Next) && (head->Next->idNumber == assignedID));

        PersonP newPerson = createPerson(firstName, lastName, birthDate, assignedID);
        insertSorted(head, newPerson);
    }

    fclose(file);
}

void printList(PersonP head) {
    while (head != NULL) {
        printf("ID: %d, Name: %s %s, Birth Date: %s\n", head->idNumber, head->firstName, head->lastName, head->birthDate);
        head = head->Next;
    }
}

PersonP findPrevious(PersonP head, PersonP toDelete) {  
    PersonP current = head->Next;  
    while (current != NULL && current->Next != toDelete) {  
        current = current->Next;  
    }  
    return current;  
}  

int deletePerson(PersonP head, int idNumber) {  
    if (head == NULL) {  
        printf("Lista je prazna.\n");  
        return -1;  
    }  

    PersonP current = head->Next;   
    while (current != NULL && current->idNumber != idNumber) {  
        current = current->Next;  
    }  

    if (current == NULL) {  
        printf("Osoba s ID-em %d nije pronađena.\n", idNumber);  
        return -1;  
    }  

    PersonP previous = findPrevious(head, current);  
    if (previous == NULL) {  
        head->Next = current->Next;  
    } else {  
        previous->Next = current->Next;  
    }  
    
    free(current);  
    return EXIT_SUCCESS;  
}  
void freeList(PersonP head) {
    PersonP current = head;
    while (current != NULL) {
        PersonP next = current->Next;
        free(current);
        current = next;
    }
}
