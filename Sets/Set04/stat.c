#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define CHARS 222

typedef long int Statistique[CHARS];

FILE* demander_fichier();
void initialise_statistique(Statistique s);
unsigned long int collecte_statistique(Statistique a_remplir, FILE* fichier_a_lire);
void affiche(Statistique s, unsigned long int total);


const int TRY_MAX = 3;

int main(void){
	FILE* fichier = demander_fichier();
	if( fichier == NULL ){
		printf("=> j'abandonne !\n");
		return 1;
	}
	Statistique s;
	initialise_statistique(s);
	//printf("initialise statistique DONE\n");
	size_t total = collecte_statistique(s, fichier);
	//printf("collecte statistique DONE\n");
	affiche(s, total);
	fclose(fichier);
	return 0;
}

FILE* demander_fichier(){
	char nom_fichier[FILENAME_MAX] = "";
	FILE* entree = NULL;
	int try = 0;
	while( try < TRY_MAX && entree == NULL){
		printf("nom de fichier à lire :\n");
		fgets(nom_fichier, FILENAME_MAX-1, stdin);
		//printf("HI\n");
		size_t len = strlen(nom_fichier) - 1;
		if((len >= 0) && (nom_fichier[len] == '\n')){
			nom_fichier[len] = '\0';
		}
		entree = fopen(nom_fichier, "r");
		if(entree == NULL){
			printf(" -> ERREUR, je ne peux pas lire le fichier \"%s\"\n", nom_fichier);
		}
		++try;
		//printf("(%d)", try<TRY_MAX);
	}
	if(entree != NULL){
		printf(" -> OK, fichier \"%s\" ouvert pour lecture.\n", nom_fichier);
	}
	return entree;
}

void initialise_statistique(Statistique s){
	for(int i=0; i<CHARS; ++i){
		s[i] = 0;
	}
	return;
}

unsigned long int collecte_statistique(Statistique a_remplir, FILE* fichier_a_lire){
	unsigned long int counter = 0;
	while(!feof(fichier_a_lire)){
		int c =  getc(fichier_a_lire);
		++a_remplir[c-32];
		++counter;
	}
	return counter;
}

void affiche(Statistique s, unsigned long int total){
	printf("STATISTIQUES :\n");
	for(int i=0; i<CHARS; ++i){
		unsigned long int q = s[i];
		if(q > 0){
			char c = i+32;
			double p = (double)100*q/total;
			printf("%c :\t%zu\t-\t%f%%\n", c, q, p);
		}
	}
}
