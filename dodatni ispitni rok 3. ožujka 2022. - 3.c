#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ID_LENGTH 64
#define MAX_NAME_LENGTH 64

struct _visit;
typedef struct _visit *VisitP;
typedef struct _visit {
    char userId[MAX_ID_LENGTH];
    int day;
    int month;
    int year;
    VisitP next;
} Visit;

struct _membership;
typedef struct _membership* MembershipP;
typedef struct _membership {
    char userId[MAX_ID_LENGTH];
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    int month;
    int year;
    int price;
    int visitCount;
    VisitP visits;
    MembershipP next;
} Membership;

MembershipP createMembership(char* userId, char* firstName, char* lastName, int month, int year, int price) {
    MembershipP newMembership = (MembershipP)malloc(sizeof(Membership));
    if (!newMembership) {
        printf("Neuspjesna alokacija memorije!\n");
        return NULL;
    }
    strcpy(newMembership->userId, userId);
    strcpy(newMembership->firstName, firstName);
    strcpy(newMembership->lastName, lastName);
    newMembership->month = month;
    newMembership->year = year;
    newMembership->price = price;
    newMembership->visitCount = 0;
    newMembership->visits = NULL;
    newMembership->next = NULL;
    return newMembership;
}

VisitP createVisit(char* userId, int day, int month, int year) {
    VisitP newVisit = (VisitP)malloc(sizeof(Visit));
    if (!newVisit) {
        printf("Neuspjesna alokacija memorije!\n");
        return NULL;
    }
    strcpy(newVisit->userId, userId);
    newVisit->day = day;
    newVisit->month = month;
    newVisit->year = year;
    newVisit->next = NULL;
    return newVisit;
}

void insertSortedMembership(MembershipP* head, MembershipP newMembership) {
    MembershipP current = *head, prev = NULL;
    while (current && (current->year < newMembership->year || 
          (current->year == newMembership->year && current->month < newMembership->month))) {
        prev = current;
        current = current->next;
    }
    if (prev) prev->next = newMembership;
    else *head = newMembership;
    newMembership->next = current;
}

void insertSortedVisit(VisitP* head, VisitP newVisit) {
    VisitP current = *head, prev = NULL;
    while (current && current->day < newVisit->day) {
        prev = current;
        current = current->next;
    }
    if (prev) prev->next = newVisit;
    else *head = newVisit;
    newVisit->next = current;
}

void loadMemberships(const char* filename, MembershipP* head) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Ne mogu otvoriti datoteku %s!\n", filename);
        return;
    }
    char userId[MAX_ID_LENGTH], firstName[MAX_NAME_LENGTH], lastName[MAX_NAME_LENGTH];
    int month, year, price;
    while (fscanf(file, "%s %s %s %d %d %d", userId, firstName, lastName, &month, &year, &price) == 6) {
        MembershipP newMembership = createMembership(userId, firstName, lastName, month, year, price);
        if (newMembership) insertSortedMembership(head, newMembership);
    }
    fclose(file);
}

void loadVisits(const char* filename, MembershipP head) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Ne mogu otvoriti datoteku %s!\n", filename);
        return;
    }
    char userId[MAX_ID_LENGTH];
    int day, month, year;
    while (fscanf(file, "%s %d %d %d", userId, &day, &month, &year) == 4) {
        VisitP newVisit = createVisit(userId, day, month, year);
        if (!newVisit) continue;
        MembershipP current = head;
        while (current) {
            if (strcmp(current->userId, userId) == 0 && current->month == month && current->year == year) {
                insertSortedVisit(&(current->visits), newVisit);
                current->visitCount++;
                break;
            }
            current = current->next;
        }
    }
    fclose(file);
}

void printMemberships(MembershipP head) {
    while (head) {
        printf("%s %s (%d/%d) - %d HRK\n", head->lastName, head->firstName, head->month, head->year, head->price);
        VisitP visit = head->visits;
        if (!visit) printf("    NEMA POSJETA\n");
        else while (visit) {
            printf("    Posjeta: %d/%d/%d\n", visit->day, visit->month, visit->year);
            visit = visit->next;
        }
        head = head->next;
    }
}

int calculateRevenue(MembershipP head, int month, int year) {
    int totalRevenue = 0;
    while (head) {
        if (head->year == year && head->month == month && head->visitCount > 0) {
            printf("%s %s - %d HRK\n", head->lastName, head->firstName, head->price);
            totalRevenue += head->price;
        }
        head = head->next;
    }
    return totalRevenue;
}

void freeVisits(VisitP head) { while (head) { VisitP temp = head; head = head->next; free(temp); } }
void freeMemberships(MembershipP head) { while (head) { freeVisits(head->visits); MembershipP temp = head; head = head->next; free(temp); } }

int main() {
    char membershipFile[50], visitsFile[50];
    printf("Unesite ime datoteke s clanarina: "); scanf("%s", membershipFile);
    printf("Unesite ime datoteke s posjetama: "); scanf("%s", visitsFile);
    MembershipP membershipList = NULL;
    loadMemberships(membershipFile, &membershipList);
    loadVisits(visitsFile, membershipList);
    printf("\nLista clanarina i posjeta:\n");
    printMemberships(membershipList);
    int month, year;
    printf("\nUnesite mjesec i godinu za ispis: ");
    scanf("%d %d", &month, &year);
    printf("\nClanarine s barem jednom posjetom:\n");
    int revenue = calculateRevenue(membershipList, month, year);
    printf("Ukupna zarada: %d HRK\n", revenue);
    freeMemberships(membershipList);
    return 0;
}
