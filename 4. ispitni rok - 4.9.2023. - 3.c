#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_LEN 50

struct _word;
typedef struct _word* WordP;
typedef struct _word {
    char word[MAX_WORD_LEN];
    int count;
    WordP next;
} Word;

WordP createWordNode(const char* word) {
    WordP newWord = (WordP)malloc(sizeof(Word));
    if (!newWord) {
        printf("Neuspješna alokacija memorije!\n");
        return NULL;
    }
    strcpy(newWord->word, word);
    newWord->count = 1;
    newWord->next = NULL;
    return newWord;
}

void insertOrUpdateWord(WordP* head, const char* word) {
    WordP current = *head;
    WordP prev = NULL;

    while (current != NULL) {
        if (strcmp(current->word, word) == 0) {
            current->count++;
            return;
        }
        prev = current;
        current = current->next;
    }

    WordP newWord = createWordNode(word);
    if (!newWord) return;

    if (prev) {
        prev->next = newWord;
    } else {
        *head = newWord;
    }
}

void loadWords(const char* filename, WordP* head) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Ne mogu otvoriti datoteku %s!\n", filename);
        return;
    }

    char word[MAX_WORD_LEN];
    while (fscanf(file, "%s", word) == 1) {
        insertOrUpdateWord(head, word);
    }

    fclose(file);
}

void printWords(WordP head) {
    while (head) {
        printf("%s (%d)\n", head->word, head->count);
        head = head->next;
    }
}

void moveTopFiveToFront(WordP* head) {
    WordP sorted = NULL;

    for (int i = 0; i < 5; i++) {
        WordP maxNode = NULL;
        WordP maxPrev = NULL;
        WordP current = *head;
        WordP prev = NULL;

        while (current != NULL) {
            if (!maxNode || current->count > maxNode->count) {
                maxNode = current;
                maxPrev = prev;
            }
            prev = current;
            current = current->next;
        }

        if (!maxNode) return;

        if (maxPrev) {
            maxPrev->next = maxNode->next;
        } else {
            *head = maxNode->next;
        }

        maxNode->next = sorted;
        sorted = maxNode;
    }

    WordP tail = sorted;
    while (tail && tail->next) {
        tail = tail->next;
    }
    if (tail) {
        tail->next = *head;
        *head = sorted;
    }
}

int getRandomDeletionThreshold(WordP head) {
    int minCount = head->count, maxCount = head->count;
    WordP current = head;

    while (current) {
        if (current->count < minCount)
            minCount = current->count;
        if (current->count > maxCount)
            maxCount = current->count;
        current = current->next;
    }

    if (minCount == maxCount)
        return minCount;

    return (rand() % (maxCount - minCount + 1)) + minCount;
}

void deleteWordsByCount(WordP* head, int threshold) {
    WordP current = *head;
    WordP prev = NULL;

    while (current) {
        if (current->count == threshold) {
            if (prev) {
                prev->next = current->next;
                free(current);
                current = prev->next;
            } else {
                *head = current->next;
                free(current);
                current = *head;
            }
        } else {
            prev = current;
            current = current->next;
        }
    }
}

void freeList(WordP head) {
    while (head) {
        WordP temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    srand(time(NULL));
    WordP wordList = NULL;
    
    loadWords("rijeci.txt", &wordList);
    
    printf("Lista prije pomicanja najčešćih riječi:\n");
    printWords(wordList);
    
    moveTopFiveToFront(&wordList);
    
    printf("\nLista nakon pomicanja najčešćih riječi:\n");
    printWords(wordList);
    
    int threshold = getRandomDeletionThreshold(wordList);
    printf("\nBrisanje svih riječi koje se ponavljaju %d puta...\n", threshold);
    
    deleteWordsByCount(&wordList, threshold);
    
    printf("\nLista nakon brisanja riječi s frekvencijom %d:\n", threshold);
    printWords(wordList);
    
    freeList(wordList);
    return 0;
}
