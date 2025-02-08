#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME (64)

struct _item;
typedef struct _item* ItemP;

typedef struct _item {
    char name[MAX_NAME];
    int quantity;
    ItemP next;
} Item;

// Funkcija za usporedbu naziva opreme
int compareNames(const char *name1, const char *name2) {
    return strcmp(name1, name2);
}

// Funkcija za umetanje u vezanu listu prema nazivu opreme
ItemP insertSorted(ItemP head, Item* newItem) {
    ItemP current = head, prev = NULL;

    // Ako je lista prazna ili novo ime treba biti na početku
    if (head == NULL || compareNames(newItem->name, head->name) < 0) {
        newItem->next = head;
        return newItem;
    }

    // Pronađi odgovarajuće mjesto za umetanje
    while (current != NULL && compareNames(newItem->name, current->name) > 0) {
        prev = current;
        current = current->next;
    }

    // Ako postoji oprema s istim imenom, samo povećaj količinu
    if (current != NULL && compareNames(newItem->name, current->name) == 0) {
        current->quantity += newItem->quantity;
        free(newItem);  // Osiguraj da ne dupliramo podatke
        return head;
    }

    // Ako nije pronađen isti naziv, umetni novi čvor
    newItem->next = current;
    if (prev != NULL) {
        prev->next = newItem;
    }

    return head;
}

// Funkcija za ispisivanje liste
void printList(ItemP head) {
    ItemP current = head;
    while (current != NULL) {
        printf("%s %d\n", current->name, current->quantity);
        current = current->next;
    }
}

// Funkcija za oslobađanje memorije
void freeList(ItemP head) {
    ItemP temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    FILE *file = fopen("inventura.txt", "r");
    if (!file) {
        printf("Failed to open file.\n");
        return 1;
    }

    ItemP head = NULL;
    Item tempItem;
    
    while (fscanf(file, "%s %d", tempItem.name, &tempItem.quantity) == 2) {
        tempItem.next = NULL;
        head = insertSorted(head, &tempItem);
    }

    fclose(file);

    // Ispis učitane liste
    printf("Inventura:\n");
    printList(head);

    freeList(head);
    return 0;
}
