#include <stdio.h>

void echange(int* a, int* b);

void echange(int* a, int* b){
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

int main(void){
    int ij[2] = {10, 55};
	//int i = 10;
	//int j = 55;
	printf("Avant : i=%d et j=%d\n", ij[0], ij[1]);
	echange(&ij[0], &ij[1]);
	printf("Après : i=%d et j=%d\n", ij[0], ij[1]);
	printf("%lf", )
	return 0;
}
