#define _CRT_SECURE_NO_WARNINGS
#define MAX_ID_LEN (16)
#define MAX_ID (8)
#define MAX_NAME_LEN (64)
#define MAX_NAME_LENGTH (32)

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>


//podatke o clanovima knjiznice trebaju se spremiti u jednostruku vezanu listu sortiranu po prezimenu, pa po imenu pa po id-u
//podatke o posudenim knjigama trebaju se spremiti u jednostruko vezanu listu sortiranu po nazivu knjige
//ocjdena 2 pronaci clana knjiznice koji je pročitao najviše knjiga te ispisati njegovo ime i prezime
//ocjena 3 clana izbrisati iz vezane liste

struct _member;
typedef struct _member* MemberP;
typedef struct _member {
	char ID[MAX_ID_LEN];
	char firstname[MAX_NAME_LEN];
	char lastName[MAX_NAME_LEN];
	MemberP next;
}Member;

struct _book;
typedef struct _book* BookP;
typedef struct _book {
	char name[MAX_NAME_LEN];
	char memberID[MAX_ID_LEN];
	BookP next;
}Book;

int ReadFile(MemberP head, char* filename);
int MakeList(MemberP head, char* name, char* surname, char* ID);
MemberP CreateNewMember(char* name, char* surname, char* ID);
BookP CreateNewBook(char* name, char* memberId);
int ReadFile2(BookP head2, char* filename);
int MakeListOfBooks(BookP head2, char* nameBook, char* memId);
int SortName(MemberP head, MemberP new);
int Printmem(MemberP head);
int printBook(BookP head2);
BookP findID(BookP head, char* id);
MemberP FindTheMemberWithMostBooks(MemberP head, BookP head2);
int main() {

	Member head = {
		.ID = {0},
		.firstname = {0},
		.lastName = {0},
		.next = NULL
	};
	Book head2 = {
		.name = {0},
		.memberID = {0},
		.next = NULL,
	};
	char id[MAX_ID_LEN] = { 0 };
	printf("list of members:\n");
	ReadFile(&head, "clanovi.txt");
	Printmem(head.next);

	printf("\n");
	printf("List of books:\n");
	ReadFile2(&head2, "knjige.txt");
	printBook(head2.next);
	//FindTheMemberWithMostBooks(&head, &head2);

	DeleteMember(&head, &head2);
	Printmem(head.next);
	printBook(head2.next);
	/*DEleteMembers(&head);
	DEleteBooks(&head2);
	printBook(head2.next);
	Printmem(head.next);*/

	return 0;

}
int DEleteBooks(BookP head) {
	BookP temp = NULL;
	while (head->next != NULL) {
		temp = head->next;
		head->next = temp->next;
		free(temp);
	}
	return EXIT_SUCCESS;

}
int DEleteMembers(MemberP head1) {
	MemberP temp = NULL;
	while (head1->next != NULL) {
		temp = head1->next;
		head1->next = temp->next;
		free(temp);
	}
	return EXIT_SUCCESS;

}

MemberP  FindTheMemberWithMostBooks(MemberP head, BookP head2) {
	int maxbooks = 0;
	MemberP maxClan = NULL;

	MemberP current = head;
	while (current!=NULL) {
		int bookcount = 0;
		BookP currentB = head2;
		while (currentB!=NULL) {
			if (strcmp(currentB->memberID, current->ID) == 0) {
				bookcount++;
			}
			currentB = currentB->next;
		}
		if (bookcount > maxbooks) {
			maxbooks = bookcount;
			maxClan = current;
		}
		current = current->next;
	}


	printf("Clan koji je procitao najvise knjiga je %s %s (%s). Pročitao je %d knjiga.\n", maxClan->firstname, maxClan->lastName, maxClan->ID,maxbooks);

	return maxClan;
}
int ReadFile2(BookP head2, char* filename) {
	char name[MAX_NAME_LEN] = { 0 };
	char memberId[MAX_ID_LEN] = { 0 };
	FILE* fptr = NULL;

	fptr = fopen(filename, "r");
	if (!fptr) {
		printf("Error while opening file!\n");
		return EXIT_FAILURE;
	}
	else {
		printf("File opened!\n");
	}

	while (!feof(fptr)) {
		fscanf(fptr, "%s %s", memberId, name);

		MakeListOfBooks(head2, name, memberId);

	}
	fclose(fptr);
	return EXIT_SUCCESS;


}
int printBook(BookP head2) {
	if (head2 == NULL) {
		printf("list is empty!\n");
	}


	while (head2 != NULL) {
		printf("%s %s\n", head2->name, head2->memberID);
		head2 = head2->next;
	}
	return EXIT_SUCCESS;
}
int MakeListOfBooks(BookP head2, char* nameBook, char* memId) {
	BookP newb = NULL;
	newb = CreateNewBook(nameBook, memId);
	while (head2->next != NULL && strcmp(head2->next->name, newb->name) < 0) {
		head2 = head2->next;
	}
	newb->next = head2->next;
	head2->next = newb;

	return EXIT_SUCCESS;
}

