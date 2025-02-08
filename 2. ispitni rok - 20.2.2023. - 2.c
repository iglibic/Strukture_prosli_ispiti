#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LEN 32

struct _kategorija;
typedef struct _kategorija* KategorijaP;
typedef struct _kategorija {
    char imeKategorije[MAX_NAME_LEN];
    int minCijena;
    int maxCijena;
    float avgCijena;
    KategorijaP next;
    struct _proizvod* child;
} Kategorija;

struct _proizvod;
typedef struct _proizvod* ProizvodP;
typedef struct _proizvod {
    char proizvodIme[MAX_NAME_LEN];
    int cijena;
    ProizvodP next;
} Proizvod;

// Funkcija za generiranje slučajne cijene unutar danog opsega
int generateRandomPrice(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Funkcija za umetanje kategorije u sortiranu listu
KategorijaP insertCategory(KategorijaP head, Kategorija* newCategory) {
    KategorijaP current = head, prev = NULL;
    
    // Ako je lista prazna ili novo ime treba biti na početku
    if (head == NULL || strcmp(newCategory->imeKategorije, head->imeKategorije) < 0) {
        newCategory->next = head;
        return newCategory;
    }

    // Pronađi odgovarajuće mjesto za umetanje
    while (current != NULL && strcmp(newCategory->imeKategorije, current->imeKategorije) > 0) {
        prev = current;
        current = current->next;
    }

    // Umetni novu kategoriju
    newCategory->next = current;
    if (prev != NULL) {
        prev->next = newCategory;
    }

    return head;
}

// Funkcija za dodavanje proizvoda u kategoriju
ProizvodP insertProduct(ProizvodP head, Proizvod* newProduct) {
    newProduct->next = head;
    return newProduct;
}

// Funkcija za učitavanje kategorija iz datoteke
KategorijaP loadCategories(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file %s.\n", filename);
        return NULL;
    }

    KategorijaP head = NULL;
    Kategorija tempCategory;

    while (fscanf(file, "%s %d %d", tempCategory.imeKategorije, &tempCategory.minCijena, &tempCategory.maxCijena) == 3) {
        tempCategory.avgCijena = (tempCategory.minCijena + tempCategory.maxCijena) / 2.0;
        tempCategory.next = NULL;
        tempCategory.child = NULL;
        head = insertCategory(head, &tempCategory);
    }

    fclose(file);
    return head;
}

// Funkcija za učitavanje proizvoda iz datoteke
void loadProducts(KategorijaP categories, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file %s.\n", filename);
        return;
    }

    char categoryName[MAX_NAME_LEN];
    Proizvod tempProduct;
    KategorijaP category;
    
    while (fscanf(file, "%s %s", categoryName, tempProduct.proizvodIme) == 2) {
        fscanf(file, "%d %d", &tempProduct.cijena, &tempProduct.cijena);  // Pretpostavljamo cijenu

        // Potraži kategoriju
        category = categories;
        while (category != NULL && strcmp(category->imeKategorije, categoryName) != 0) {
            category = category->next;
        }

        // Ako kategorija postoji, dodaj proizvod
        if (category != NULL) {
            tempProduct.cijena = generateRandomPrice(category->minCijena, category->maxCijena);
            category->child = insertProduct(category->child, &tempProduct);
        }
    }

    fclose(file);
}

// Funkcija za ispis kategorija i proizvoda
void printCategories(KategorijaP head) {
    KategorijaP category = head;
    while (category != NULL) {
        printf("Kategorija: %s (Min: %d, Max: %d)\n", category->imeKategorije, category->minCijena, category->maxCijena);
        ProizvodP product = category->child;
        while (product != NULL) {
            printf("    Proizvod: %s, Cijena: %d\n", product->proizvodIme, product->cijena);
            product = product->next;
        }
        category = category->next;
    }
}

int main() {
    srand(time(NULL)); // Inicijalizacija generatora slučajnih brojeva

    KategorijaP categories = loadCategories("kategorije.txt");
    loadProducts(categories, "proizvodi.txt");

    printf("Kategorije i proizvodi:\n");
    printCategories(categories);

    // Oslobađanje memorije bi išlo ovdje (nisam implementirao oslobodjenje memorije u ovom dijelu)

    return 0;
}
