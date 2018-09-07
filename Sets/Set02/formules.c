#include <stdio.h>
#include <math.h>
#include <stdbool.h>

const double M_E = 2.7182818284590452354;

void solve(double x, int i, bool b);

int main(void) {
    double x;
    scanf("%lf", &x);
    solve(x, 1, x!=0);
    solve(x, 2, x>0 && x!=1);
    solve(x, 3, (x*x-8*x) >= 0 && x!=2 );
    solve(x, 4, x!=0 && sqrt(x*x-1/x) > 0 && (sin(x) - x/20)*(log(sqrt(x*x-1/x))) >= 0);
}


void solve(double x, int i, bool b) {
    if (b) {
        double v;
        switch(i) {
        case 1 :
            v = x/(1-pow(M_E, x));
            break;
        case 2 :
            v = x*log(x)*pow(M_E, 2/(x-1));
            break;
        case 3 :
            v = (-x - sqrt(x*x - 8*x))/(2 - x);
            break;
        case 4 :
            v = sqrt((sin(x) - x/20)*(log(sqrt(x*x-1/x))));
            break;
        }
        printf("Expression %d = %f\n", i, v);
    } else {
        printf("Expression %d indefinie\n", i);
    }
}
