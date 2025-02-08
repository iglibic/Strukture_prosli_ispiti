// Napisati program koji čita datoteku koja sadrži podatke o
// osobama ("people.txt") te kreirati vezanu listu osoba sortiranju
// po prezimenu i imenu.
// Nakon toga iz datoteke "vehicle.txt", treba pročitati podatke o
// automibilima koje posjeduje pojedina osoba i za svaku osobu
// kreirati dodatnu vezanu listu koja sadrži podatke o njenim
// vozilima.

// Osoba je definirana strukturom:

#define MAX_ID_LEN (8)
#define MAX_NAME_LEN (64)
struct _person;
typedef struct _person* PersonP;
typedef struct _person {
	char id[MAX_NAME_LEN];
	char lastName[MAX_NAME_LEN];
	PersonP next;
	VehicleP child;
} People;

// Vozila su definirana strukturom:

struct _vehicle;
typedef struct _vehicle *VehicleP;
typedef struct _vehicle {
	char brandName[MAX_NAME_LEN];
	char modelName[MAX_NAME_LEN];
	DateP date;
	VehicleP next;
} Vehicle;

// Datum je definiran strukturom: 

struct _date;
typedef struct _date *DateP;
typedef struct _date {
	int year;
	int month;
	int day;
} Date;

// OCJENA 2: Ispisati podatke o svim osobama i njihovimvozilima, 
// uz uvjet da se za osobe koje nemaju vozila ispiše "NEMA VOZILA".

// OCJENA 3: Pronaći sva vozila koja na današnji datum nemaju
// važeću registraciju (nije važno u kojem je trenutku istekla) i
// ispisati podatke o tim vozilima i njihovim vlasnicima.

