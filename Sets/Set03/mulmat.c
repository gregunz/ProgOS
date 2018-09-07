#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 10

typedef struct {
    unsigned int m;
    unsigned int n;
    double matrice[N][N];
} Matrice;

Matrice lire_matrice(void);
Matrice multiplication(const Matrice a, const Matrice b);
void  affiche_matrice(const Matrice m);

int askUInt(const unsigned int inf, const unsigned int sup);
int askUIntError(const unsigned int inf, const unsigned int sup, const char* errorMessage);

int main(void) {
    Matrice M1, M2;
    M1 = lire_matrice();
    M2 = lire_matrice();
    affiche_matrice(multiplication(M1, M2));
    return 0;
}

Matrice lire_matrice(void) {
    printf("Création d'une matrice (m x n) \nEntrez la valeur de m\n");
    unsigned int m = askUInt(0, N);
    printf("Entrez la valeur de n:\n");
    unsigned int n = askUInt(0, N);
    printf("Entrez les valeurs de votre matrice ligne par ligne:\n");
    Matrice mat;
    mat.m = m;
    mat.n = n;
    for(int i=0; i<m; ++i) {
        for(int j=0; j<n; ++j) {
            scanf("%lf", &(mat.matrice[i][j]));
        }
    }
    return mat;
}

Matrice multiplication(const Matrice a, const Matrice b) {
    if(a.n != b.m) {
        printf("colonne a != colonne b");
        exit(1);
    }
    Matrice mat;
    mat.m = a.m;
    mat.n = b.n;
    for(int i=0; i<a.m; ++i) {
        for(int j=0; j<b.n; ++j) {
            double val = 0;
            for(int k=0; k<a.n; ++k) {
                val += a.matrice[i][k] * b.matrice[k][j];
            }
            mat.matrice[i][j] = val;
        }
    }
    return mat;
}

void  affiche_matrice(const Matrice m) {
    printf("{\n");
    for(int i=0; i<m.m; ++i) {
        printf(" {");
        for(int j=0; j<m.n; ++j) {
            printf(" %f", m.matrice[i][j]);
            if(j<m.n-1)
                printf(",");
        }
        printf("}");
        if(i<m.m-1)
            printf(",");
        printf("\n");
    }
    printf("}");
}

int askUInt(const unsigned int inf, const unsigned int sup) {
    askUIntError(inf, sup, "la valeur n'est pas comprise dans l'ensemble [%d, %d[\nReessayez :\n");
}

int askUIntError(const unsigned int inf, const unsigned int sup, const char* errorMessage) {
    int val;
    bool firstTry = true;
    do {
        if(!firstTry) {
            printf(errorMessage, inf, sup);
        }
        firstTry = true;
        scanf("%u", &val);
    } while(val<=inf || val>N);
    return val;
}
