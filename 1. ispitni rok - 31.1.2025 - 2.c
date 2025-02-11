#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 64
#define FILE_NOT_OPENED (-1)

struct _item;
typedef struct _item* ItemP;
typedef struct _item {
    char name[MAX_NAME];
    int quantity;
    ItemP next;
} Item;

int insertSorted(ItemP head, char* name);
int printList(ItemP head);
int freeList(ItemP head);

int main(){
    
    FILE* fp = fopen("inventura.txt", "r");
    if (fp == NULL) {
        printf("ERROR! Could not open the file!");
        return FILE_NOT_OPENED;
    }

    ItemP head = (ItemP)malloc(sizeof(Item));
    if (head == NULL) {
        printf("ERROR! Could not allocate memmory!");
        return EXIT_FAILURE;
    }
    head->next = NULL;

    char productName[MAX_NAME];
    int counter;

    while (fscanf(fp, "%s %d", productName, &counter) == 2) {
        insertSorted(head, productName);
    }

    printList(head);
    freeList(head);

    fclose(fp);

    return EXIT_SUCCESS;
}

int insertSorted(ItemP head, char* name) {
    ItemP curr = head;

    while (curr->next != NULL) {
        if (strcmp(curr->next->name, name) == 0) {
            curr->next->quantity++;
            return EXIT_SUCCESS;
        }
        if (strcmp(curr->next->name, name) > 0) break;
        curr = curr->next;
    }

    ItemP newPerson = (ItemP)malloc(sizeof(Item));
    if (newPerson == NULL) {
        printf("ERROR! Could not allocate the memmory!");
        return EXIT_FAILURE;
    }
    strcpy(newPerson->name, name);
    newPerson->quantity = 1;
    newPerson->next = curr->next;
    curr->next = newPerson;

    return EXIT_SUCCESS;
}

int printList(ItemP head) {
    ItemP temp = head->next;

    while (temp) {
        printf("%s %d\n", temp->name, temp->quantity);
        temp = temp->next;
    }

    return EXIT_SUCCESS;
}

int freeList(ItemP head) {
    ItemP temp = head->next;

    while (temp) {
        ItemP toFree = temp;
        temp = temp->next;
        free(toFree);
    }

    return EXIT_SUCCESS;
}
