#include <stdio.h>

#define TAILLE_MAX 100

int nextToken(char const * str, int* from, int* len);

int nextToken(char const * str, int* from, int* len){
    int i = *from + *len;
    // purge space
    for(; str[i] == ' '; ++i);
    *from = i;

    for(; str[i] != ' ' && str[i] != '\0' && str[i] != '\n'; ++i);

    if( *from == i ){
        return 0;
    }

    *len = i - *from;
    return 1;
}

int main(void){

    char ligne_a_lire[TAILLE_MAX];
    fgets(ligne_a_lire, TAILLE_MAX, stdin);
    int from = 0, len = 0;

    while( nextToken(ligne_a_lire, &from, &len) > 0){
        //printf("%d, %d\n", from, len);
        printf("\"");
        for( int i = from; i < from + len; ++i){
            printf("%c", ligne_a_lire[i]);
        }
        printf("\"\n");
    }

    return 0;
}
