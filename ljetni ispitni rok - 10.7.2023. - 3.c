#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LEN 32
#define MIN_SALARY 1000
#define MAX_SALARY 1050

typedef struct _date {
    int year;
    int month;
    int day;
} Date, *DateP;

typedef struct _employee {
    char firstName[MAX_NAME_LEN];
    char lastName[MAX_NAME_LEN];
    Date date;
    int salary;
    struct _employee* next;
} Employee, *EmployeeP;

EmployeeP createEmployee(char* firstName, char* lastName, int year, int month, int day, int salary) {
    EmployeeP newEmployee = (EmployeeP)malloc(sizeof(Employee));
    if (!newEmployee) {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newEmployee->firstName, firstName);
    strcpy(newEmployee->lastName, lastName);
    newEmployee->date.year = year;
    newEmployee->date.month = month;
    newEmployee->date.day = day;
    newEmployee->salary = salary;
    newEmployee->next = NULL;
    return newEmployee;
}

void insertSorted(EmployeeP* head, EmployeeP newEmployee) {
    EmployeeP current = *head, prev = NULL;
    while (current && (current->date.year < newEmployee->date.year ||
                       (current->date.year == newEmployee->date.year && current->date.month < newEmployee->date.month) ||
                       (current->date.year == newEmployee->date.year && current->date.month == newEmployee->date.month &&
                        current->date.day < newEmployee->date.day) ||
                       (current->date.year == newEmployee->date.year && current->date.month == newEmployee->date.month &&
                        current->date.day == newEmployee->date.day && strcmp(current->firstName, newEmployee->firstName) < 0))) {
        prev = current;
        current = current->next;
    }
    newEmployee->next = current;
    if (prev)
        prev->next = newEmployee;
    else
        *head = newEmployee;
}

void printList(EmployeeP head) {
    while (head) {
        printf("%s %s - %d/%d/%d - %d€\n", head->firstName, head->lastName, head->date.day, head->date.month, head->date.year, head->salary);
        head = head->next;
    }
    printf("\n");
}

void deleteByName(EmployeeP* head, char* name) {
    EmployeeP current = *head, prev = NULL;
    while (current) {
        if (strcmp(current->firstName, name) == 0) {
            if (prev)
                prev->next = current->next;
            else
                *head = current->next;
            free(current);
            current = (prev) ? prev->next : *head;
        } else {
            prev = current;
            current = current->next;
        }
    }
}

void moveTopThreeSalaries(EmployeeP* head) {
    EmployeeP top[3] = {NULL, NULL, NULL};
    EmployeeP prevTop[3] = {NULL, NULL, NULL};
    EmployeeP prev = NULL, current = *head;
    
    while (current) {
        for (int i = 0; i < 3; i++) {
            if (!top[i] || current->salary > top[i]->salary) {
                for (int j = 2; j > i; j--) {
                    top[j] = top[j-1];
                    prevTop[j] = prevTop[j-1];
                }
                top[i] = current;
                prevTop[i] = prev;
                break;
            }
        }
        prev = current;
        current = current->next;
    }
    
    for (int i = 2; i >= 0; i--) {
        if (top[i]) {
            if (prevTop[i])
                prevTop[i]->next = top[i]->next;
            else
                *head = top[i]->next;
            top[i]->next = *head;
            *head = top[i];
        }
    }
}

int main() {
    EmployeeP employees = NULL;
    loadEmployeesFromFile(&employees, "zaposlenici.txt");
    
    printf("Original list:\n");
    printList(employees);
    
    deleteByName(&employees, "Ivan");
    
    printf("List after removing 'Ivan':\n");
    printList(employees);
    
    moveTopThreeSalaries(&employees);
    
    printf("List after moving top three salaries:\n");
    printList(employees);
    
    freeList(employees);
    return 0;
}
