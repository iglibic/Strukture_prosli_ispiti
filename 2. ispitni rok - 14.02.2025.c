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

struct number_;
typedef struct number_ *NumberP;
typedef struct number_ {
  int value;
  int no_repeats;
};
