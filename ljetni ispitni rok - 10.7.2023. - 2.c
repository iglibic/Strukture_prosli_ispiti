#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define _CRT_SECURE_NO_WARNINGS
#define MAX_NAME_LEN 32
#define MAX_DATE_LEN 11
#define MIN_SALARY 1000
#define MAX_SALARY 1050

typedef struct _employee* EmployeeP;
typedef struct _employee {
    char firstName[MAX_NAME_LEN];
    char lastName[MAX_NAME_LEN];
    char date[MAX_DATE_LEN];  // Datum kao string "YYYY-MM-DD"
    int salary;
    EmployeeP next;
} Employee;

EmployeeP createEmployee(char* firstName, char* lastName, char* date, int salary);
EmployeeP insertSorted(EmployeeP head, EmployeeP newEmployee);
void printList(EmployeeP head);
EmployeeP deleteIvan(EmployeeP head);
void freeList(EmployeeP head);
int generateUniqueSalary(int usedSalaries[], int count);

int main() {
    EmployeeP head = NULL;
    FILE* file = fopen("zaposlenici.txt", "r");

    if (!file) {
        printf("Ne mogu otvoriti datoteku\n");
        return 1;
    }

    srand((unsigned)time(NULL));
    int usedSalaries[MAX_SALARY - MIN_SALARY + 1] = {0};
    int salaryCount = 0;

    char firstName[MAX_NAME_LEN], lastName[MAX_NAME_LEN], date[MAX_DATE_LEN];

    while (fscanf(file, "%s %s %s", firstName, lastName, date) == 3) {
        int salary = generateUniqueSalary(usedSalaries, salaryCount++);
        EmployeeP newEmployee = createEmployee(firstName, lastName, date, salary);
        if (!newEmployee) continue;
        head = insertSorted(head, newEmployee);
    }
    fclose(file);

    printf("Sortirana lista zaposlenika:\n");
    printList(head);

    head = deleteIvan(head);
    printf("\nLista nakon brisanja Ivana:\n");
    printList(head);

    freeList(head);
    return 0;
}

EmployeeP createEmployee(char* firstName, char* lastName, char* date, int salary) {
    EmployeeP newEmployee = (EmployeeP)malloc(sizeof(Employee));
    if (!newEmployee) {
        printf("Neuspjela alokacija memorije\n");
        return NULL;
    }
    strcpy(newEmployee->firstName, firstName);
    strcpy(newEmployee->lastName, lastName);
    strcpy(newEmployee->date, date);
    newEmployee->salary = salary;
    newEmployee->next = NULL;
    return newEmployee;
}

EmployeeP insertSorted(EmployeeP head, EmployeeP newEmployee) {
    EmployeeP current = head, previous = NULL;

    while (current && strcmp(current->date, newEmployee->date) < 0) {
        previous = current;
        current = current->next;
    }

    if (!previous) {
        newEmployee->next = head;
        return newEmployee;
    }

    previous->next = newEmployee;
    newEmployee->next = current;
    return head;
}

void printList(EmployeeP head) {
    while (head) {
        printf("%s %s - %s - %d€\n", head->firstName, head->lastName, head->date, head->salary);
        head = head->next;
    }
}

EmployeeP deleteIvan(EmployeeP head) {
    EmployeeP current = head, previous = NULL;

    while (current) {
        if (strcmp(current->firstName, "Ivan") == 0) {
            if (!previous) {
                head = current->next;
                free(current);
                current = head;
            } else {
                previous->next = current->next;
                free(current);
                current = previous->next;
            }
        } else {
            previous = current;
            current = current->next;
        }
    }
    return head;
}

void freeList(EmployeeP head) {
    while (head) {
        EmployeeP next = head->next;
        free(head);
        head = next;
    }
}

int generateUniqueSalary(int usedSalaries[], int count) {
    int salary;
    do {
        salary = MIN_SALARY + rand() % (MAX_SALARY - MIN_SALARY + 1);
    } while (usedSalaries[salary - MIN_SALARY]);

    usedSalaries[salary - MIN_SALARY] = 1;
    return salary;
}
