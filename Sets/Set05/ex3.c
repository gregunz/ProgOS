#include <stdio.h>

typedef unsigned char octet;

void afficher_binaire(octet o);
void afficher(size_t i, octet o);
void dump_mem(const size_t N, octet* o);

void dump_mem(const size_t N, octet* o){
    printf("à partir de %p :\n", o);
    for(int i=N-1; i>=0; --i){
        afficher(i, o[i]);
    }
}

void afficher(size_t i, octet o){
	printf("%02zu : ", i);
	afficher_binaire(o);
	printf(" %3u ", o);
	if(o > 31 && o < 127){
		printf("'%c'", o);
	}
	printf("\n");
	return;
}

void afficher_binaire(octet o){
	int v = 128;
	while(v > 0){
		if(o >= v){
			printf("1");
			o -= v;
		}else{
			printf("0");
		}
		v /= 2;
	}
	return;
}

int main(void){
	int i1 = 80, i2 = -80;
	double d1 = 0.5, d2 = 0.1;
	dump_mem(sizeof(i1), &i1);
	dump_mem(sizeof(i2), &i2);
	dump_mem(sizeof(d1), &d1);
	dump_mem(sizeof(d2), &d2);
}
