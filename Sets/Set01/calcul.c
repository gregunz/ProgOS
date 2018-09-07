#include <stdio.h>

int main() {

int x, y;
double a, b, c, d;

x = 2;
y = 4;

a = x + y;
b = x - y;
c = x * y;
d = x / (double)y;

printf("%lf, %lf, %lf, %lf",a,b,c,d);

}
