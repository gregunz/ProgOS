#include <stdio.h>

typedef double (*Fonction)(double);

double id(double a);
double integre(Fonction f, double a);

double integre(Fonction f, double a){
    return f(a);
}

double id(double a){
    return a;
}

int main(void){
    printf( "%f", integre(id, 2));
    return 0;
}
