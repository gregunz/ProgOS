#include <stdio.h>
#include <math.h>

double askValue(const char* string);

const double G = 9.81;

int main(void) {
    double h0 = askValue("de la hauteur initiale"),
           eps = askValue("du coefficient de frottement"),
           h_fin = askValue("de la hauteur finale désirée");

    double v;
    int i=0;
    do {
        ++i;
        v = sqrt(2*h0*G)*eps;
        h0 = v*v/(2*G);
    } while(h0 > h_fin);

    printf("%d",i);
    return 0;
}

double askValue(const char* string) {
    printf("Entrez la valeur %s ?\n",string);
    double val;
    scanf("%lf", &val);
    return val;
}
