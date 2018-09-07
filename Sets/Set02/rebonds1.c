#include <stdio.h>
#include <math.h>

double askValue(const char* string);

const double G = 9.81;

int main(void)
{
    double h0 = askValue("de la hauteur initiale"),
            eps = askValue("du coefficient de frottement"),
            nbr = askValue("du nombre de rebond");

    double v;
    for(int i=0; i<nbr; ++i){
        v = sqrt(2*h0*G)*eps;
        h0 = v*v/(2*G);
    }

    printf("%f",h0);
    return 0;
}

double askValue(const char* string)
{
    printf("Entrez la valeur %s ?\n",string);
    double val;
    scanf("%lf", &val);
    return val;
}
