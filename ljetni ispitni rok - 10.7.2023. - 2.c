
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

EmployeeP createEmployee(char* firstName, char* lastName, int year, int month, int day, int salary);
void insertSorted(EmployeeP* head, EmployeeP newEmployee);
void printList(EmployeeP head);
void deleteByName(EmployeeP* head, char* name);
int generateUniqueSalary(int* usedSalaries, int count);
void loadEmployeesFromFile(EmployeeP* head, char* filename);
void freeList(EmployeeP head);

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

int generateUniqueSalary(int* usedSalaries, int count) {
    int salary;
    do {
        salary = MIN_SALARY + rand() % (MAX_SALARY - MIN_SALARY + 1);
        int i;
        for (i = 0; i < count; i++) {
            if (usedSalaries[i] == salary)
                break;
        }
        if (i == count) return salary;
    } while (1);
}

void loadEmployeesFromFile(EmployeeP* head, char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    
    char firstName[MAX_NAME_LEN], lastName[MAX_NAME_LEN];
    int year, month, day;
    int usedSalaries[MAX_SALARY - MIN_SALARY + 1] = {0};
    int count = 0;
    
    srand(time(NULL));
    
    while (fscanf(file, "%s %s %d %d %d", firstName, lastName, &year, &month, &day) == 5) {
        int salary = generateUniqueSalary(usedSalaries, count);
        usedSalaries[count++] = salary;
        insertSorted(head, createEmployee(firstName, lastName, year, month, day, salary));
    }
    fclose(file);
}

void freeList(EmployeeP head) {
    while (head) {
        EmployeeP temp = head;
        head = head->next;
        free(temp);
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
    
    freeList(employees);
    return 0;
}
