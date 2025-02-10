#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 64

struct _item;
typedef struct _item* ItemP;
typedef struct _item {
    char name[MAX_NAME];
    int quantity;
    ItemP next;
} Item;

void insert_sorted(ItemP head, char* name);
void print_list(ItemP head);
void free_list(ItemP head);

int main() {
    FILE* file = fopen("inventura.txt", "r");
    if (!file) {
        printf("Neuspjelo otvaranje datoteke.\n");
        return -1;
    }

    Item head = { "", 0, NULL };
    char name[MAX_NAME];
    double price;

    while (fscanf(file, "%s %lf", name, &price) == 2) {
        insert_sorted(&head, name);
    }

    fclose(file);
    print_list(head.next);
    free_list(head.next);

    return 0;
}

void insert_sorted(ItemP head, char* name) {
    ItemP curr = head;

    while (curr->next != NULL) {
        if (strcmp(curr->next->name, name) == 0) {
            curr->next->quantity++;
            return;
        }
        if (strcmp(curr->next->name, name) > 0) break;
        curr = curr->next;
    }

    ItemP newItem = malloc(sizeof(Item));
    if (!newItem) return;
    strcpy(newItem->name, name);
    newItem->quantity = 1;
    newItem->next = curr->next;
    curr->next = newItem;
}

void print_list(ItemP head) {
    while (head != NULL) {
        printf("%s %d\n", head->name, head->quantity);
        head = head->next;
    }
}

void free_list(ItemP head) {
    ItemP temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
