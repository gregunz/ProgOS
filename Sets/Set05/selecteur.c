#include <stdio.h>

int main(void){
	double valeur1 = 5678.15,
		valeur2 = 1651.1,
		valeur3 = 120.0;
	
	double* choix;
	
	int num;
	printf("Choisissez une nombre entre 1 et 3\n");
	scanf("%d", &num);
	switch(num){
		case 1: choix = &valeur1; break;
		case 2: choix = &valeur2; break;
		case 3: choix = &valeur3; break;
		default: printf("J'avais dit entre 1 et 3\n"); return 1;
	}
	printf("nombre choisi : %f", *choix);
	
	return 0;
}
