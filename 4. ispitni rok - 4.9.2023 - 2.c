#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LEN 20
#define TOP_WORDS_COUNT 5

typedef struct _rijec* RijecP;
typedef struct _rijec {
    char word[MAX_WORD_LEN];
    int count;
    RijecP next;
} Rijec;

RijecP createWord(char* word);
RijecP insertWord(RijecP head, char* word);
void printList(RijecP head);
RijecP moveTopWords(RijecP head);
void freeList(RijecP head);

int main() {
    RijecP head = NULL;
    FILE* file = fopen("rijeci.txt", "r");

    if (!file) {
        printf("Ne mogu otvoriti datoteku\n");
        return 1;
    }

    char word[MAX_WORD_LEN];
    while (fscanf(file, "%s", word) == 1) {
        head = insertWord(head, word);
    }
    fclose(file);

    printf("Vezana lista rijeci:\n");
    printList(head);

    head = moveTopWords(head);
    printf("\nLista nakon pomicanja najcescih rijeci:\n");
    printList(head);

    freeList(head);
    return 0;
}

RijecP createWord(char* word) {
    RijecP newWord = (RijecP)malloc(sizeof(Rijec));
    if (!newWord) {
        printf("Neuspjela alokacija memorije\n");
        return NULL;
    }
    strcpy(newWord->word, word);
    newWord->count = 1;
    newWord->next = NULL;
    return newWord;
}

RijecP insertWord(RijecP head, char* word) {
    RijecP current = head, previous = NULL;

    while (current) {
        if (strcmp(current->word, word) == 0) {
            current->count++;
            return head;
        }
        previous = current;
        current = current->next;
    }

    RijecP newWord = createWord(word);
    if (!newWord) return head;

    if (!previous) return newWord;
    previous->next = newWord;
    return head;
}

void printList(RijecP head) {
    while (head) {
        printf("%s (%d)\n", head->word, head->count);
        head = head->next;
    }
}

RijecP moveTopWords(RijecP head) {
    if (!head) return NULL;

    RijecP sorted = NULL, sortedTail = NULL, rest = head;

    for (int i = 0; i < TOP_WORDS_COUNT; i++) {
        RijecP maxNode = NULL, prevMax = NULL, current = rest, prev = NULL;

        while (current) {
            if (!maxNode || current->count > maxNode->count) {
                maxNode = current;
                prevMax = prev;
            }
            prev = current;
            current = current->next;
        }

        if (!maxNode) break; // Ako nema više elemenata, prekini  

        if (prevMax) prevMax->next = maxNode->next;
        else rest = maxNode->next; // Ako je maxNode bio prvi, pomakni početak  

        maxNode->next = NULL;

        if (!sorted) {
            sorted = maxNode;
            sortedTail = maxNode;
        }
        else {
            sortedTail->next = maxNode;
            sortedTail = maxNode;
        }
    }

    if (sortedTail) sortedTail->next = rest;
    return sorted ? sorted : head;
}

void freeList(RijecP head) {
    while (head) {
        RijecP next = head->next;
        free(head);
        head = next;
    }
}