BookP CreateNewBook(char* name, char* memberId) {
	BookP newBook = NULL;
	newBook = (BookP)malloc(sizeof(Book));

	if (!newBook) {
		printf("Allocation failed!\n");
	}

	strcpy(newBook->name, name);
	strcpy(newBook->memberID, memberId);
	newBook->next = NULL;

	return newBook;
}
int ReadFile(MemberP head, char* filename) {
	char ID[MAX_ID_LEN] = { 0 };
	char surname[MAX_NAME_LEN] = { 0 };
	char name[MAX_NAME_LEN] = { 0 };
	FILE* fptr = NULL;

	fptr = fopen(filename, "r");
	if (!fptr) {
		printf("Error while opening file!\n");
		return EXIT_FAILURE;
	}
	else {
		printf("File opened!\n");
	}

	while (!feof(fptr)) {
		fscanf(fptr, "%s %s %s", ID, surname, name);
		MakeList(head, name, surname, ID);


	}
	fclose(fptr);

	return EXIT_SUCCESS;

}
int MakeList(MemberP head, char* name, char* surname, char* ID) {
	MemberP new = NULL;

	new = CreateNewMember(name, surname, ID);
	while (head->next != NULL && SortName(head->next, new) < 0) {
		head = head->next;
	}
	new->next = head->next;
	head->next = new;

	return EXIT_SUCCESS;
}
int SortName(MemberP head, MemberP new) {
	if (strcmp(head->lastName, new->lastName) > 0) {
		return 1;
	}
	else if (strcmp(head->lastName, new->lastName) < 0) {
		return -1;
	}
	else if (strcmp(head->lastName, new->lastName) == 0) {
		if (strcmp(head->firstname, new->firstname) > 0) {
			return 1;
		}
		else if (strcmp(head->firstname, new->firstname) < 0) {
			return -1;
		}
		else if (strcmp(head->firstname, new->firstname) == 0) {
			if (strcmp(head->ID, new->ID) > 0) {
				return 1;
			}
			else if (strcmp(head->ID, new->ID) < 0) {
				return -1;
			}
		}

	}

}
MemberP CreateNewMember(char* name, char* surname, char* ID) {
	MemberP newMem = NULL;
	newMem = (MemberP)malloc(sizeof(Member));
	if (!newMem) {
		printf("Allocation failed!\n");
		return NULL;
	}
	strcpy(newMem->firstname, name);
	strcpy(newMem->lastName, surname);
	strcpy(newMem->ID, ID);
	newMem->next = NULL;

	return newMem;
}
int Printmem(MemberP head) {

	if (head == NULL) {
		printf("list is empty!\n");
	}


	while (head != NULL) {
		printf("%s %s %s\n", head->firstname, head->lastName, head->ID);
		head = head->next;
	}
	return EXIT_SUCCESS;
}

int DeleteMember(MemberP head, BookP head2){
	MemberP maxhead = NULL;
	maxhead=FindTheMemberWithMostBooks(head, head2);
	MemberP prevM = head;
	MemberP currentM = NULL;
	if (maxhead == NULL) {
		printf("No members found!\n");
	}
	//delete the books
	BookP prevB = head2;
	
	while (prevB->next != NULL) {
		BookP currentB = prevB->next;
		if (strcmp(currentB->memberID, maxhead->ID) == 0) {
			prevB->next = currentB->next;
			free(currentB);
		}
		else {
			prevB = currentB;
			
		}
	}

	while (prevM->next != NULL) {
		currentM = prevM->next;
		if (strcmp(currentM->ID, maxhead->ID) == 0) {
			
			prevM->next = currentM->next;
			free(currentM);
			
		}
		else {
			prevM = currentM;
			
		}
	}
	return EXIT_SUCCESS;
}
