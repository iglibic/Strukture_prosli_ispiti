#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME (64)

struct _item;
typedef struct _item* ItemP;

typedef struct _item {
    char name[MAX_NAME];
    double sellPrice;  // Cijena opreme
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

    // Ako postoji oprema s istim imenom, samo povećaj količinu i cijenu
    if (current != NULL && compareNames(newItem->name, current->name) == 0) {
        current->quantity += newItem->quantity;
        // Ako cijena nije jednaka, dodaj novu instancu opreme s različitom cijenom
        if (current->sellPrice != newItem->sellPrice) {
            Item* newNode = (Item*)malloc(sizeof(Item));
            strcpy(newNode->name, newItem->name);
            newNode->quantity = newItem->quantity;
            newNode->sellPrice = newItem->sellPrice;
            newNode->next = current->next;
            current->next = newNode;
        }
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
        printf("%s %d %.2f\n", current->name, current->quantity, current->sellPrice);
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

// Funkcija za brisanje članova čija ukupna vrijednost premašuje 50
ItemP deleteExpensiveItems(ItemP head) {
    ItemP current = head, prev = NULL;
    
    while (current != NULL) {
        double totalValue = current->quantity * current->sellPrice;
        if (totalValue > 50) {
            // Ako je čvor na početku
            if (prev == NULL) {
                head = current->next;
            } else {
                prev->next = current->next;
            }
            ItemP temp = current;
            current = current->next;
            free(temp);  // Oslobađamo memoriju
        } else {
            prev = current;
            current = current->next;
        }
    }
    
    return head;
}

int main() {
    FILE *file = fopen("inventura.txt", "r");
    if (!file) {
        printf("Failed to open file.\n");
        return 1;
    }

    ItemP head = NULL;
    Item tempItem;
    
    // Čitanje podataka iz datoteke
    while (fscanf(file, "%s %lf %d", tempItem.name, &tempItem.sellPrice, &tempItem.quantity) == 3) {
        tempItem.next = NULL;
        head = insertSorted(head, &tempItem);
    }

    fclose(file);

    // Ispis originalne liste
    printf("Original Inventory:\n");
    printList(head);

    // Brisanje opreme čija ukupna vrijednost premašuje 50
    head = deleteExpensiveItems(head);

    // Ispis nove liste nakon brisanja
    printf("\nUpdated Inventory (items with total value > 50 removed):\n");
    printList(head);

    freeList(head);
    return 0;
}
