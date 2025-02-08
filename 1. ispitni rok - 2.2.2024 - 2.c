#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN (32)

typedef struct _date {
    int day;
    int month;
    int year;
} Date;

struct _person;
typedef struct _person* PersonP;

typedef struct _person {
    char firstName[MAX_NAME_LEN];
    char lastName[MAX_NAME_LEN];
    int idNumber;
    Date birthDate;
    PersonP Next;
} Person;

void freeList(PersonP head) {
    PersonP temp;
    while (head != NULL) {
        temp = head;
        head = head->Next;
        free(temp);
    }
}

void printList(PersonP head) {
    PersonP current = head;
    while (current != NULL) {
        printf("ID: %d | Name: %s %s | Birth Date: %02d/%02d/%04d\n",
               current->idNumber, current->firstName, current->lastName,
               current->birthDate.day, current->birthDate.month, current->birthDate.year);
        current = current->Next;
    }
}

int compareDates(Date d1, Date d2) {
    if (d1.year != d2.year) return d1.year - d2.year;
    if (d1.month != d2.month) return d1.month - d2.month;
    return d1.day - d2.day;
}

PersonP insertSorted(PersonP head, Person* newPerson) {
    PersonP current = head;
    if (head == NULL || compareDates(newPerson->birthDate, head->birthDate) < 0) {
        newPerson->Next = head;
        return newPerson;
    }

    while (current->Next != NULL && compareDates(newPerson->birthDate, current->Next->birthDate) > 0) {
        current = current->Next;
    }
    newPerson->Next = current->Next;
    current->Next = newPerson;
    return head;
}

PersonP deletePersonByID(PersonP head, int id) {
    PersonP current = head;
    PersonP prev = NULL;
    
    while (current != NULL && current->idNumber != id) {
        prev = current;
        current = current->Next;
    }
    
    if (current == NULL) return head;
    
    if (prev == NULL) {
        head = current->Next;
    } else {
        prev->Next = current->Next;
    }
    free(current);
    return head;
}

int generateUniqueID() {
    static int nextID = 100;
    if (nextID > 180) nextID = 100;  // reset ID if it exceeds 180
    return nextID++;
}

int main() {
    FILE *file = fopen("Osobe.txt", "r");
    if (!file) {
        printf("Failed to open file.\n");
        return 1;
    }

    PersonP head = NULL;
    Person tempPerson;

    while (fscanf(file, "%s %s %d/%d/%d", tempPerson.firstName, tempPerson.lastName, 
                  &tempPerson.birthDate.day, &tempPerson.birthDate.month, &tempPerson.birthDate.year) == 5) {
        tempPerson.idNumber = generateUniqueID();
        tempPerson.Next = NULL;
        head = insertSorted(head, &tempPerson);
    }

    fclose(file);

    printf("Original List:\n");
    printList(head);

    int idToDelete;
    printf("\nEnter ID to delete: ");
    scanf("%d", &idToDelete);

    head = deletePersonByID(head, idToDelete);

    printf("\nUpdated List:\n");
    printList(head);

    freeList(head);
    return 0;
}
