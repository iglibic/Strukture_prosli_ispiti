// Napisati program koji generira slučajne brojeve u opsegu od 130 - 160. 
// Ukoliko broj ne postoji u vezanoj listi dodaje se sortirano po vrijednosti,
// a ukoliko broj već postoji u vezanoj listi, samo se uveća broj ponavljanja tog broja.
// Ispisati listu (vrijednost i broj ponavljanja).
//
// Ocjena 2: Pronaći broj/brojeve koji su se ponovili najmanje puta i ispisati ih.
//
// Ocjena 3: Tražiti od korisnika da upiše jedan broj i sve brojeve koji su se
// ponovili toliko puta treba pomaknuti na početak liste. Ispisati novu listu.
//
// U izradi zadatka može se koristiti struktura: 

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILE_NOT_OPENED (-1)

struct number_;
typedef struct number_* NumberP;
typedef struct number_ {
	int value;
	int no_repeats;
	NumberP next;
} Number;

int generateNumber();
NumberP createNumber(int value, int nORepeats);
int addToList(NumberP head, int value);
int findLovest(NumberP first);
int printLovest(NumberP first);
int printList(NumberP first);
int freeList(NumberP head);

int chooseNumber(NumberP head,int number);

int main() {
	srand((unsigned int)time(NULL));

	NumberP head = (NumberP)malloc(sizeof(Number));
	if (head == NULL) {
		printf("ERROR! Could not allocate the memmory!");
		return EXIT_FAILURE;
	}
	head->next = NULL;

	for (int i = 0; i < 20; i++) {
		addToList(head, generateNumber());
	}

	printList(head->next);

	printf("Brojevi s najmanjim brojem ponavljanja: \n");

	printLovest(head->next);

	int broj;

	printf("Unesite broj ponavljanja (optimalno izmedju 1 i 4): ");
	scanf("%d", &broj);

	chooseNumber(head, broj);

	printf("Lista nakon pomicanja: ");
	printList(head->next);

	freeList(head);

	return EXIT_SUCCESS;
}

int generateNumber() {

	int broj = rand() % 31 + 130;

	return broj;
}

NumberP createNumber(int value, int nORepeats) {
	NumberP number = (NumberP)malloc(sizeof(Number));
	if (number == NULL) {
		printf("ERROR! Could not allocate memmory!");
		return NULL;
	}

	number->value = value;
	number->no_repeats = nORepeats;
	number->next = NULL;

	return number;
}

int addToList(NumberP head, int value) {
	NumberP temp = head;
	
	while (temp->next != NULL && temp->next->value < value) {
		temp = temp->next;
	}
	if (temp->next != NULL && temp->next->value == value) {
		temp->next->no_repeats++;
		return EXIT_SUCCESS;
	}

	NumberP newNumber = createNumber(value, 1);
	if (newNumber == NULL) {
		printf("ERROR! Could not create new element!");
		return EXIT_FAILURE;
	}

	newNumber->next = temp->next;
	temp->next = newNumber;

	return EXIT_SUCCESS;
}

int findLovest(NumberP first) {
	NumberP temp = first;
	int minBrojPonavljanja = temp->no_repeats;

	while (temp != NULL) {
		if (temp->no_repeats < minBrojPonavljanja) {
			minBrojPonavljanja = temp->no_repeats;
		}
		temp = temp->next;
	}

	return minBrojPonavljanja;
}

int printLovest(NumberP first) {
	NumberP temp = first;

	while (temp) {
		if (temp->no_repeats == findLovest(first)) {
			printf("Broj: %d\n", temp->value);
		}
		temp = temp->next;
	}

	return EXIT_SUCCESS;
}

int printList(NumberP first) {
	NumberP temp = first;

	while (temp) {
		printf("Broj: %d, Broj ponavljanja: %d\n", temp->value, temp->no_repeats);
		temp = temp->next;
	}

	return EXIT_SUCCESS;
}

int freeList(NumberP head) {
	NumberP temp = head->next;

	while (temp) {
		NumberP toFree = temp;
		temp = temp->next;
		free(toFree);
	}
	head->next = NULL;

	return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------------------*/

int chooseNumber(NumberP head, int number) {
	NumberP prev = head;
	NumberP temp = head->next;

	while (temp != NULL) {
		if (temp->no_repeats == number) {
			prev->next = temp->next;

			NumberP toMove = temp;

			toMove->next = head->next;
			head->next = toMove;

			temp = prev->next;
		}
		else {
			prev = temp;
			temp = temp->next;
		}
	}

	return EXIT_SUCCESS;
}
